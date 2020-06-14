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

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "mymoney/mymoneyexception.h"

#include "backend/url.h"
#include "storage/interface/enums.h"

namespace Storage {
namespace Sql {
namespace PostgresSql {

Backend::Backend() = default;
Backend::~Backend() = default;

bool Backend::createDatabase(const QUrl &url)
{
  const QString queryTemplateString = QStringLiteral("CREATE DATABASE %1;");
  Url enhancedUrl(url);
  const auto databaseName = enhancedUrl.databaseName();
  const auto queryString = queryTemplateString.arg(databaseName);
  auto query = db->exec(queryString);
  if (query.isActive()) {
    query.finish();
    return true;
  } else {
    throw MYMONEYEXCEPTION(query.lastError().text());
  }
  return false;
}

bool Backend::selectDatabase(const QUrl &url)
{
  Url enhancedUrl(url);
  const auto databaseName = enhancedUrl.databaseName();
  db->close();
  db->setDatabaseName(databaseName);
  if (db->open())
    return true;
  return false;
}

bool Backend::databaseExists(const QUrl &url)
{
  const QString queryTemplateString = QStringLiteral("SELECT 1 FROM pg_database WHERE datname='%1';");
  Url enhancedUrl(url);
  const auto databaseName = enhancedUrl.databaseName();
  const auto queryString = queryTemplateString.arg(databaseName);
  auto query = db->exec(queryString);
  if (query.isActive() && query.first()) {
    query.finish();
    return true;
  }
  return false;
}

bool Backend::isServerRunning(const QUrl &url) const
{
  auto lastError = db->lastError();
  if (lastError.databaseText().contains(QString::fromLatin1("%1").arg(url.host())) &&
      !lastError.databaseText().contains("fe_sendauth")) { // all conditions are heurustic
    return false;
  }
  return true;
}

} // namespace PostgresSql
} // namespace Sql
} // namespace Storage
