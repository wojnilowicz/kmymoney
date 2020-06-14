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
namespace Gpg {

QString Message::standardMessage(eIssue issue)
{
  const QMap<eIssue , QString> messages {
    {eIssue::RecoveryKeyCorrupt,          i18n( "Installed key with fingerprint: %1.\n"
                                               "Expected key with fingerprint: %2.\n"
                                               "It means, that the key could be forged.\n"
                                               "Do you want to uninstall it?")},
    {eIssue::RecoveryKeyInstalled,        i18n( "Recovery key installed successfully.")},
    {eIssue::RecoveryKeyDownloadFailed,   i18n( "Failed to download recovery key.\n"
                                                "Would you like to retry?")}
  };

  return messages.value(issue);
}

QString Message::standardTitle(eIssue issue)
{
  const QMap<eIssue , QString> titles {
    {eIssue::RecoveryKeyCorrupt,        i18n("Corrupt recovery key")},
    {eIssue::RecoveryKeyInstalled,      i18n("Recovery key installed")},
    {eIssue::RecoveryKeyDownloadFailed, i18n("Download failure")}
  };

  return titles.value(issue);
}

} // namespace Gpg
} // namespace Storage
