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

#ifndef STORAGE_MANAGER_PLUGIN_H
#define STORAGE_MANAGER_PLUGIN_H

#include <memory>

// ----------------------------------------------------------------------------
// QT Includes

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "storage/interface/iplugin.h"

namespace Storage {
namespace Manager {

class PluginPrivate;
class Plugin : public IStorageManager
{
  Q_OBJECT
  Q_INTERFACES(Storage::IStorageManager)
  Q_DECLARE_PRIVATE(Plugin)

public:
  explicit Plugin(QObject *parent, const QVariantList &args);
  ~Plugin() override final;

  void setProgressCallback(ProgressCallback callback) override final;
  void setParent(QObject *parent) override final;

  void openStorage(MyMoneyFile &storage, const QUrl &url) override final;
  void saveStorage(const MyMoneyFile &storage, const QUrl &url) override final;
  void closeStorage(const QUrl &url) override final;

  void openStorageDialog() override final;
  void saveStorageDialog(const QUrl &presetUrl) override final;

  std::unique_ptr<MyMoneyFile> storageMgr(const QUrl &url) override;

private:
  const std::unique_ptr<PluginPrivate> d_ptr;

private Q_SLOTS:
  void slotStorageOpened(const QUrl &url);
  void slotStorageSaved(const QUrl &url);
};

} // namespace Manager
} // namespace Storage

#endif
