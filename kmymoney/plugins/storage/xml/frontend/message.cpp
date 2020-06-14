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
namespace Xml {

QString Message::standardMessage(eIssue issue)
{
  const QMap<eIssue, QString> messages {
    {eIssue::OnlyEncryptionKeys,        i18n( "You chose keys that are able only to encrypt.\n"
                                              "It means, that you won't be able to decrypt your storage yourself.\n"
                                              "Do you want to continue?")},
    {eIssue::SomeMissingEncryptionKeys, i18n( "Some encryption keys are missing.\n"
                                              "Do you want to continue?")},
    {eIssue::MissingEncryptionKeys,     i18n( "Your storage has been encrypted with keys that are currently missing on your system.\n"
                                              "It means, that you won't be able to encrypt your storage with them anymore.")},
    {eIssue::MissingDecryptionKeys,     i18n( "Your storage has been encrypted with keys that are currently missing on your system.\n"
                                              "It means, that you won't be able to decrypt your storage.")},
    {eIssue::MissingRecoveryKey,        i18n( "You chose to encrypt with recovery key by default, but the key is not available on your system.\n"
                                              "Do you want KMyMoneyNEXT to install it for you from %1?")},
    {eIssue::ExpiringEncryptionKeys,    i18n( "Following keys will soon expire.")},
    {eIssue::UnableToDetectKeys,        i18n( "Unable to dectect keys used to decrypt your storage.")},
  };

  return messages.value(issue, IMessage::standardMessage(issue));
}

QString Message::standardTitle(eIssue issue)
{
  const QMap<eIssue, QString> titles {
    {eIssue::OnlyEncryptionKeys,        i18n("Storage won't be decryptable")},
    {eIssue::SomeMissingEncryptionKeys, i18n("Missing encryption keys")},
    {eIssue::MissingEncryptionKeys,     i18n("Missing encryption keys")},
    {eIssue::MissingDecryptionKeys,     i18n("Missing decryption keys")},
    {eIssue::MissingRecoveryKey,        i18n("Missing recovery key")},
    {eIssue::ExpiringEncryptionKeys,    i18n("Keys about to expire")},
    {eIssue::UnableToDetectKeys,        i18n("Unable to detect keys")},
  };

  return titles.value(issue, IMessage::standardTitle(issue));
}

} // namespace Xml
} // namespace Storage
