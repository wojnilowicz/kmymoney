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
#include <QSqlDatabase>
#include <QFileInfo>

// ----------------------------------------------------------------------------
// KDE Includes

#include <KPluginFactory>
#include <KLocalizedString>
#include <KBackup>

// ----------------------------------------------------------------------------
// Project Includes

#include "mymoney/mymoneyfile.h"
#include "mymoney/mymoneyexception.h"

#include "storage/interface/issuesprocessor.h"
#include "storage/interface/ivalidator.h"
#include "storage/interface/enums.h"

#include "backend/backend.h"
#include "backend/url.h"

#include "frontend/credentials.h"
#include "frontend/questioner.h"

#include "storage/interface/widget/messagebox.h"
#include "frontend/widget/opensave.h"
#include "frontend/widget/opensaveserver.h"
#include "frontend/widget/credentials.h"

#ifdef ENABLE_QUICK
#include "storage/interface/quick/messagebox.h"
#include "frontend/quick/opensave.h"
#include "frontend/quick/opensaveserver.h"
#include "frontend/quick/credentials.h"
#endif

#include "storage/interface/wallet.h"
#include "storage/interface/enums.h"
#include "configuration.h"

typedef std::function<void (const QUrl& urlWithPassphrase)> PassphraseCb;

namespace Storage {
namespace Sql {

class PluginPrivate
{
  Q_DISABLE_COPY(PluginPrivate)
  Q_DECLARE_PUBLIC(Plugin)

public:
  PluginPrivate(Plugin *q);
  ~PluginPrivate() = default;

  void readConfig();

  std::shared_ptr<IQuestioner> questionerFactory();
  std::shared_ptr<Credentials> credentialsFactory(const QString &requestText);

  void processOpenStorageIssue(MyMoneyFile &storage, const QUrl &url, eIssue issue, eAnswer answer, ProcessorCb success);
  void processSaveStorageIssue(const MyMoneyFile &storage, const QUrl &url, eIssue issue, eAnswer answer, ProcessorCb success);

  void processCredentials(const QUrl &url, eIssue issue, PassphraseCb retryWithCredentials);
  void processNeedsDatabaseCreated(const MyMoneyFile &storage, const QUrl &url);

  QUrl restoreLastUsedDirectory() const;
  void storeLastUsedDirectory(const QUrl &url);

  ProgressCallback m_callback = nullptr;
  QObject *m_parent = nullptr;
  eUI m_uiType = eUI::Unknown;

