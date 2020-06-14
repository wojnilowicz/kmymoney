/*
 * Copyright 2019       Łukasz Wojniłowicz <lukasz.wojnilowicz@gmail.com>
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

#ifndef STORAGE_INTERFACE_IPLUGIN_H
#define STORAGE_INTERFACE_IPLUGIN_H

#include <kmm_istorage_export.h>

#include <memory>

// ----------------------------------------------------------------------------
// QT Includes

#include <QtPlugin>
#include <QHashFunctions>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "kmymoneyplugin.h"
#include "icallback.h"

template <typename T>  class QVector;

class MyMoneyFile;

namespace Storage {

enum class eUI;
enum class eType;
enum class eVersionStatus;

class IDialogPartOpenSave;

class KMM_ISTORAGE_EXPORT IStorageManager :
    public KMyMoneyPlugin::Plugin
{
  Q_OBJECT

public:
  IStorageManager();
  virtual ~IStorageManager() = 0;

  virtual void setProgressCallback(ProgressCallback callback) = 0;
  virtual void setParent(QObject *parent) = 0;

  virtual void openStorage(MyMoneyFile &storage, const QUrl &url) = 0;
  virtual void saveStorage(const MyMoneyFile &storage, const QUrl &url) = 0;
  virtual void closeStorage(const QUrl &url) = 0;

  virtual void openStorageDialog() = 0;
  virtual void saveStorageDialog(const QUrl &presetUrl) = 0;

  virtual std::unique_ptr<MyMoneyFile> storageMgr(const QUrl &url) = 0;

Q_SIGNALS:
  void storageOpened(const QUrl &url);
  void storageSaved(const QUrl &url);
  void openStorageRequested(const QUrl &url);
  void saveStorageRequested(const QUrl &url);
};

class KMM_ISTORAGE_EXPORT IStoragePlugin :
    public KMyMoneyPlugin::Plugin
{
  Q_OBJECT

public:
  IStoragePlugin();
  virtual ~IStoragePlugin() = 0;

  virtual void setProgressCallback(ProgressCallback callback) = 0;
  virtual void setParent(QObject *parent) = 0;
  virtual void setUiType(eUI type) = 0;

  virtual QVector<eType> types() const = 0;
  virtual QString typeDisplayedName(eType storageType) const = 0;
  virtual QString typeInternalName(eType storageType) const = 0;

  virtual bool canOpen(eType storageType) const = 0;
  virtual bool canSave(eType storageType) const = 0;

  virtual QString scheme() const = 0;

  virtual IDialogPartOpenSave *openDialogPart(eType storageType) = 0;
  virtual IDialogPartOpenSave *saveAsDialogPart(eType storageType) = 0;

  virtual void openStorage(MyMoneyFile &storage, const QUrl &url) = 0;
  virtual void saveStorage(const MyMoneyFile &storage, const QUrl &url) = 0;
  virtual void closeStorage(const QUrl &url) = 0;

  virtual eType storageType(const QUrl &url) = 0;

  virtual std::unique_ptr<MyMoneyFile> storageManager() = 0;

Q_SIGNALS:
  void storageOpened(const QUrl &url);
  void storageSaved(const QUrl &url);
};

} // namespace Storage

Q_DECLARE_INTERFACE(Storage::IStorageManager, "org.kmymoney.plugin.storage.imanagerplugin")
Q_DECLARE_INTERFACE(Storage::IStoragePlugin, "org.kmymoney.plugin.storage.istorageplugin")

#endif
