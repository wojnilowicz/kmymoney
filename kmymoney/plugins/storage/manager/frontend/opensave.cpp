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
#include "opensave_p.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QUrl>
#include <QDebug>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "storage/interface/idialogpartopensave.h"
#include "storage/interface/enums.h"
#include "configuration.h"

namespace Storage {
namespace Manager {

OpenSavePrivate::OpenSavePrivate(eUI uiType, eAction actionType, const StoragePluginsMap &storagePlugins) :
  m_uiType(uiType),
  m_actionType(actionType),
  m_storagePlugins(storagePlugins)
{
};

OpenSavePrivate::~OpenSavePrivate()
{
  for (auto &cachedDialog : m_cachedDialogParts)
    delete cachedDialog;
}

QVector<eType> OpenSavePrivate::availableStorageTypes() const
{
  QVector<eType> types;
  for (const auto &plugin : m_storagePlugins)
    for (auto type : plugin->types())
      types.append(type);
  return types;
}

QVector<eType> OpenSavePrivate::filterVisibleStorageTypes(const QVector<eType> &unfilteredTypes) const
{
  QVector<eType> types;

  const auto visibleStorageTypes = Configuration::self()->visibleStorageTypes();
  if (visibleStorageTypes.isEmpty()) {
    types = unfilteredTypes;
    return types;
  }

  for (const auto &plugin : m_storagePlugins)
    for (auto type : plugin->types())
      if (unfilteredTypes.contains(type))
        if (visibleStorageTypes.contains(plugin->typeInternalName(type)))
          types.append(type);

  if (types.isEmpty())
    types = unfilteredTypes;

  return types;
}

QVector<eType> OpenSavePrivate::filterOpeanableStorageTypes(const QVector<eType> &unfilteredTypes) const
{
  QVector<eType> types;
  for (const auto &plugin : m_storagePlugins)
    for (auto type : plugin->types())
      if (unfilteredTypes.contains(type))
        if (plugin->canOpen(type))
          types.append(type);
  return types;
}

QVector<eType> OpenSavePrivate::filterSaveableStorageTypes(const QVector<eType> &unfilteredTypes) const
{
  QVector<eType> types;
  for (const auto &plugin : m_storagePlugins)
    for (auto type : plugin->types())
      if (unfilteredTypes.contains(type))
        if (plugin->canSave(type))
          types.append(type);
  return types;
}

QVector<eType> OpenSavePrivate::storageTypesForOpen() const
{
  auto types = availableStorageTypes();
  types = filterOpeanableStorageTypes(types);
  types = filterVisibleStorageTypes(types);
  return types;
}

QVector<eType> OpenSavePrivate::storageTypesForSave() const
{
  auto types = availableStorageTypes();
  types = filterSaveableStorageTypes(types);
  types = filterVisibleStorageTypes(types);
  return types;
}

QStringList OpenSavePrivate::storageTypeNames(const QVector<eType> &types) const
{
  QStringList names;
  for (const auto &plugin : m_storagePlugins)
    for (auto type : plugin->types())
      if (types.contains(type))
          names.append(plugin->typeDisplayedName(type));
  return names;
}

int OpenSavePrivate::fallbackStorageTypeIndex(const QVector<eType> &types) const
{
  const QVector<eType> fallbackCandidates { eType::kmmn_XmlGzip, eType::kmmn_SQLite };
  auto index = -1;

  for (auto type : fallbackCandidates) {
    index = types.indexOf(type);
    if (index != -1) {
      break;
    }
  }

  if (index == -1 && !types.isEmpty())
    index = 0;

  return index;
}

int OpenSavePrivate::presetStorageTypeIndex(const QUrl &url, const QVector<eType> &types) const
{
  auto index = 0;
  auto presetType = storageTypeByUrl(url);
  index = types.indexOf(presetType);
  return index;
}

eType OpenSavePrivate::storageTypeByUrl(const QUrl &storageUrl) const
{
  IUrl enhancedStorageUrl(storageUrl);
  const auto storageTypeFromUrl = enhancedStorageUrl.storageType();
  return storageTypeFromInternalName(storageTypeFromUrl);
}

eType OpenSavePrivate::storageTypeFromDisplayedName(const QString &displayedName) const
{
  auto storageType = eType::Unknown;
  for (const auto &storagePlugin : m_storagePlugins)
    for (const auto &type : storagePlugin->types())
      if (displayedName == storagePlugin->typeDisplayedName(type))
        storageType = type;
  return storageType;
}

eType OpenSavePrivate::storageTypeFromInternalName(const QString &internalName) const
{
  auto storageType = eType::Unknown;
  for (const auto &plugin : m_storagePlugins)
    for (const auto &type : plugin->types())
      if (internalName == plugin->typeInternalName(type))
        storageType = type;
  return storageType;
}

IDialogPartOpenSave *OpenSavePrivate::dialogPart(eType storageType)
{
  // reuse already instantiated dialog parts to restore dialog fields half filled by the user
  if (!m_cachedDialogParts.value(storageType)) {
    for (const auto &storagePlugin : m_storagePlugins) {
      if (storagePlugin->types().contains(storageType)) {
        switch (m_actionType) {
          case eAction::Open:
            m_cachedDialogParts.insert(storageType, storagePlugin->openDialogPart(storageType));
            break;
          case eAction::Save:
            m_cachedDialogParts.insert(storageType, storagePlugin->saveAsDialogPart(storageType));
            break;
          default:
            break;
        }
        break;
      }
    }
  }

  return m_cachedDialogParts.value(storageType, nullptr);
}


OpenSave::OpenSave(OpenSavePrivate &d) :
d_ptr(&d)
{
}

OpenSave::~OpenSave() = default;

QUrl OpenSave::storageUrl()
{
  Q_D(OpenSave);
  const auto &currentDialogPart = d->dialogPart(d->m_storageType);
  return currentDialogPart->storageUrl();
}

void OpenSave::slotStorageTypeChanged(const QString &storageType)
{
  Q_D(OpenSave);
  d->m_storageType = d->storageTypeFromDisplayedName(storageType);
  if (auto cachedDialogPart = d->dialogPart(d->m_storageType)) {
    connect(cachedDialogPart, &IDialogPartOpenSave::userBasedValidityChanged,
            this, &Manager::OpenSave::userBasedValidityChanged, Qt::UniqueConnection);

    // check user's selection validity manually because...
    // ...simply replacing widget in the layout won't trigger that
    cachedDialogPart->validateUserSelections();
  }
}

} // namespace Manager
} // namespace Storage
