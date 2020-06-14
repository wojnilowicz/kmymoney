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

#include "message.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QMap>

// ----------------------------------------------------------------------------
// KDE Includes

#include <KLocalizedString>

// ----------------------------------------------------------------------------
// Project Includes

#include "storage/interface/enums.h"

namespace Storage {
namespace Manager {

QString Message::standardMessage(eIssue issue)
{
  const QMap<eIssue, QString> messages {
    {eIssue::MissingPlugins,        i18n( "There is no storage plugin loaded.<br>"
                                          "Please enable one under<br>"
                                          "<b>Settings->Configure KMyMoneyNEXT->Plugins</b><br>"
                                          "Until then you won't be able to anything with your storage.\n"
                                          )},
    {eIssue::MissingPlugin,         i18n( "There is no storage plugin loaded that can handle <b>%1</b>.<br>"
                                          "Please try to enable the right one under<br>"
                                          "<b>Settings->Configure KMyMoneyNEXT->Plugins</b><br>"
                                          "Until then you won't be able to anything with your storage.\n"
                                          )},
  };

  return messages.value(issue, IMessage::standardMessage(issue));
}

QString Message::standardTitle(eIssue issue)
{
  const QMap<eIssue, QString> titles {
    {eIssue::MissingPlugins,  i18n("Missing plugins")},
    {eIssue::MissingPlugin,   i18n("Missing plugin")},
  };

  return titles.value(issue, IMessage::standardTitle(issue));
}

} // namespace Manager
} // namespace Storage
