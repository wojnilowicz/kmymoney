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

#ifndef STORAGE_MANAGER_FRONTEND_QUICK_OPENSAVE_H
#define STORAGE_MANAGER_FRONTEND_QUICK_OPENSAVE_H

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// QT Includes

#include <QtGlobal>
#include <QMap>

// ----------------------------------------------------------------------------
// Project Includes

#include "frontend/opensave.h"

namespace Storage {
class IDialogPartOpenSave;
enum class eAction;
namespace Manager {
namespace Quick {

class OpenSavePrivate;
class OpenSave : public Manager::OpenSave
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(OpenSave)

public:
  explicit OpenSave(QObject *parent,
                    const StoragePluginsMap &storagePlugins,
                    eAction storageAction,
                    const QUrl &presetUrl);
  ~OpenSave() override final;

  QObject *uiPart() const override final;
  Q_INVOKABLE QObject *dialogPartUIPart();
};

} // namespace Quick$
} // namespace Manager
} // namespace Storage

#endif
