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

#ifndef STORAGE_XML_FRONTEND_OPENSAVE_H
#define STORAGE_XML_FRONTEND_OPENSAVE_H

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

namespace Xml {
class Backend;

class OpenSavePrivate;
class OpenSave : public IDialogPartOpenSave
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(OpenSave)

public:
  virtual ~OpenSave() = 0;

  QUrl storageUrl() const override final;
  void validateUserSelections() override final;

protected:
  const std::unique_ptr<OpenSavePrivate> d_ptr;
  explicit OpenSave(OpenSavePrivate &d, Backend *backend, eType storageType, eAction action);

protected Q_SLOTS:
  virtual void setFilePath(const QString &text);
};

} // namespace Xml
} // namespace Storage

#endif
