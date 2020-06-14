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

#ifndef STORAGE_SQL_BACKEND_BACKEND_H
#define STORAGE_SQL_BACKEND_BACKEND_H

#include <memory>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// QT Includes

#include <QtGlobal>

// ----------------------------------------------------------------------------
// Project Includes

#include "icallback.h"

class MyMoneyFile;
class QUrl;

namespace Storage {

struct Issue;
enum class eProducer;
enum class eType;

namespace Sql {

class BackendPrivate;
class Backend
{
  Q_DECLARE_PRIVATE(Backend)

public:
  Backend();
  ~Backend();

  void setProgressCallback(ProgressCallback callback);

  void openStorage(MyMoneyFile &storage, const QUrl &url);
  void saveStorage(const MyMoneyFile &storage, const QUrl &url);
  void upgradeStorage(const QUrl &url);
  QUrl convertStorage(const QUrl &url, eProducer targetProducer);

  eType storageType(const QUrl &url);
  QVector<eType> types() const;
  QVector<eType> canOpenTypes() const;
  QVector<eType> canSaveTypes() const;

  bool isKMyMoneyType(eType type) const;
  bool isStorageFileBased(const QUrl &url);

  QString typeDisplayedName(eType storageType) const;
  QString typeInternalName(eType storageType) const;

  QVector<Issue> validateForOpen(const QUrl &url);
  QVector<Issue> validateForSave(const QUrl &url);

  bool createDatabase(const QUrl &url);
  void closeConnection(const QUrl &url);

private:
  const std::unique_ptr<BackendPrivate> d_ptr;
  friend class BackendTest;
};

} // namespace Sql
} // namespace Storage

#endif
