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

#ifndef STORAGE_MANAGER_FRONTEND_OPENSAVE_H
#define STORAGE_MANAGER_FRONTEND_OPENSAVE_H

#include <memory>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// QT Includes

#include <QObject>

// ----------------------------------------------------------------------------
// Project Includes

namespace Storage {
enum class eAction;
class IStoragePlugin;
typedef QMap<QString, IStoragePlugin *>    StoragePluginsMap;

namespace Manager {

class OpenSavePrivate;
class OpenSave : public QObject
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(OpenSave)

public:
  virtual ~OpenSave() = 0;

  QUrl storageUrl();
  virtual QObject *uiPart() const = 0;

Q_SIGNALS:
  void accepted();
  void rejected();

  void userBasedValidityChanged(bool isValid);

protected:
  const std::unique_ptr<OpenSavePrivate> d_ptr;
  explicit OpenSave(OpenSavePrivate &d);

protected Q_SLOTS:
  virtual void slotStorageTypeChanged(const QString &text);
};

} // namespace Manager
} // namespace Storage

#endif
