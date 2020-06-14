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

#ifndef STORAGE_SQL_POSTGRESSQL_BACKEND_H
#define STORAGE_SQL_POSTGRESSQL_BACKEND_H

// ----------------------------------------------------------------------------
// QT Includes

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "interface/ibackend.h"

class QUrl;
class QSqlQuery;
class QString;

namespace Storage {
namespace Sql {
namespace PostgresSql {

class Backend : public IBackend
{
public:
  Backend();
  ~Backend() override;

  bool createDatabase(const QUrl &url);
  bool selectDatabase(const QUrl &url);
  bool databaseExists(const QUrl &url);

  bool isServerRunning(const QUrl &url) const;

};

} // namespace PostgresSql
} // namespace Sql
} // namespace Storage

#endif
