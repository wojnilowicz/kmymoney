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

#include <QDebug>
#include <QUrlQuery>
#include <QFileInfo>

// ----------------------------------------------------------------------------
// KDE Includes

#include <KPluginFactory>
#include <KLocalizedString>
#include <KActionCollection>
#include <KPluginLoader>
#include <KPluginMetaData>

// ----------------------------------------------------------------------------
// Project Includes

#include "mymoney/mymoneyfile.h"

#include "storage/interface/issuesprocessor.h"
#include "storage/interface/ivalidator.h"
#include "storage/interface/icallback.h"
#include "storage/interface/enums.h"
#include "storage/interface/iurl.h"
#include "storage/interface/ui.h"

#include "frontend/opensave.h"
#include "frontend/questioner.h"

#include "storage/interface/widget/messagebox.h"
#include "frontend/widget/opensave.h"

#ifdef ENABLE_QUICK
#include "storage/interface/quick/messagebox.h"
#include "frontend/quick/opensave.h"
#endif

#include "configuration.h"

typedef QMap<QString, Storage::IStoragePlugin *> StoragePluginsMap;

namespace Storage {
namespace Manager {

class PluginPrivate
{
public:
  ~PluginPrivate() = default;

  void readConfig();
  std::shared_ptr<IQuestioner> questionerFactory();


  StoragePluginsMap storagePluginsMap() const;
  void setPluginsParent(QObject *parent) const;
  void warnOfMissingPlugins(const QVector<Issue> &issues);

  std::unique_ptr<Manager::OpenSave> m_dialogOpenSave;
  ProgressCallback m_callback = nullptr;
  QObject *m_parent = nullptr;
  eUI m_uiType = eUI::Unknown;

