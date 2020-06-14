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

#ifndef STORAGE_SQL_PLUGIN_H
#define STORAGE_SQL_PLUGIN_H

#include <memory>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// QT Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "storage/interface/iplugin.h"

namespace Storage {
namespace Sql {

class PluginPrivate;
class Plugin : public IStoragePlugin
{
  Q_OBJECT
  Q_INTERFACES(Storage::IStoragePlugin)
  Q_DECLARE_PRIVATE(Plugin)

public:
  explicit Plugin(QObject *parent, const QVariantList &args);
  ~Plugin() override final;

  void setProgressCallback(ProgressCallback callback) override final;
  void setParent(QObject *parent) override final;
  void setUiType(eUI type) override final;

  QVector<eType> types() const override final;
  QString typeDisplayedName(eType storageType) const override final;
  QString typeInternalName(eType storageType) const override final;

  bool canOpen(eType storageType) const override final;
  bool canSave(eType storageType) const override final;

  QString scheme() const override final;

  void openStorage(MyMoneyFile &storage, const QUrl &url) override final;
  void saveStorage(const MyMoneyFile &storage, const QUrl &url) override final;
  void closeStorage(const QUrl &url) override final;

  eType storageType(const QUrl &url) override final;

  IDialogPartOpenSave *openDialogPart(eType storageType) override final;
  IDialogPartOpenSave *saveAsDialogPart(eType storageType) override final;

  std::unique_ptr<MyMoneyFile> storageManager() override final;

private:
  const std::unique_ptr<PluginPrivate> d_ptr;
};

} // namespace Sql
} // namespace Storage

#endif
