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

#include "imessagebox.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QMap>

// ----------------------------------------------------------------------------
// KDE Includes

#include <KLocalizedString>

// ----------------------------------------------------------------------------
// Project Includes

#include "enums.h"

namespace Storage {

IMessageBox::IMessageBox(QObject *parent) :
  m_parent(parent)
{
};
IMessageBox::~IMessageBox() = default;

QString IMessageBox::fillPlaceholders(const QString &message, const QStringList &data)
{
  QString messageWithData = message;
  for (const auto &dataPart : data)
    messageWithData = messageWithData.arg(dataPart);
  return messageWithData;
}

} // namespace Storage
