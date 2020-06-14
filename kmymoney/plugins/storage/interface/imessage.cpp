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

#include "imessage.h"

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

IMessage::~IMessage() = default;

QString IMessage::standardMessage(eIssue issue)
{
  const QMap<eIssue, QString> messages {
    {eIssue::Invalid,                 i18n( "This message is unexpected.\n"
                                            "Please report it.")},
    {eIssue::InvalidStorage,          i18n( "Your storage is invalid.")},
    {eIssue::WrongTypeSelected,       i18n( "You try to open your storage as <br/><b>%1</b><br/>but it's<br/><b>%2</b><br/>"
                                            "Please select correct storage type and then try again.")},
    {eIssue::NotReadable,             i18n( "You have no read permission to the storage.")},
    {eIssue::NotWriteable,            i18n( "You have no write permission to the storage.")},
    {eIssue::FileDoesntExist,         i18n( "Storage doesn't exist.")},
    {eIssue::TooShort,                i18n( "Storage is too short.")},
    {eIssue::NeedsUpgrade,            i18n( "Your storage is from older %1.\n"
                                            "Upgrade is possible.\n"
                                            "Do you want to continue?")},
    {eIssue::NeedsNewerSoftware,      i18n( "Your storage is from newer %1.\n"
                                            "Please use newer KMyMoneyNEXT to open it.")},
    {eIssue::NeedsConversion,         i18n( "Your storage is from KMyMoney and is about to be converted to KMyMoneyNEXT.\n"
                                            "It means, that you won't be able to open it in KMyMoney afterwards.\n"
                                            "Do you want to continue?")},
    {eIssue::UnrecommendedType,       i18n( "You are about to save as %1 storage.\n"
                                            "It's not recommended as it may cause compatibility issues.\n"
                                            "Do you still want to continue?")},
  };

  return messages.value(issue);
}

QString IMessage::standardTitle(eIssue issue)
{
  const QMap<eIssue, QString> titles {
    {eIssue::Invalid,                 i18n("Invalid message")},
    {eIssue::InvalidStorage,          i18n("Invalid storage")},
    {eIssue::WrongTypeSelected,       i18n("Wrong storage type selected")},
    {eIssue::NotReadable,             i18n("Storage not readable")},
    {eIssue::NotWriteable,            i18n("Storage not writeable")},
    {eIssue::FileDoesntExist,         i18n("Storage doesn't exist")},
    {eIssue::TooShort,                i18n("Storage is too short")},
    {eIssue::NeedsUpgrade,            i18n("Storage version upgrade")},
    {eIssue::NeedsNewerSoftware,      i18n("Unsupported storage version")},
    {eIssue::NeedsConversion,         i18n("Storage needs conversion")},
    {eIssue::UnrecommendedType,       i18n("Unrecommended storage type")},
  };

  return titles.value(issue);
}

} // namespace Storage
