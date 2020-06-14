/*
 * Copyright 2017-2018  Łukasz Wojniłowicz <lukasz.wojnilowicz@gmail.com>
 * Copyright 2019       Thomas Baumgart <tbaumgart@kde.org>
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

#include "names.h"

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// QT Includes

#include <QMap>

// ----------------------------------------------------------------------------
// Project Includes

namespace Storage {
namespace Sql {

QString tableName(Table tableID)
{
  static const QHash<Table, QString> tableNames {
    {Table::Accounts, QStringLiteral("kmmAccounts")},
    {Table::FileInfo, QStringLiteral("kmmFileInfo")}

  };
  return tableNames.value(tableID);
}

uint qHash(const Table key, uint seed) { return ::qHash(static_cast<uint>(key), seed); }

namespace Column {
uint qHash(const FileInfo key, uint seed) { return ::qHash(static_cast<uint>(key), seed); }
}

QString columnName(Column::FileInfo columnID)
{
  static const QHash<Column::FileInfo, QString> columnNames {
    {Column::FileInfo::Version,   QStringLiteral("version")},
    {Column::FileInfo::LogonUser, QStringLiteral("logonUser")},
    {Column::FileInfo::LogonAt,   QStringLiteral("logonAt")},
    {Column::FileInfo::FixLevel,  QStringLiteral("fixLevel")}
  };
  return columnNames.value(columnID);
}

} // namespace Sql
} // namespace Storage
