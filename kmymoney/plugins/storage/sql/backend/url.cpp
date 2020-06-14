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

#include "url.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QMap>
#include <QUrlQuery>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "storage/interface/enums.h"

namespace Storage {
namespace Sql {

static const QString dbName = QStringLiteral("dbname");

static const QMap <eType, QString> typeValues {
  {eType::kmm_MySql,          QStringLiteral("kmm-mysql")},
  {eType::kmm_PostgreSql,     QStringLiteral("kmm-postgres")},
  {eType::kmm_SQLite,         QStringLiteral("kmm-sqlite")},
  {eType::kmm_SqlCipher,      QStringLiteral("kmm-sqlcipher")},
  {eType::kmmn_MySql,         QStringLiteral("kmmn-mysql")},
  {eType::kmmn_PostgreSql,    QStringLiteral("kmmn-postgres")},
  {eType::kmmn_SQLite,        QStringLiteral("kmmn-sqlite")},
  {eType::kmmn_SqlCipher,     QStringLiteral("kmmn-sqlcipher")},
};

static const QString schemeString = QStringLiteral("sql");

Url::Url(const QUrl &url) :
  IUrl(url)
{
}

QString Url::pluginScheme()
{
  return schemeString;
}

void Url::setPluginScheme()
{
  IUrl::setPluginScheme(schemeString);
}

eType Url::decodeStorageType() const
{
  const auto query = QUrlQuery(*this);
  const auto typeInternalName = query.queryItemValue(keyName(eUrl::Type));
  if (typeInternalName.isEmpty())
    return eType::Unknown;
  return typeValues.key(typeInternalName, eType::Unknown);
}

void Url::encodeStorageType(eType type)
{
  auto query = QUrlQuery(*this);
  auto typeName = typeValues.value(type);
  if (typeName.isEmpty())
    return;
  query.removeQueryItem(keyName(eUrl::Type));
  query.addQueryItem(keyName(eUrl::Type), typeName);
  this->setQuery(query);
}

QString Url::databaseName() const
{
  auto databaseName = path();
  if (databaseName.startsWith('/') && databaseName.count('/') == 1 && !databaseName.contains('.'))
    return databaseName.mid(1);
  return databaseName;
}

void Url::setDatabaseName(const QString &name)
{
  if (name.contains('/') || name.contains('\\'))
    setPath(name);
  else
    setPath(QString::fromLatin1("/%1").arg(name));
}

QString Url::typeInternalName(eType storageType)
{
  return typeValues.value(storageType);
}

} // namespace Sql
} // namespace Storage
