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

#include <memory>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// QT Includes

#include <QtGlobal>
#include <QPair>

// ----------------------------------------------------------------------------
// Project Includes

#ifndef STORAGE_SQL_COMPARATOR_H
#define STORAGE_SQL_COMPARATOR_H

class QString;
class QStringList;
class QSqlDatabase;

namespace Storage {
namespace Sql {

class ComparatorPrivate;
class Comparator {
  Q_DECLARE_PRIVATE(Comparator)

public:
  Comparator();
  ~Comparator();

  static QString addSuffixToFileName(const QString &filePath, const QString &suffixToAdd);
  static QString changeFileExtension(const QString &filePath, const QString &newExtension);

  void saveDiffsToFile(const QString &firstFilePath, const QString &secondFilePath) const;
  bool areStoragesEqual(const QSqlDatabase &firstDb, const QSqlDatabase &secondDb);  

private:
  const std::unique_ptr<ComparatorPrivate> d_ptr;
};

} // namespace Sql
} // namespace Storage

#endif
