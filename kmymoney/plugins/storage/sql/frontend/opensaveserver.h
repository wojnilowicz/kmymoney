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

#ifndef STORAGE_SQL_FRONTEND_OPENSAVESERVER_H
#define STORAGE_SQL_FRONTEND_OPENSAVESERVER_H

#include <memory>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// QT Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "storage/interface/idialogpartopensave.h"

namespace Storage {
enum class eType;
enum class eAction;

namespace Sql {
class Backend;

class OpenSaveServerPrivate;
class OpenSaveServer : public IDialogPartOpenSave
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(OpenSaveServer)

public:
  virtual ~OpenSaveServer() = 0;

  QUrl storageUrl() const override final;
  void validateUserSelections() override final;

Q_SIGNALS:
  void changeDatabaseNameTooltip(const QString &tooltip);

protected:
  const std::unique_ptr<OpenSaveServerPrivate> d_ptr;
  explicit OpenSaveServer(OpenSaveServerPrivate &d, Backend *backend, eType storageType, eAction action);

protected Q_SLOTS:
  virtual void setHostName(const QString &hostName);
  virtual void setDatabaseName(const QString &databaseName);
  virtual void setDatabaseNameTooltip(const QString &databaseNameTooltip) = 0;
};

} // namespace Sql
} // namespace Storage

#endif
