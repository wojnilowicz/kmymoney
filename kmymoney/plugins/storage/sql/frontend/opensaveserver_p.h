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

#ifndef STORAGE_SQL_FRONTEND_OPENSAVESERVER_P_H
#define STORAGE_SQL_FRONTEND_OPENSAVESERVER_P_H

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// QT Includes

#include <QString>

// ----------------------------------------------------------------------------
// Project Includes

#include "storage/interface/enums.h"

namespace Storage {
namespace Sql {
class Backend;
  
class OpenSaveServerPrivate
{
public:
  virtual ~OpenSaveServerPrivate() = 0;

  QString validateDatabaseName() const;

  Backend *m_backend = nullptr;
  eType m_storageType = eType::Unknown;
  QString m_hostName;
  QString m_databaseName;
  QString m_databaseNameTooltip;
  eAction m_action = eAction::Unknown;
};

} // namespace Sql
} // namespace Storage

#endif
