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

#ifndef STORAGE_INTERFACE_ENUMS_H
#define STORAGE_INTERFACE_ENUMS_H

#include <QHashFunctions>

namespace Storage {

  enum class eAction {
    Unknown,
    Open,
    Save,
    Edit
  };
  
  enum class eUI {
    Unknown,
    Widget,
    Quick
  };

//  inline uint qHash(const UI key, uint seed) { return ::qHash(static_cast<uint>(key), seed); }

  enum class eType {
    Unknown,
    kmmn_XmlGzip,
    kmmn_XmlGpg,
    kmmn_XmlPlain,
    kmmn_XmlAnonymous,
    kmmn_MySql,
    kmmn_PostgreSql,
    kmmn_SQLite,
    kmmn_SqlCipher,

    kmm_XmlGzip,
    kmm_XmlGpg,
    kmm_XmlPlain,
    kmm_XmlAnonymous,
    kmm_MySql,
    kmm_PostgreSql,
    kmm_SQLite,
    kmm_SqlCipher
  };
    inline uint qHash(const eType key, uint seed) { return ::qHash(static_cast<uint>(key), seed); }

  enum class eVersionStatus {
    Unknown,
    Current,
    Older,
    Newer,
    Deprecated,
  };

  enum class eProducer {
    Unknown,
    KMyMoney,
    KMyMoneyNEXT
  };

  enum class eIssue {
    Invalid,
    InvalidStorage,
    WrongTypeSelected,
    NotReadable,
    NotWriteable,
    FileDoesntExist,
    TooShort,
    ServerNotRunning,
    NeedsDatabasePresent,
    NeedsDatabaseCreated,
    DatabaseCreated,
    DatabaseNotCreated,
    NeedsPassphrase,
    NeedsCredentials,
    BadPassphrase,
    BadCredentials,
    NeedsConversion,
    NeedsUpgrade,
    NeedsNewerSoftware,
    OnlyEncryptionKeys,
    MissingKeys,
    UnableToDetectKeys,
    SomeMissingEncryptionKeys,
    MissingEncryptionKeys,
    MissingDecryptionKeys,
    MissingRecoveryKey,
    RecoveryKeyInstalled,
    RecoveryKeyCorrupt,
    RecoveryKeyDownloadFailed,
    ExpiringEncryptionKeys,
    SomebodyIsLogedIn,
    UnrecommendedType,
    MissingPlugins,
    MissingPlugin
  };

  enum class eAnswer {
    Yes,
    No,
    Ok,
    Cancel,
    Retry
  };

//  inline uint qHash(const Type key, uint seed) { return ::qHash(static_cast<uint>(key), seed); }
}


#endif
