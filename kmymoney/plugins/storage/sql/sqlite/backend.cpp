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

#include "backend.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QFileInfo>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "backend/url.h"
#include "backend/names.h"
#include "storage/interface/enums.h"

namespace Storage {
namespace Sql {
namespace SQLite {

Backend::Backend() = default;
Backend::~Backend() = default;

void Backend::setConnectionOptions()
{
  db->exec(QString::fromLatin1("PRAGMA foreign_keys = ON"));
}

bool Backend::magicHeaderMatches(const QUrl &url) const
{
  const auto localFileName = url.path();
  if (localFileName.isEmpty())
    return false;

  QFile fileHandle(localFileName);
  if (!fileHandle.open(QIODevice::ReadOnly))
    return false;

  const auto bytesToInferType = 16;
  const auto fileHead = fileHandle.read(bytesToInferType);
  if (!fileHead.startsWith("SQLite format 3\000"))
    return false;

  return true;
}

void Backend::dropColumn(const QString &tableName, const QString &columnName)
{
  enum columnInfoEnum {
    cid,
    name,
    type,
    notnull,
    dflt_value,
    pk,
    LastColumn
  };

  const QMap<columnInfoEnum, QString> columnNamesMap {
    {columnInfoEnum::cid, QStringLiteral("cid")},
    {columnInfoEnum::name, QStringLiteral("name")},
    {columnInfoEnum::type, QStringLiteral("type")},
    {columnInfoEnum::notnull, QStringLiteral("notnull")},
    {columnInfoEnum::dflt_value, QStringLiteral("dflt_value")},
    {columnInfoEnum::pk, QStringLiteral("pk")},
  };

  QVector<QMap<columnInfoEnum, QString>> allColumnValues;
  auto queryString = QString::fromLatin1("PRAGMA table_info(%1);").arg(tableName);
  QSqlQuery query(*db);
  query.exec(queryString);
  while(query.next()) {
    QMap<columnInfoEnum, QString> columnValues;
    for (int i = columnInfoEnum::cid; i < columnInfoEnum::LastColumn; ++i) {
      const auto columnEnum = static_cast<columnInfoEnum>(i);
      const auto columnName = columnNamesMap.value(columnEnum);
      const auto columnValue = query.value(columnName).toString();
      columnValues.insert(columnEnum, columnValue);
    }
    allColumnValues.append(columnValues);
  }

  QStringList primaryColumnsList;
  QStringList columnDefinitions;
  QStringList columnNamesList;
  for (const auto &columnValues : allColumnValues) {
    if (columnValues.value(columnInfoEnum::name) == columnName)
      continue;
    columnNamesList.append(columnValues.value(columnInfoEnum::name));
    auto variableString = QString::fromLatin1("%1 %2").
                          arg(columnValues.value(columnInfoEnum::name)).
                          arg(columnValues.value(columnInfoEnum::type));

    if (columnValues.value(columnInfoEnum::notnull) == "1")
      variableString.append(QStringLiteral(" NOT NULL"));

    if (!columnValues.value(columnInfoEnum::dflt_value).isEmpty())
      variableString.append(QString::fromLatin1(" DEFAULT '%1'").arg(columnValues.value(columnInfoEnum::dflt_value)));

    if (columnValues.value(columnInfoEnum::pk) == "1")
      primaryColumnsList.append(columnValues.value(columnInfoEnum::name));
    columnDefinitions.append(variableString);
  }

  QString primaryColumnsString;
  if (!primaryColumnsList.isEmpty())
    primaryColumnsString = QString::fromLatin1(", PRIMARY KEY (%1)").arg(primaryColumnsList.join(", "));
  auto columnsDefinitionsString = columnDefinitions.join(", ");

  // there are no foreign keys in KMM database, so this should be redundant
  queryString = QString::fromLatin1("PRAGMA foreign_keys=OFF;");
  query.exec(queryString);

  // this prevents kmmBalances corruption
  queryString = QString::fromLatin1("PRAGMA legacy_alter_table=ON;");
  query.exec(queryString);

  const auto tempTableName = QString::fromLatin1("%1_temp").arg(tableName);
  queryString = QString::fromLatin1("CREATE TABLE %1 (%2%3);").
                arg(tempTableName).
                arg(columnsDefinitionsString).
                arg(primaryColumnsString);
  query.exec(queryString);

  queryString = QString::fromLatin1("INSERT INTO %1 SELECT %2 FROM %3;").
                arg(tempTableName).
                arg(columnNamesList.join(',')).
                arg(tableName);
  query.exec(queryString);

  queryString = QString::fromLatin1("DROP TABLE %1;").
                arg(tableName);
  if (!query.exec(queryString))
    qDebug() << "Cannot drop table: " << query.lastError();

  queryString = QString::fromLatin1("ALTER TABLE %1 RENAME TO %2;").
                arg(tempTableName).
                arg(tableName);
  if (!query.exec(queryString))
    qDebug() << "Cannot rename table: " << query.lastError();

  queryString = QString::fromLatin1("PRAGMA foreign_keys=ON;");
  query.exec(queryString);

  queryString = QString::fromLatin1("PRAGMA legacy_alter_table=OFF;");
  query.exec(queryString);
}

} // namespace SQLite
} // namespace Sql
} // namespace Storage
