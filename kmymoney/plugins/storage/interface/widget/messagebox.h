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

#ifndef STORAGE_INTERFACE_WIDGET_MESSAGEBOX_H
#define STORAGE_INTERFACE_WIDGET_MESSAGEBOX_H

#include <kmm_istorage_export.h>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// QT Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "imessagebox.h"

namespace Storage {
namespace Widget {

class KMM_ISTORAGE_EXPORT MessageBox : public IMessageBox
{
public:
  using IMessageBox::IMessageBox;

  void questionYesNo(const QString &message, const QString &title, const QStringList &data) override;
  void warning(const QString &message, const QString &title, const QStringList &data) override;
  void error(const QString &message, const QString &title, const QStringList &data) override;
  void detailedError(const QString &message, const QString &title, const QStringList &data) override;
  void informationList(const QString &message, const QString &title, const QStringList &data) override;
  void information(const QString &message, const QString &title, const QStringList &data) override;
};

} // namespace Widget
} // namespace Storage

#endif
