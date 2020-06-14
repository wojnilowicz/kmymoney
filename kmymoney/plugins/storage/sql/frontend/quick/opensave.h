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

#ifndef STORAGE_SQL_FRONTEND_QUICK_OPENSAVE_H
#define STORAGE_SQL_FRONTEND_QUICK_OPENSAVE_H

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// QT Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "frontend/opensave.h"

namespace Storage {
namespace Sql {
namespace Quick {

class OpenSavePrivate;
class OpenSave : public Sql::OpenSave
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(OpenSave)
  Q_PROPERTY(QString filePath READ filePath WRITE setFilePath NOTIFY filePathChanged)

public:
  explicit OpenSave(Backend *backend,
                    eType storageType,
                    eAction actionType,
                    const QUrl &startDirectory);
  ~OpenSave() override final;

  QObject *uiPart() override final;
  void setStorageUrl(const QUrl &storageUrl) override final;

  QString filePath() const;

Q_SIGNALS:
    void filePathChanged(const QString &filePath);

protected Q_SLOTS:
    void setFilePath(const QString &text) override final;
};

} // namespace Quick$
} // namespace Sql
} // namespace Storage

#endif
