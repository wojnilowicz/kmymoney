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

#ifndef STORAGE_SQL_FRONTEND_WIDGET_OPENSAVESERVER_H
#define STORAGE_SQL_FRONTEND_WIDGET_OPENSAVESERVER_H

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// QT Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "frontend/opensaveserver.h"

namespace Storage {
namespace Sql {
namespace Widget {

class OpenSaveServerPrivate;
class OpenSaveServer : public Sql::OpenSaveServer
{
  Q_DECLARE_PRIVATE(OpenSaveServer)

public:
  explicit OpenSaveServer(Backend *backend, eType storageType, eAction action);
  ~OpenSaveServer() override final;

  QObject *uiPart() override final;
  void setStorageUrl(const QUrl &storageUrl) override final;
  void setDatabaseNameTooltip(const QString &databaseNameTooltip) override final;
};

} // namespace Widget
} // namespace Sql
} // namespace Storage

#endif
