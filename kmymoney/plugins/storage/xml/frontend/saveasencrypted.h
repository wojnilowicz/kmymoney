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

#ifndef STORAGE_XML_FRONTEND_SAVEASENCRYPTED_H
#define STORAGE_XML_FRONTEND_SAVEASENCRYPTED_H

#include <memory>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// QT Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "storage/interface/idialogpartopensave.h"

namespace Storage {
namespace Xml {
class Backend;

class SaveAsEncryptedPrivate;
class SaveAsEncrypted : public IDialogPartOpenSave
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(SaveAsEncrypted)

public:
  virtual ~SaveAsEncrypted() = 0;

  QUrl storageUrl() const override final;
  void setStorageUrl(const QUrl &storageUrl) override;
  void validateUserSelections() override final;

protected:
  const std::unique_ptr<SaveAsEncryptedPrivate> d_ptr;
  explicit SaveAsEncrypted(SaveAsEncryptedPrivate &d, Backend *backend, eType storageType);

protected Q_SLOTS:
  virtual void setFilePath(const QString &text);
  void setKeysList(const QStringList &keysList);
};

} // namespace Xml
} // namespace Storage

#endif
