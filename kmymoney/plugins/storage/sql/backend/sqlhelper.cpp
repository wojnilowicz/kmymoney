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

#include "sqlhelper.h"
#include <unistd.h>

// ----------------------------------------------------------------------------
// QT Includes

#include <QMap>
#include <QVariant>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "names.h"
#include "sqlcipher/backend.h"
#include "sqlite/backend.h"

namespace Storage {
namespace Sql {
namespace SqlHelper {

void setVersion(QSqlDatabase &db, unsigned int version)
{
  QSqlQuery query(db);
  auto queryString = QString::fromLatin1("UPDATE %1 SET %2 = %3;").
                     arg(tableName(Table::FileInfo)).
                     arg(columnName(Column::FileInfo::Version)).
                     arg(version);
  query.exec(queryString);
  query.finish();
}

void dropColumn(QSqlDatabase &db, const QString &tableName, const QString &columnName)
{
  const auto driverName = db.driverName();
  if (driverName == "QSQLITE") {
    SQLite::Backend sqlite;
    sqlite.db = &db;
    sqlite.dropColumn(tableName, columnName);
  } else if (driverName == "QSQLCIPHER") {
    SQLCipher::Backend sqlcipher;
    sqlcipher.db = &db;
    sqlcipher.dropColumn(tableName, columnName);
  } else {
    QSqlQuery query(db);
    auto queryString = QString::fromLatin1("ALTER TABLE %1 DROP COLUMN %2;").
                  arg(tableName).arg(columnName);
    query.exec(queryString);
  }

}

} // namespace SqlHelper
} // namespace Sql
} // namespace Storage
