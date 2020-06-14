/*
 * Copyright 2020       Łukasz Wojniłowicz <lukasz.wojnilowicz@gmail.com>
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

#ifndef STORAGE_INTERFACE_NAMES_H
#define STORAGE_INTERFACE_NAMES_H

#include "ui.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QMap>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "enums.h"

namespace Storage {
namespace StorageUi {

const QMap<eUI, QString> types {
  {eUI::Widget, QStringLiteral("widget")},
  {eUI::Quick, QStringLiteral("quick")}
};

QString typeString(eUI typeEnum)
{
  return types.value(typeEnum);
}

eUI typeEnum(const QString &typeString)
{
  return types.key(typeString, eUI::Unknown);
}

} // namespace Ui
} // namespace Storage


#endif
