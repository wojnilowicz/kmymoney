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

#include "opensave.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QUrl>
#include <QDebug>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <KLocalizedString>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "frontend/opensave_p.h"
#include "storage/interface/idialogpartopensave.h"
#include "storage/interface/enums.h"

namespace Storage {
namespace Manager {
namespace Quick {

class OpenSavePrivate : public Manager::OpenSavePrivate
{
public:
  OpenSavePrivate(QObject *parent, eAction actionType, const StoragePluginsMap &storagePlugins);
  ~OpenSavePrivate() override final = default;

  const QScopedPointer<QQmlApplicationEngine, QScopedPointerDeleteLater> m_engine;
};

OpenSavePrivate::OpenSavePrivate(QObject *parent, eAction actionType, const StoragePluginsMap &storagePlugins) :
  Manager::OpenSavePrivate(eUI::Quick, actionType, storagePlugins),
  m_engine(new QQmlApplicationEngine(parent))
{
}

OpenSave::OpenSave(QObject *parent,
                   const StoragePluginsMap &storagePlugins,
                   eAction actionType,
                   const QUrl &presetUrl) :
  Manager::OpenSave(*new OpenSavePrivate(parent, actionType, storagePlugins))
{
  Q_D(OpenSave);

  auto ctx = d->m_engine->rootContext();
  ctx->setContextProperty("backend", this);

  const QMap<eAction, QString> windowTitles {
    {eAction::Open, i18n("Select file for opening")},
    {eAction::Save, i18n("Select file for saving")}
  };
  ctx->setContextProperty("dialogTitle", windowTitles.value(d->m_actionType));

  const auto storageTypes = actionType == eAction::Open ? d->storageTypesForOpen() : d->storageTypesForSave();
  const auto storageTypeNames = d->storageTypeNames(storageTypes);
  const auto presetStorageTypeIndex = d->presetStorageTypeIndex(presetUrl, storageTypes);
  const auto storageTypeIndex = presetStorageTypeIndex != -1 ? presetStorageTypeIndex : d->fallbackStorageTypeIndex(storageTypes);

  ctx->setContextProperty("storageTypesList", storageTypeNames);
  ctx->setContextProperty("defaultTypeIndex", storageTypeIndex);

  d->m_engine->load(QUrl("qrc:/storage/manager/opensave.qml"));

  if (presetStorageTypeIndex != -1)
    if (auto cachedDialogPart = d->dialogPart(d->m_storageType))
      cachedDialogPart->setStorageUrl(presetUrl);
}

OpenSave::~OpenSave() = default;

QObject *OpenSave::uiPart() const
{
  Q_D(const OpenSave);
  return d->m_engine.get();
}

QObject *OpenSave::dialogPartUIPart()
{
  Q_D(OpenSave);
  if (auto dialogPart = d->dialogPart(d->m_storageType))
    return dialogPart->uiPart();
  return nullptr;
}

} // namespace Quick$
} // namespace Manager
} // namespace Storage
