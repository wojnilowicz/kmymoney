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

#ifndef STORAGE_SQL_FRONTEND_QUICK_OPENSAVESERVER_H
#define STORAGE_SQL_FRONTEND_QUICK_OPENSAVESERVER_H

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// QT Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "frontend/opensaveserver.h"

namespace Storage {
namespace Sql {
namespace Quick {

class OpenSaveServerPrivate;
class OpenSaveServer : public Sql::OpenSaveServer
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(OpenSaveServer)
  Q_PROPERTY(QString hostName READ hostName WRITE setHostName NOTIFY hostNameChanged)
  Q_PROPERTY(QString databaseName READ databaseName WRITE setDatabaseName NOTIFY databaseNameChanged)
  Q_PROPERTY(QString databaseNameTooltip READ databaseNameTooltip NOTIFY databaseNameTooltipChanged)

public:
  explicit OpenSaveServer(Backend *backend, eType storageType, eAction action);
  ~OpenSaveServer() override final;

  QObject *uiPart() override final;
  void setStorageUrl(const QUrl &storageUrl) override final;

  QString hostName() const;
  QString databaseName() const;
  QString databaseNameTooltip() const;

Q_SIGNALS:
    void hostNameChanged(const QString &hostName);
    void databaseNameChanged(const QString &databaseName);
    void databaseNameTooltipChanged(const QString &databaseNameTooltip);

protected Q_SLOTS:
    void setHostName(const QString &hostName) override final;
    void setDatabaseName(const QString &databaseName) override final;
    void setDatabaseNameTooltip(const QString &databaseNameTooltip) override final;
};

} // namespace Quick$
} // namespace Sql
} // namespace Storage

#endif
