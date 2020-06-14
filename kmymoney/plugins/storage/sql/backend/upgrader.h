/*
 * Copyright 2020       Łukasz Wojniłowicz <lukasz.wojnilowicz@gmail.com>
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

#ifndef STORAGE_SQL_BACKEND_UPGRADER_H
#define STORAGE_SQL_BACKEND_UPGRADER_H

#include <memory>

// ----------------------------------------------------------------------------
// QT Includes

#include <QtGlobal>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "storage/interface/icallback.h"

template <typename T>  class QVector;
class QSqlDatabase;

namespace Storage {

enum class eProducer;

namespace Sql {

class UpgraderPrivate;
class Upgrader
{
  Q_DISABLE_COPY(Upgrader)
  Q_DECLARE_PRIVATE(Upgrader)

public:
  explicit Upgrader(ProgressCallback callback = nullptr);
  ~Upgrader();

  static QVector<unsigned int> supportedVersions(eProducer storageProducer);
  bool upgradeStorage(QSqlDatabase &db, eProducer storageProducer, unsigned int sourceVersion, unsigned int targetVersion = 0);

private:
  const std::unique_ptr<UpgraderPrivate> d_ptr;
};

} // namespace Sql
} // namespace Storage

#endif
