/*
 * Copyright 2019       Łukasz Wojniłowicz <lukasz.wojnilowicz@gmail.com>
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

#ifndef STORAGE_INTERFACE_IDIALOGPART_H
#define STORAGE_INTERFACE_IDIALOGPART_H

#include <kmm_istorage_export.h>

// ----------------------------------------------------------------------------
// QT Includes

#include <QObject>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

namespace Storage {
enum class eType;

class KMM_ISTORAGE_EXPORT IDialogPartOpenSave : public QObject
{
  Q_OBJECT

public:
  using QObject::QObject;
  virtual ~IDialogPartOpenSave() = 0;

  virtual QObject *uiPart() = 0;

  virtual QUrl storageUrl() const = 0;
  virtual void setStorageUrl(const QUrl &storageUrl) = 0;

  virtual void validateUserSelections() = 0;

Q_SIGNALS:
  void userBasedValidityChanged(bool isValid);
};

} // namespace Storage

#endif
