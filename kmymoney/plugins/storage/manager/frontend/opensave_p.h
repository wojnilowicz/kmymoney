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

#ifndef STORAGE_MANAGER_FRONTEND_OPENSAVE_P_H
#define STORAGE_MANAGER_FRONTEND_OPENSAVE_P_H

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// QT Includes

#include <QMap>
#include <QVector>

// ----------------------------------------------------------------------------
// Project Includes

#include "storage/interface/enums.h"
#include "storage/interface/iplugin.h"
#include "storage/interface/iurl.h"

namespace Storage {
  typedef QMap<QString, IStoragePlugin *>    StoragePluginsMap;
  typedef QMap<eType, IDialogPartOpenSave *> DialogPartsMap;

namespace Manager {
  
class OpenSavePrivate
{
public:
  OpenSavePrivate(eUI uiType, eAction actionType, const StoragePluginsMap & storagePlugins);
  virtual ~OpenSavePrivate() = 0;

  QVector<eType> availableStorageTypes() const;
  QVector<eType> filterVisibleStorageTypes(const QVector<eType> &unfilteredTypes) const;
  QVector<eType> filterOpeanableStorageTypes(const QVector<eType> &unfilteredTypes) const;
  QVector<eType> filterSaveableStorageTypes(const QVector<eType> &unfilteredTypes) const;
  QVector<eType> storageTypesForOpen() const;
  QVector<eType> storageTypesForSave() const;

  QStringList storageTypeNames(const QVector<eType> &types) const;
  int fallbackStorageTypeIndex(const QVector<eType> &types) const;
  int presetStorageTypeIndex(const QUrl &url, const QVector<eType> &types) const;

  eType storageTypeByUrl(const QUrl &storageUrl) const;

  eType storageTypeFromDisplayedName(const QString &displayedName) const;
  eType storageTypeFromInternalName(const QString &internalName) const;

  IDialogPartOpenSave *dialogPart(eType storageType);

  const eUI         m_uiType;
  const eAction     m_actionType;
  const StoragePluginsMap m_storagePlugins;

  DialogPartsMap    m_cachedDialogParts;

  eType             m_storageType = eType::Unknown;
};

} // namespace Manager
} // namespace Storage

#endif