  QDateTime m_lastConfigRead;
};

void PluginPrivate::readConfig()
{
  const auto configFileName = Configuration::self()->config()->name();
  const auto configFilePath = QStandardPaths::locate(QStandardPaths::GenericConfigLocation, configFileName);

  const QFileInfo configFile(configFilePath);
  if (m_lastConfigRead > configFile.lastModified())
    return;

  Configuration::self()->load();

  m_uiType = StorageUi::typeEnum(Configuration::self()->uiType());

  if (m_uiType == eUI::Unknown)
    m_uiType = eUI::Widget;

  // Warning! plugins may be empty for the first time
  for (const auto &plugin : pPlugins.newstorage)
    plugin->setUiType(m_uiType);

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

StoragePluginsMap PluginPrivate::storagePluginsMap() const
{
  StoragePluginsMap storagePluginsMap;
  for (const auto &plugin : pPlugins.newstorage)
    storagePluginsMap.insert(plugin->objectName(), plugin);
  return storagePluginsMap;
}

void PluginPrivate::setPluginsParent(QObject *parent) const
{
  const auto plugins = storagePluginsMap();
  for (const auto &plugin : plugins)
    plugin->setParent(parent);
}

void PluginPrivate::warnOfMissingPlugins(const QVector<Issue> &issues)
{
  const auto finishHandler = [](bool success){
    Q_UNUSED(success);
  };

  auto questioner = questionerFactory();
  auto frontendHandler = [=] () {
    return questioner.get();
  };

  auto backendHandler = [] (eIssue issue, eAnswer answer, ProcessorCb processorCb) {
    Q_UNUSED(issue); Q_UNUSED(answer); Q_UNUSED(processorCb);
  };

  IssuesProcessor::createIssuesProcessor(issues, frontendHandler, backendHandler, finishHandler);
}

Plugin::Plugin(QObject *parent, const QVariantList &args) :
  d_ptr(new PluginPrivate)
{
  Q_UNUSED(parent)
  Q_UNUSED(args)
  Q_D(Plugin);

  setObjectName("storagemanager");
  qDebug() << QString::fromLatin1("Plugins: %1 loaded").arg(objectName());
  d->readConfig();
}

Plugin::~Plugin()
{
  delete Configuration::self();
  qDebug() << QString::fromLatin1("Plugins: %1 unloaded").arg(objectName());
}

void Plugin::setProgressCallback(Storage::ProgressCallback callback)
{
  Q_D(Plugin);
  d->m_callback = callback;
  for (const auto &plugin : pPlugins.newstorage)
    plugin->setProgressCallback(callback);
}

void Plugin::setParent(QObject *parent)
{
  Q_D(Plugin);
  d->m_parent = parent;
  for (const auto &plugin : pPlugins.newstorage)
    plugin->setParent(parent);

  // this shouldn't be here, but is good for initializing storage plugins
  for (const auto &plugin : pPlugins.newstorage)
    plugin->setUiType(d->m_uiType);
}

void Plugin::openStorage(MyMoneyFile &storage, const QUrl &url)
{
  Q_D(Plugin);

  IUrl storageUrl(url);
  const auto pluginScheme = storageUrl.pluginScheme();

  auto isPluginFounded = false;
  if (!pluginScheme.isEmpty()) {
    for (const auto &plugin : pPlugins.newstorage) {
      if (plugin->scheme() == pluginScheme) {
        connect(plugin, &IStoragePlugin::storageOpened,
                this, &Plugin::slotStorageOpened, Qt::UniqueConnection);
        plugin->openStorage(storage, url);
        isPluginFounded = true;
      }
    }
  }

  if (!isPluginFounded) {
    const QVector<Issue> issues {{eIssue::MissingPlugin, QStringList{pluginScheme}}};
    d->warnOfMissingPlugins(issues);
  }
}

void Plugin::saveStorage(const MyMoneyFile &storage, const QUrl &url)
{
  Q_D(Plugin);

  IUrl storageUrl(url);
  const auto pluginScheme = storageUrl.pluginScheme();

  auto isPluginFounded = false;
  if (!pluginScheme.isEmpty()) {
    for (const auto &plugin : pPlugins.newstorage) {
      if (plugin->scheme() == pluginScheme) {
        connect(plugin, &IStoragePlugin::storageSaved,
                this, &Plugin::slotStorageSaved, Qt::UniqueConnection);
        plugin->saveStorage(storage, url);
        isPluginFounded = true;
      }
    }
  }

  if (!isPluginFounded) {
    const QVector<Issue> issues {{eIssue::MissingPlugin, QStringList{pluginScheme}}};
    d->warnOfMissingPlugins(issues);
  }
}

void Plugin::closeStorage(const QUrl &url)
{
  IUrl storageUrl(url);
  const auto pluginScheme = storageUrl.pluginScheme();

  if (!pluginScheme.isEmpty()) {
    for (const auto &plugin : pPlugins.newstorage) {
      if (plugin->scheme() == pluginScheme) {
        plugin->closeStorage(url);
      }
    }
  }
}

std::unique_ptr<MyMoneyFile> Plugin::storageMgr(const QUrl &url)
{
//  IUrl storageUrl(url);
//  const auto pluginScheme = storageUrl.pluginScheme();

//  for (const auto &plugin : pPlugins.newstorage) {
//    if (plugin->scheme() == pluginScheme) {
//      return plugin->storageManager();
//    }
//  }
  return nullptr;
}

void Plugin::openStorageDialog()
{
  Q_D(Plugin);

  d->readConfig();

  const auto& storagePlugins = d->storagePluginsMap();
  if (storagePlugins.isEmpty()) {
    const QVector<Issue> issues {{eIssue::MissingPlugins, QStringList()}};
    d->warnOfMissingPlugins(issues);
    return;
  }

  switch (d->m_uiType) {
    case eUI::Widget:
      d->m_dialogOpenSave = std::make_unique<Widget::OpenSave>(d->m_parent, storagePlugins, eAction::Open, QUrl());
      break;
#ifdef ENABLE_QUICK
    case eUI::Quick:
      d->m_dialogOpenSave = std::make_unique<Quick::OpenSave>(d->m_parent, storagePlugins, eAction::Open, QUrl());
      break;
#endif
    default:
      return;
  }

  auto slotUserAccepted = [=]() {
    emit openStorageRequested(d->m_dialogOpenSave->storageUrl());
  };

  auto slotUserRejected = [=]() {
    d->setPluginsParent(d->m_parent);
    d->m_dialogOpenSave.reset();
  };

  d->setPluginsParent(d->m_dialogOpenSave->uiPart());

  connect(d->m_dialogOpenSave.get(), &Manager::OpenSave::accepted, slotUserAccepted);
  connect(d->m_dialogOpenSave.get(), &Manager::OpenSave::rejected, slotUserRejected);
}

void Plugin::saveStorageDialog(const QUrl &presetUrl)
{
  Q_D(Plugin);

  d->readConfig();

  const auto& storagePlugins = d->storagePluginsMap();
  if (storagePlugins.isEmpty()) {
    const QVector<Issue> issues {{eIssue::MissingPlugins, QStringList()}};
    d->warnOfMissingPlugins(issues);
    return;
  }

  switch (d->m_uiType) {
    case eUI::Widget:
      d->m_dialogOpenSave = std::make_unique<Widget::OpenSave>(d->m_parent, storagePlugins, eAction::Save, presetUrl);
      break;
#ifdef ENABLE_QUICK
    case eUI::Quick:
      d->m_dialogOpenSave = std::make_unique<Quick::OpenSave>(d->m_parent, storagePlugins, eAction::Save, presetUrl);
      break;
#endif
    default:
      return;
  }

  auto slotUserAccepted = [=]() {
    emit saveStorageRequested(d->m_dialogOpenSave->storageUrl());
  };

  auto slotUserRejected = [=]() {
    d->setPluginsParent(d->m_parent);
    d->m_dialogOpenSave.reset();
  };

  d->setPluginsParent(d->m_dialogOpenSave->uiPart());

  connect(d->m_dialogOpenSave.get(), &Manager::OpenSave::accepted, slotUserAccepted);
  connect(d->m_dialogOpenSave.get(), &Manager::OpenSave::rejected, slotUserRejected);
}

void Plugin::slotStorageOpened(const QUrl &url)
{
  Q_D(Plugin);
  d->setPluginsParent(d->m_parent);
  d->m_dialogOpenSave.reset();
  emit storageOpened(url);
}

void Plugin::slotStorageSaved(const QUrl &url)
{
  Q_D(Plugin);
  d->setPluginsParent(d->m_parent);
  d->m_dialogOpenSave.reset();
  emit storageSaved(url);
}


K_PLUGIN_FACTORY_WITH_JSON(PluginFactory, "plugin.json", registerPlugin<Plugin>();)

} // namespace Manager
} // namespace Storage

#include "plugin.moc"
