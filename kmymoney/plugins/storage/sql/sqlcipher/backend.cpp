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

#include "backend/url.h"
#include "storage/interface/enums.h"

namespace Storage {
namespace Sql {
namespace SQLCipher {

Backend::Backend() = default;
Backend::~Backend() = default;

bool Backend::unlockDatabase(const QUrl &url)
{
  if (url.password().isEmpty())
    return false;

  auto queryString = QString::fromLatin1("PRAGMA key = '%1';").arg(url.password());
  auto query = db->exec(queryString);
  query.finish();

  // Choose compatibility over migration
  // For details see https://discuss.zetetic.net/t/upgrading-to-sqlcipher-4/3283
  // This query cannot be squashed with "PRAGMA key" query
  queryString = QString::fromLatin1("PRAGMA cipher_compatibility = 3;");
  query = db->exec(queryString);
  query.finish();

  queryString = QString::fromLatin1("SELECT COUNT(*) FROM sqlite_master;");
  query = db->exec(queryString);
  if (query.isActive()) {
    query.finish();
    return true;
  }
  return false;
}


} // namespace SQLCipher
} // namespace Sql
} // namespace Storage
