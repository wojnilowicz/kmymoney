/*
 * Copyright 2020  Łukasz Wojniłowicz <lukasz.wojnilowicz@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "plugin.h"
#include "cmakedefine.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QVector>
#include <QDebug>
#include <QUrlQuery>
#include <QRegularExpression>
#include <QWidget>
#include <QFileInfo>

// ----------------------------------------------------------------------------
// KDE Includes

#include <KPluginFactory>
#include <KLocalizedString>
#include <KBackup>

// ----------------------------------------------------------------------------
// Project Includes

#include "mymoney/mymoneyexception.h"
#include "mymoney/mymoneyfile.h"

#include "storage/interface/issuesprocessor.h"
#include "storage/interface/ivalidator.h"
#include "storage/interface/enums.h"

#include "backend/backend.h"
#include "backend/url.h"

#include "frontend/widget/opensave.h"
#ifdef ENABLE_GPG
#include "frontend/widget/saveasencrypted.h"
#endif
#include "frontend/widget/questioner.h"

#ifdef ENABLE_QUICK
#include "frontend/quick/opensave.h"
#include "frontend/quick/saveasencrypted.h"
#include "frontend/quick/questioner.h"
#endif

#include "configuration.h"

#ifdef ENABLE_GPG
#include "gpg/frontend/widget/keydownloader.h"
#include "gpg/backend/backend.h"
#include "gpg/frontend/widget/passphraseprovider.h"
#ifdef ENABLE_QUICK
#include "gpg/frontend/quick/keydownloader.h"
#endif
#endif

namespace Storage {
namespace Xml {

class PluginPrivate
{
  Q_DISABLE_COPY(PluginPrivate)
  Q_DECLARE_PUBLIC(Plugin)

public:
  PluginPrivate(Plugin *q);
  ~PluginPrivate() = default;

  void readConfig();

  std::shared_ptr<IQuestioner> questionerFactory();
#ifdef ENABLE_GPG
  std::shared_ptr<Gpg::KeyDownloader> keyDownloaderFactory();
#endif
  void processOpenSaveStorageIssues(MyMoneyFile &storage, const QUrl &url, eIssue issue, eAnswer answer, ProcessorCb success);
  void processMissingRecoveryKey(const QUrl &url, eIssue issue, eAnswer answer, ProcessorCb success);
  void processMissingKeys(MyMoneyFile &storage, const QUrl &url, eIssue issue, eAnswer answer, ProcessorCb success);
  void setPassphraseProvider(eUI type, QObject *parent);

  QUrl restoreLastUsedDirectory() const;
  void storeLastUsedDirectory(const QUrl &url);

  ProgressCallback m_callback = nullptr;
  QObject *m_parent = nullptr;
  eUI m_uiType = eUI::Unknown;

  const std::unique_ptr<Backend> m_backend;
  std::unique_ptr<MyMoneyFile> m_openedStorage = nullptr;
  QDateTime m_lastConfigRead;
  Plugin *q_ptr;
};

PluginPrivate::PluginPrivate(Plugin *q) :
  m_backend(new Backend),
  q_ptr(q)
{
}

void PluginPrivate::readConfig()
{
  const auto configFileName = Configuration::self()->config()->name();
  const auto configFilePath = QStandardPaths::locate(QStandardPaths::GenericConfigLocation, configFileName);

  const QFileInfo configFile(configFilePath);
  if (m_lastConfigRead > configFile.lastModified())
    return;

  Configuration::self()->load();

  m_lastConfigRead = QDateTime::currentDateTime();
}

std::shared_ptr<IQuestioner> PluginPrivate::questionerFactory()
{
  switch (m_uiType) {
    case eUI::Widget:
      return std::make_shared<Widget::Questioner>(m_parent);
#ifdef ENABLE_QUICK
    case eUI::Quick:
      return std::make_shared<Quick::Questioner>(m_parent);
#endif
    default:
      return nullptr;
  }
}

#ifdef ENABLE_GPG
std::shared_ptr<Gpg::KeyDownloader> PluginPrivate::keyDownloaderFactory()
{
  switch (m_uiType) {
    case eUI::Widget:
      return std::make_shared<Gpg::Widget::KeyDownloader>(m_parent);
#ifdef ENABLE_QUICK
    case eUI::Quick:
      return std::make_shared<Gpg::Quick::KeyDownloader>(m_parent);
#endif
    default:
      return nullptr;
  }
}
#endif

void PluginPrivate::processOpenSaveStorageIssues(MyMoneyFile &storage, const QUrl &url, eIssue issue, eAnswer answer, ProcessorCb success)
{
  switch (issue) {
    case eIssue::NeedsUpgrade:
      m_backend->upgradeStorage(url);
      break;

    case eIssue::NeedsConversion:
      m_backend->upgradeStorage(url);
      break;

#ifdef ENABLE_GPG
    case eIssue::MissingKeys:
      processMissingKeys(storage, url, issue, answer, success);
      return;
    case eIssue::UnableToDetectKeys:
      success(false);
      break;
    case eIssue::MissingRecoveryKey:
      if (answer == eAnswer::Yes) {
        processMissingRecoveryKey(url, issue, answer, success);
        return;
      }
      break;
#endif
    default:
      break;
  }

  if (answer == eAnswer::Yes)
    success(true);
  else
    success(false);
}

#ifdef ENABLE_GPG
void PluginPrivate::processMissingRecoveryKey(const QUrl &url, eIssue issue, eAnswer answer, ProcessorCb success)
{
  Q_UNUSED(url);
  Q_UNUSED(issue);
  Q_UNUSED(answer);

  Q_Q(Plugin);
  auto keyDownloader = keyDownloaderFactory();
  auto connection = std::make_shared<QMetaObject::Connection>();
  auto slotKeyDownloaderFinished = [=](bool downloadSuccess) mutable {
    Q_UNUSED(downloadSuccess);
    q->disconnect(*connection);
    keyDownloader.reset();
    success(true); // don't hold the user, if he doesn't want the key
  };
  *connection = q->connect(keyDownloader.get(), &Gpg::KeyDownloader::result, slotKeyDownloaderFinished);
  keyDownloader->downloadRecoveryKey();
}

void PluginPrivate::processMissingKeys(MyMoneyFile &storage, const QUrl &url, eIssue issue, eAnswer answer, ProcessorCb success)
{
  Q_Q(Plugin);
  Q_UNUSED(issue);
  Q_UNUSED(answer);
  success(false);
  const auto subKeys = m_backend->gpg()->fileEncryptionKeys(url.path());
  const auto mainKeys = m_backend->gpg()->mainKeysFromSubKeys(subKeys);
  if (!mainKeys.isEmpty()) {
    Url enhancedUrl(url);
    enhancedUrl.setEncryptionKeys(mainKeys);
    q->openStorage(storage, enhancedUrl);
    return;
  }

  QVector<Issue> issues;
  issues.append({eIssue::UnableToDetectKeys, QStringList()});

  auto questioner = questionerFactory();
  auto frontendHandler = [=] () {
    return questioner.get();
  };

  auto backendHandler = [=, &storage] (eIssue issue, eAnswer answer, ProcessorCb success) {
    processOpenSaveStorageIssues(storage, url, issue, answer, success);
  };

  auto onSuccess = [=](bool alwaysFalse) {
    Q_UNUSED(alwaysFalse);
  };

  IssuesProcessor::createIssuesProcessor(issues, frontendHandler, backendHandler, onSuccess);
}
#endif

void PluginPrivate::setPassphraseProvider(eUI type, QObject *parent)
{
#ifdef ENABLE_GPG
  if (!m_backend->gpg()->isOpeartional())
    return;

  if (type == eUI::Quick)
    m_backend->gpg()->setPassphraseProvider(nullptr);
  else if (type == eUI::Widget)
    m_backend->gpg()->setPassphraseProvider(std::make_unique<Gpg::Widget::PassphraseProvider>(qobject_cast<QWidget *>(parent), Configuration::useKWalletToManagePasswords()));
#else
  Q_UNUSED(type);
  Q_UNUSED(parent);
#endif
}

QUrl PluginPrivate::restoreLastUsedDirectory() const
{
  auto startDir = Configuration::self()->lastDirectory();
  if (startDir.isEmpty())
    startDir = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first();
  return QUrl::fromLocalFile(startDir);
}

void PluginPrivate::storeLastUsedDirectory(const QUrl &url)
{
  QFileInfo fileInfo(url.path());
  Configuration::self()->setLastDirectory(fileInfo.absolutePath());
  Configuration::self()->save();
}

Plugin::Plugin(QObject *parent, const QVariantList &args) :
  d_ptr(std::make_unique<PluginPrivate>(this))
{
  Q_UNUSED(args)
  Q_UNUSED(parent)

  setObjectName("storagexml");
  qDebug() << QString::fromLatin1("Plugins: %1 loaded").arg(objectName());
}

Plugin::~Plugin()
{
  qDebug() << QString::fromLatin1("Plugins: %1 unloaded").arg(objectName());
}

void Plugin::setProgressCallback(Storage::ProgressCallback callback)
{
  Q_D(Plugin);
  d->m_callback = callback;
  d->m_backend->setProgressCallback(callback);
}

void Plugin::setParent(QObject *parent)
{
  Q_D(Plugin);
  if (parent != d->m_parent) {
    d->m_parent = parent;
    d->setPassphraseProvider(d->m_uiType, parent);
  }
}

void Plugin::setUiType(eUI type)
{
  Q_D(Plugin);
  if (d->m_uiType != type) {
    d->m_uiType = type;
    d->setPassphraseProvider(type, d->m_parent);
  }
}

QVector<eType> Plugin::types() const
{
  Q_D(const Plugin);
  return d->m_backend->types();
}

QString Plugin::typeDisplayedName(eType storageType) const
{
  Q_D(const Plugin);
  return d->m_backend->typeDisplayedName(storageType);
}

QString Plugin::typeInternalName(eType storageType) const
{
  Q_D(const Plugin);
  return d->m_backend->typeInternalName(storageType);
}

bool Plugin::canOpen(eType storageType) const
{ 
  Q_D(const Plugin);
  const auto &canOpenTypes = d->m_backend->canOpenTypes();
  if (canOpenTypes.contains(storageType))
    return true;
  return false;
}

bool Plugin::canSave(eType storageType) const
{
  Q_D(const Plugin);
  const auto &canSaveTypes = d->m_backend->canSaveTypes();
  if (canSaveTypes.contains(storageType))
    return true;
  return false;
}

QString Plugin::scheme() const
{
  return Url::pluginScheme();
}

void Plugin::openStorage(MyMoneyFile &storage, const QUrl &url)
{
  Q_D(Plugin);

  d->readConfig();

  auto finishHandler = [=, &storage](bool success){
    if (success) {
      d->m_backend->openStorage(storage, url);
      emit storageOpened(url);
      d->storeLastUsedDirectory(url);
    }
  };

  const auto issues = d->m_backend->validateForOpen(url);

  if (issues.isEmpty()) {
    finishHandler(true);
    return;
  }
  auto questioner = d->questionerFactory();
  auto frontendHandler = [=] () {
    return questioner.get();
  };

  auto backendHandler = [=, &storage] (eIssue issue, eAnswer answer, ProcessorCb processorCb) {
    d->processOpenSaveStorageIssues(storage, url, issue, answer, processorCb);
  };

  IssuesProcessor::createIssuesProcessor(issues, frontendHandler, backendHandler, finishHandler);
}

void Plugin::saveStorage(const MyMoneyFile &storage, const QUrl &url)
{
  Q_D(Plugin);

  d->readConfig();

  const auto finishHandler = [=, &storage](bool success){
    if (success) {
      d->m_backend->saveStorage(storage, url);
      if (Configuration::keepSoManyBackups())
        KBackup::numberedBackupFile(url.path(), QString(), QStringLiteral("~"), Configuration::keepSoManyBackups());
      emit storageSaved(url);
      d->storeLastUsedDirectory(url);
    }
  };

  const auto issues = d->m_backend->validateForSave(url);

  if (issues.isEmpty()) {
    finishHandler(true);
    return;
  }

  auto questioner = d->questionerFactory();
  auto frontendHandler = [=] () {
    return questioner.get();
  };

  auto backendHandler = [=, &storage] (eIssue issue, eAnswer answer, ProcessorCb processorCb) {
    d->processOpenSaveStorageIssues(const_cast<MyMoneyFile &>(storage), url, issue, answer, processorCb);
  };

  IssuesProcessor::createIssuesProcessor(issues, frontendHandler, backendHandler, finishHandler);
}

void Plugin::closeStorage(const QUrl &url)
{
  Q_UNUSED(url);
}

eType Plugin::storageType(const QUrl &url)
{
  Q_D(Plugin);
  return d->m_backend->storageType(url);
}

IDialogPartOpenSave *Plugin::openDialogPart(eType storageType)
{
  Q_D(Plugin);

  d->readConfig();

  switch (d->m_uiType) {
    case eUI::Widget:
      return new Widget::OpenSave(d->m_backend.get(), storageType, eAction::Open, d->restoreLastUsedDirectory());
#ifdef ENABLE_QUICK
    case eUI::Quick:
      return new Quick::OpenSave(d->m_backend.get(), storageType, eAction::Open, d->restoreLastUsedDirectory());
      break;
#endif
    default:
      break;
  }
  return nullptr;
}

IDialogPartOpenSave *Plugin::saveAsDialogPart(eType storageType)
{
  Q_D(Plugin);

  d->readConfig();

  switch (storageType) {
    case eType::kmm_XmlGzip:
    case eType::kmm_XmlPlain:
    case eType::kmm_XmlAnonymous:
    case eType::kmmn_XmlGzip:
    case eType::kmmn_XmlPlain:
    case eType::kmmn_XmlAnonymous:
      switch (d->m_uiType) {
        case eUI::Widget:
          return new Widget::OpenSave(d->m_backend.get(), storageType, eAction::Save, d->restoreLastUsedDirectory());
#ifdef ENABLE_QUICK
        case eUI::Quick:
          return new Quick::OpenSave(d->m_backend.get(), storageType, eAction::Save, d->restoreLastUsedDirectory());
#endif
        default:
          break;
      } break;
#ifdef ENABLE_GPG
    case eType::kmm_XmlGpg:
    case eType::kmmn_XmlGpg:
      switch (d->m_uiType) {
        case eUI::Widget:
          return new Widget::SaveAsEncrypted(d->m_parent, d->m_backend.get(), storageType, d->restoreLastUsedDirectory());
#ifdef ENABLE_QUICK
        case eUI::Quick:
          return new Quick::SaveAsEncrypted(d->m_parent, d->m_backend.get(), storageType, d->restoreLastUsedDirectory());
#endif
        default:
          break;
      } break;
#endif
    default:
      break;
  }
  return nullptr;
}

std::unique_ptr<MyMoneyFile> Plugin::storageManager()
{
  Q_D(Plugin);
  return std::move(d->m_openedStorage);
}

K_PLUGIN_FACTORY_WITH_JSON(PluginFactory, "plugin.json", registerPlugin<Plugin>();)

} // namespace Xml
} // namespace Storage

#include "plugin.moc"