  const std::unique_ptr<Backend> m_backend;
  std::unique_ptr<MyMoneyFile> m_openedStorage;
  const MyMoneyFile *stashedStorage;
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
      return std::make_shared<Questioner>(*new Storage::Widget::MessageBox(m_parent));
#ifdef ENABLE_QUICK
    case eUI::Quick:
      return std::make_shared<Questioner>(*new Storage::Quick::MessageBox(m_parent));
#endif
    default:
      return nullptr;
  }
}

std::shared_ptr<Sql::Credentials> PluginPrivate::credentialsFactory(const QString &requestText)
{
  switch (m_uiType) {
    case eUI::Widget:
      return std::make_shared<Widget::Credentials>(requestText, m_parent);
#ifdef ENABLE_QUICK
    case eUI::Quick:
      return std::make_shared<Quick::Credentials>(requestText, m_parent);
#endif
    default:
      return nullptr;
  }
}

void PluginPrivate::processCredentials(const QUrl &url, eIssue issue, PassphraseCb retryWithCredentials)
{
  Q_Q(Plugin);
  if (Configuration::useKWalletToManagePasswords() &&
      (issue == eIssue::NeedsPassphrase ||
      issue == eIssue::NeedsCredentials)) {
    auto wallet = std::make_unique<Wallet>();
    auto urlWithCredentials = wallet->restoreCredentials(url);
    if (urlWithCredentials != url ) {
      retryWithCredentials(urlWithCredentials);
      return;
    }
  }

  QMap<eIssue, QPair<bool, QString>> credentialsMap {
    {eIssue::NeedsPassphrase,     {false, i18n("Please enter passphrase for<br><b>%1</b>")}},
    {eIssue::BadPassphrase,       {false, i18n("Bad passphrase given for<br><b>%1</b><br>Please try again.")}},
    {eIssue::NeedsCredentials,  {true,  i18n("Please enter credentials for<br><b>%1</b>")}},
    {eIssue::BadCredentials,    {true,  i18n("Bad credentials given for<br><b>%1</b><br>Please try again.")}},
  };
  auto askAboutUserName = credentialsMap.value(issue).first;
  auto requestText = credentialsMap.value(issue).second.arg(url.toString(QUrl::RemoveQuery | QUrl::RemoveUserInfo));

  auto passphraseProvider = credentialsFactory(requestText);
  auto connection = std::make_shared<QMetaObject::Connection>();
  auto slotPassphraseReceived = [=](const QString &userName, const QString &passphrase) mutable {
    q->disconnect(*connection);
    passphraseProvider.reset();
    if (!passphrase.isEmpty()) {
      auto urlWithPassphrase = url;
      urlWithPassphrase.setPassword(passphrase);
      urlWithPassphrase.setUserName(userName);
      retryWithCredentials(urlWithPassphrase);
    }
  };
  *connection = q->connect(passphraseProvider.get(), &Widget::Credentials::credentials, slotPassphraseReceived);
  if (askAboutUserName)
    passphraseProvider->getCredentials();
  else
    passphraseProvider->getPassphrase();
}

void PluginPrivate::processNeedsDatabaseCreated(const MyMoneyFile &storage, const QUrl &url)
{
  Q_Q(Plugin);

  QVector<Issue> issues;
  try {
    m_backend->createDatabase(url);
    issues.append({eIssue::DatabaseCreated, QStringList()});
  }  catch (const MyMoneyException &e) {
    issues.append({eIssue::DatabaseNotCreated, QStringList{e.what()}});
  }

  auto questioner = questionerFactory();
  auto frontendHandler = [=] () {
    return questioner.get();
  };

  auto backendHandler = [=] (eIssue issue, eAnswer answer, ProcessorCb success) {
    Q_UNUSED(issue);
    if (answer == eAnswer::Ok || answer == eAnswer::Yes)
      success(true);
    else
      success(false);
  };

  auto onFinish = [=, &storage](bool isDatabaseCreated) {
    if (isDatabaseCreated)
      q->saveStorage(storage, url);
  };

  IssuesProcessor::createIssuesProcessor(issues, frontendHandler, backendHandler, onFinish);
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

void PluginPrivate::processOpenStorageIssue(MyMoneyFile &storage, const QUrl &url, eIssue issue, eAnswer answer, ProcessorCb success)
{
  Q_Q(Plugin);

  switch (issue) {
    case eIssue::NeedsUpgrade:
      m_backend->upgradeStorage(url);
      break;

    case eIssue::NeedsConversion: {
      auto urlForConvertedStorage = m_backend->convertStorage(url, eProducer::KMyMoneyNEXT);
      q->openStorage(storage, urlForConvertedStorage);
      success(false);
    } return;
    case eIssue::NeedsPassphrase:
    case eIssue::BadPassphrase:
    case eIssue::NeedsCredentials:
    case eIssue::BadCredentials: {
      auto slotCredentialsProcessed = [=, &storage](const QUrl& urlWithCredentials) {
        q->openStorage(storage, urlWithCredentials);
      };

      processCredentials(url, issue, slotCredentialsProcessed);
      success(false);
    } return;
    default:
      break;
  }

  if (answer == eAnswer::Yes)
    success(true);
  else
    success(false);
}

void PluginPrivate::processSaveStorageIssue(const MyMoneyFile &storage, const QUrl &url, eIssue issue, eAnswer answer, ProcessorCb success)
{
  Q_Q(Plugin);

  switch (issue) {
    case eIssue::NeedsPassphrase:
    case eIssue::BadPassphrase:
    case eIssue::NeedsCredentials:
    case eIssue::BadCredentials: {
      auto slotCredentialsProcessed = [=, &storage](const QUrl& urlWithCredentials) {
        q->saveStorage(storage, urlWithCredentials);
      };
      processCredentials(url, issue, slotCredentialsProcessed);

      success(false);
    } return;
    case eIssue::NeedsDatabaseCreated: {
      processNeedsDatabaseCreated(storage, url);
      success(false);
    } return;
    default:
      break;
  }

  if (answer == eAnswer::Yes)
    success(true);
  else
    success(false);
}

Plugin::Plugin(QObject *parent, const QVariantList &args) :
  d_ptr(std::make_unique<PluginPrivate>(this))
{
  Q_UNUSED(args)
  Q_UNUSED(parent)

  setObjectName("storagesql");
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
  d->m_parent = parent;
}

void Plugin::setUiType(eUI type)
{
  Q_D(Plugin);
  d->m_uiType = type;
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
      if (Configuration::useKWalletToManagePasswords()) {
        auto wallet = std::make_unique<Wallet>();
        wallet->storeCredentials(url);
      }

      if (d->m_backend->isStorageFileBased(url)) {
        d->storeLastUsedDirectory(url);
      }
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
    d->processOpenStorageIssue(storage, url, issue, answer, processorCb);
  };

  IssuesProcessor::createIssuesProcessor(issues, frontendHandler, backendHandler, finishHandler);
}

void Plugin::saveStorage(const MyMoneyFile &storage, const QUrl &url)
{
  Q_D(Plugin);

  d->readConfig();

  const auto finishHandler = [=, &storage](bool success){
    if (success) {
      if (d->m_backend->isStorageFileBased(url)) {
        if (Configuration::keepSoManyBackups())
          KBackup::numberedBackupFile(url.path(), QString(), QStringLiteral("~"), Configuration::keepSoManyBackups());
        d->storeLastUsedDirectory(url);
      }

      d->m_backend->saveStorage(storage, url);

      emit storageSaved(url);
      if (Configuration::useKWalletToManagePasswords()) {
        auto wallet = std::make_unique<Wallet>();
        wallet->storeCredentials(url);
      }
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
    d->processSaveStorageIssue(storage, url, issue, answer, processorCb);
  };

  IssuesProcessor::createIssuesProcessor(issues, frontendHandler, backendHandler, finishHandler);
}

void Plugin::closeStorage(const QUrl &url)
{
  Q_D(Plugin);
  d->m_backend->closeConnection(url);
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

  switch (storageType) {
    case eType::kmm_SQLite:
    case eType::kmmn_SQLite:
    case eType::kmm_SqlCipher:
    case eType::kmmn_SqlCipher:
      switch (d->m_uiType) {
        case eUI::Widget:
          return new Widget::OpenSave(d->m_backend.get(), storageType, eAction::Open, d->restoreLastUsedDirectory());
#ifdef ENABLE_QUICK
        case eUI::Quick:
          return new Quick::OpenSave(d->m_backend.get(), storageType, eAction::Open, d->restoreLastUsedDirectory());
#endif
        default:
          break;
      }
      break;

    case eType::kmm_MySql:
    case eType::kmmn_MySql:
    case eType::kmm_PostgreSql:
    case eType::kmmn_PostgreSql:

      switch (d->m_uiType) {
        case eUI::Widget:
          return new Widget::OpenSaveServer(d->m_backend.get(), storageType, eAction::Open);
#ifdef ENABLE_QUICK
        case eUI::Quick:
          return new Quick::OpenSaveServer(d->m_backend.get(), storageType, eAction::Open);
#endif
        default:
          break;
      }
      break;

    default:
      break;;
  }


  return nullptr;
}

IDialogPartOpenSave *Plugin::saveAsDialogPart(eType storageType)
{
  Q_D(Plugin);

  d->readConfig();

  switch (storageType) {
    case eType::kmm_SQLite:
    case eType::kmmn_SQLite:
    case eType::kmm_SqlCipher:
    case eType::kmmn_SqlCipher:
      switch (d->m_uiType) {
        case eUI::Widget:
          return new Widget::OpenSave(d->m_backend.get(), storageType, eAction::Save, d->restoreLastUsedDirectory());
#ifdef ENABLE_QUICK
        case eUI::Quick:
          return new Quick::OpenSave(d->m_backend.get(), storageType, eAction::Save, d->restoreLastUsedDirectory());
#endif
        default:
          break;
      }
      break;

    case eType::kmm_MySql:
    case eType::kmmn_MySql:
    case eType::kmm_PostgreSql:
    case eType::kmmn_PostgreSql:

      switch (d->m_uiType) {
        case eUI::Widget:
          return new Widget::OpenSaveServer(d->m_backend.get(), storageType, eAction::Save);
#ifdef ENABLE_QUICK
        case eUI::Quick:
          return new Quick::OpenSaveServer(d->m_backend.get(), storageType, eAction::Save);
#endif
        default:
          break;
      }
      break;

    default:
      break;;
  }
  return nullptr;
}

std::unique_ptr<MyMoneyFile> Plugin::storageManager()
{
  Q_D(Plugin);
  return std::move(d->m_openedStorage);
}

K_PLUGIN_FACTORY_WITH_JSON(PluginFactory, "plugin.json", registerPlugin<Plugin>();)

} // namespace Sql
} // namespace Storage

#include "plugin.moc"
