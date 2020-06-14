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
namespace Sql {

QString Message::standardMessage(eIssue issue)
{
  const QMap<eIssue, QString> messages {
    {eIssue::ServerNotRunning,        i18n( "Cannot connect to your server.\n"
                                            "Please check if it's running.")},
    {eIssue::NeedsDatabasePresent,    i18n( "Database doesn't exist.")},
    {eIssue::NeedsDatabaseCreated,    i18n( "Database doesn't exist.\n"
                                             "Do you want to create it?")},
    {eIssue::DatabaseCreated,         i18n( "Database has been created successfully.")},
    {eIssue::DatabaseNotCreated,      i18n( "Failed to create database.")},
    {eIssue::SomebodyIsLogedIn,       i18n( "Database apparently in use<br>Opened by <b>%1</b> on <b>%2</b> at <b>%3</b>.<br>Open anyway?")},
  };

  return messages.value(issue, IMessage::standardMessage(issue));
}

QString Message::standardTitle(eIssue issue)
{
  const QMap<eIssue, QString> titles {
    {eIssue::ServerNotRunning,         i18n("Server not running")},
    {eIssue::NeedsDatabasePresent,    i18n("Storage doesn't exist")},
    {eIssue::NeedsDatabaseCreated,    i18n("Storage doesn't exist")},
    {eIssue::DatabaseCreated,         i18n("Database created")},
    {eIssue::DatabaseNotCreated,      i18n("Database not created")},
    {eIssue::SomebodyIsLogedIn,       i18n("Database in use")},
  };

  return titles.value(issue, IMessage::standardTitle(issue));
}

} // namespace Sql
} // namespace Storage
