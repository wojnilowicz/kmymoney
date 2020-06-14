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

#include "backend.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QVector>
#include <QUrl>
#include <QUrlQuery>
#include <QDebug>
#include <QDateTime>
#include <QFile>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

// ----------------------------------------------------------------------------
// KDE Includes

#include <KLocalizedString>

// ----------------------------------------------------------------------------
// Project Includes

#include "mymoney/mymoneyexception.h"
#include "mymoney/mymoneyfile.h"

#include "storage/interface/ivalidator.h"
#include "storage/interface/enums.h"

#include "mymoneystoragesql.h"
#include "upgrader.h"
#include "converter.h"
#include "url.h"
#include "names.h"

#include "mysql/backend.h"
#include "postgresql/backend.h"
#include "sqlcipher/backend.h"

static constexpr auto bytesToInferType = 16;

namespace Storage {
namespace Sql {

enum class eFileType {
  Unknown,
  SQLite
};

enum class eDatabaseType {
  MySql,
  PostgreSql,
  SQLite,
  SqlCipher
};

const QMap<eDatabaseType, QString> sqlDriverMap {
  {eDatabaseType::MySql,      QStringLiteral("QMYSQL")},
  {eDatabaseType::PostgreSql, QStringLiteral("QPSQL")},
  {eDatabaseType::SQLite,     QStringLiteral("QSQLITE")},
  {eDatabaseType::SqlCipher,  QStringLiteral("QSQLCIPHER")},
};

const QVector<eType> kmmTypes {
  eType::kmm_MySql,
  eType::kmm_PostgreSql,
  eType::kmm_SQLite,
  eType::kmm_SqlCipher,
};

const QVector<eType> kmmnTypes {
  eType::kmmn_MySql,
  eType::kmmn_PostgreSql,
  eType::kmmn_SQLite,
  eType::kmmn_SqlCipher,
};

const QMap<eType, eType> kmmToKmmnTypeMap {
  {eType::kmm_SQLite,     eType::kmmn_SQLite},
  {eType::kmm_PostgreSql, eType::kmmn_PostgreSql},
  {eType::kmm_SqlCipher,  eType::kmmn_SqlCipher},
  {eType::kmm_MySql,      eType::kmmn_MySql},
};

const QMap<eType, eDatabaseType> storageToDatabaseTypeMap {
  {eType::kmm_MySql,     eDatabaseType::MySql},
  {eType::kmmn_MySql,     eDatabaseType::MySql},

  {eType::kmm_PostgreSql,     eDatabaseType::PostgreSql},
  {eType::kmmn_PostgreSql,     eDatabaseType::PostgreSql},

  {eType::kmm_SQLite,     eDatabaseType::SQLite},
  {eType::kmmn_SQLite,     eDatabaseType::SQLite},

  {eType::kmm_SqlCipher,     eDatabaseType::SqlCipher},
  {eType::kmmn_SqlCipher,     eDatabaseType::SqlCipher},
};

class BackendPrivate
{
  Q_DISABLE_COPY(BackendPrivate)
  Q_DECLARE_PUBLIC(Backend)

public:
  explicit BackendPrivate(Backend *q);
  ~BackendPrivate() = default;

  eProducer storageProducer(const QUrl &url);

  QString sqlDriverNameByStorageType(const QUrl &url) const;

  QVector<eType> typesFiltered(const QVector<eType> &excludedTypes = QVector<eType>{}) const;

  void setConnectionOptions(const QUrl &url);

  QSqlDatabase connectToDatabaseServer(const QUrl &url, const QString &connectionName);
  QSqlDatabase connectToDatabaseFile(const QUrl &url, const QString &connectionName);
  QSqlDatabase connectToDatabase(const QUrl &url);

  uint storageVersionKMM(const QUrl &url);
  uint storageVersionKMMN(const QUrl &url);
  uint storageVersion(const QUrl &url);
  eVersionStatus versionStatus(const QUrl &url);

  QPair<QString, QDateTime> readLogonData(const QUrl &url);
  void writeLogonData(const QUrl &url, const QPair<QString, QDateTime> &logonData);
  void writeLogonDataForFile(const QUrl &url);
  void writeLogonDataForServer(const QUrl &url);

  QVector<Issue> checkDatabaseExistForOpening(const QUrl &url);
  QVector<Issue> checkDatabaseExistForSaving(const QUrl &url);
  bool checkDatabaseExist(const QUrl &url);

  QVector<Issue> checkIfServerIsRunning(const QUrl &url);
  QVector<Issue> validateForAccess(const QUrl &url);
  QVector<Issue> validateForExclusivity(const QUrl &url);
  QVector<Issue> validateForPassword(const QUrl &url);
  QVector<Issue> validateForOpenFile(const QUrl &url);
  QVector<Issue> validateForSaveFile(const QUrl &url);

  MySql::Backend *mysql(QSqlDatabase &db);
  PostgresSql::Backend *postgresql(QSqlDatabase &db);
  SQLite::Backend *sqlite(QSqlDatabase &db);
  SQLCipher::Backend *sqlciper(QSqlDatabase &db);

  QString uniqueConnectionName() const;
  void resetCache(const QUrl &url);

  Backend        *q_ptr;

  struct StorageInfo {
    eType type = eType::Unknown;
    eProducer producer = eProducer::Unknown;
    QString connectionName;
  };

  QMap<QUrl, StorageInfo> m_cache;
  ProgressCallback m_callback = nullptr;

private:
  std::map<eDatabaseType, std::unique_ptr<IBackend>> m_backends;
};

BackendPrivate::BackendPrivate(Backend *q) :
  q_ptr(q)
{
  m_backends.insert(std::make_pair(eDatabaseType::MySql, std::make_unique<MySql::Backend>()));
  m_backends.insert(std::make_pair(eDatabaseType::PostgreSql, std::make_unique<PostgresSql::Backend>()));
  m_backends.insert(std::make_pair(eDatabaseType::SQLite, std::make_unique<SQLite::Backend>()));
  m_backends.insert(std::make_pair(eDatabaseType::SqlCipher, std::make_unique<SQLCipher::Backend>()));
}

eProducer BackendPrivate::storageProducer(const QUrl &url)
{
  if (m_cache.contains(url) && m_cache.value(url).producer != eProducer::Unknown)
    return m_cache.value(url).producer;

  const auto db = connectToDatabase(url);
  if (!db.isOpen())
    return eProducer::Unknown;

  const auto record = db.record(tableName(Table::FileInfo));
  const auto containsFixLevel = record.contains(columnName(Column::FileInfo::FixLevel));
  const auto containsVersion = record.contains(columnName(Column::FileInfo::Version));

  auto version = 0;

  if (containsVersion) {
    QSqlQuery query(db);
    const auto queryString = QString::fromLatin1("SELECT %1 FROM %2;").
                             arg(columnName(Column::FileInfo::Version)).
                             arg(tableName(Table::FileInfo));

    query.exec(queryString);
    query.first();
    version = query.value(0).toUInt();
  }

  auto producer = eProducer::Unknown;
  if (!containsFixLevel && containsVersion && version >= 20200101)
    producer = eProducer::KMyMoneyNEXT;
  else if (containsFixLevel && containsVersion)
    producer = eProducer::KMyMoney;

  if (producer != eProducer::Unknown)
    m_cache[url].producer = producer;
  return producer;
}

QString BackendPrivate::sqlDriverNameByStorageType(const QUrl &url) const
{
  QString sqlDriverName;
  Url enhancedUrl(url);
  auto storageType = enhancedUrl.decodeStorageType();
  if (storageType == eType::Unknown)
    return sqlDriverName;
  auto databaseType = storageToDatabaseTypeMap.value(storageType);
  sqlDriverName = sqlDriverMap.value(databaseType, QString());
  return sqlDriverName;
}

QVector<eType> BackendPrivate::typesFiltered(const QVector<eType> &excludedTypes) const
{
  QVector<eType> types;
  types.append(kmmTypes);
  types.append(kmmnTypes);

  const auto &availableSqlDrivers = QSqlDatabase::drivers();
  for (auto i = types.count() - 1; i > 0; --i) {
    const auto storageType = types.at(i);
    const auto databaseType = storageToDatabaseTypeMap.value(storageType);
    const auto requiredDatabaseName = sqlDriverMap.value(databaseType, QStringLiteral("Unknown"));
    if (excludedTypes.contains(storageType) ||
        !availableSqlDrivers.contains(requiredDatabaseName))
      types.removeAt(i);
  }

  return types;
}

void BackendPrivate::setConnectionOptions(const QUrl &url)
{
  auto db = connectToDatabase(url);
  Url enhancedUrl(url);
  auto storageType = enhancedUrl.decodeStorageType();
  switch (storageType) {
    case eType::kmm_MySql:
    case eType::kmmn_MySql:
      mysql(db)->setConnectionOptions();
      break;
    case eType::kmm_SQLite:
    case eType::kmmn_SQLite:
      sqlciper(db)->setConnectionOptions();
      break;
    case eType::kmm_SqlCipher:
    case eType::kmmn_SqlCipher:
      sqlite(db)->setConnectionOptions();
    default:
    break;
  }
}

QSqlDatabase BackendPrivate::connectToDatabaseServer(const QUrl &url, const QString &connectionName)
{
  QSqlDatabase db;
  if (url.host().isEmpty())
    return db;

  auto sqlDriverName = sqlDriverNameByStorageType(url);

  db = QSqlDatabase::addDatabase(sqlDriverName, connectionName);
  db.setHostName(url.host());
  db.setUserName(url.userName());
  db.setPassword(url.password());


  Url enhancedUrl(url);
  auto storageType = enhancedUrl.decodeStorageType();
  if (storageType == eType::kmm_PostgreSql || storageType == eType::kmmn_PostgreSql)
    db.setDatabaseName("postgres"); // PostgreSQL requires database name at connection, so connect to the default one

  db.open();

  return db;
}

QSqlDatabase BackendPrivate::connectToDatabaseFile(const QUrl &url, const QString &connectionName)
{
  QSqlDatabase db;
  auto databaseName = url.path();
  if (databaseName.isEmpty())
    return db;

  auto sqlDriverName = sqlDriverNameByStorageType(url);
  if (sqlDriverName.isEmpty() && sqlite(db)->magicHeaderMatches(url))
    sqlDriverName = sqlDriverMap.value(eDatabaseType::SQLite);

  db = QSqlDatabase::addDatabase(sqlDriverName, connectionName);
  db.setDatabaseName(databaseName);
  if (!db.open())
    return db;

  Url enhancedUrl(url);
  auto storageType = enhancedUrl.decodeStorageType();
  if (storageType == eType::kmm_SqlCipher || storageType == eType::kmmn_SqlCipher)
    if (!sqlciper(db)->unlockDatabase(url))
      db = QSqlDatabase();

  return db;
}

QSqlDatabase BackendPrivate::connectToDatabase(const QUrl &url)
{
  Q_Q(Backend);

  QSqlDatabase db;
  if (m_cache.contains(url)) {
    const auto &connectionName = m_cache.value(url).connectionName;
    if (!connectionName.isEmpty() && QSqlDatabase::contains(connectionName)) {
      db = QSqlDatabase::database(connectionName);
      if (db.isOpen()) {
        return db;
      } else {
        db = QSqlDatabase();
        resetCache(url);
      }
    }
  }

  auto fileBased = q->isStorageFileBased(url);
  const auto connectionName = uniqueConnectionName();
  if (fileBased)
    db = connectToDatabaseFile(url, connectionName);
  else
    db = connectToDatabaseServer(url, connectionName);

  auto &storageInfo = m_cache[url];
  storageInfo.connectionName = connectionName;
  if (db.isOpen())
    setConnectionOptions(url);

  return db;
}

uint BackendPrivate::storageVersionKMM(const QUrl &url)
{
  auto db = connectToDatabase(url);
  QSqlQuery query(db);
  const auto queryString = QString::fromLatin1("SELECT %1, %2 FROM %3;").
                           arg(columnName(Column::FileInfo::Version)).
                           arg(columnName(Column::FileInfo::FixLevel)).
                           arg(tableName(Table::FileInfo));
  if (!query.exec(queryString))
    return 0;
  query.first();
  auto fileVersion = query.value(columnName(Column::FileInfo::Version)).toUInt();
  auto fileFixVersion = query.value(columnName(Column::FileInfo::FixLevel)).toUInt();
  return fileVersion * 1000 + fileFixVersion;
}

uint BackendPrivate::storageVersionKMMN(const QUrl &url)
{
  const auto db = connectToDatabase(url);
  QSqlQuery query(db);
  const auto queryString = QString::fromLatin1("SELECT %1 FROM %2;").
                           arg(columnName(Column::FileInfo::Version)).
                           arg(tableName(Table::FileInfo));

  if (!query.exec(queryString))
    return 0;
  query.first();
  return query.value(0).toUInt();
}

uint BackendPrivate::storageVersion(const QUrl &url)
{
  switch (storageProducer(url)) {
    case eProducer::KMyMoney:
      return storageVersionKMM(url);
    case eProducer::KMyMoneyNEXT:
      return storageVersionKMMN(url);
    default:
      throw MYMONEYEXCEPTION_CSTRING("Failed to parse storage version.");
  }
}

eVersionStatus BackendPrivate::versionStatus(const QUrl &url)
{
  const auto producer = storageProducer(url);
  const auto supportedVersions = Upgrader::supportedVersions(producer);

  if (supportedVersions.isEmpty())
    return eVersionStatus::Unknown;

  const auto highestSupportedVersion = *std::max_element(supportedVersions.cbegin(), supportedVersions.cend());
  const auto version = storageVersion(url);

  if (version == highestSupportedVersion)
    return eVersionStatus::Current;
  else if (version > highestSupportedVersion)
    return eVersionStatus::Newer;
  else
    return eVersionStatus::Older;
}


QVector<Issue> BackendPrivate::validateForPassword(const QUrl &url)
{
  QVector<Issue> issues;
  if (url.password().isEmpty())
    issues.append({eIssue::BadPassphrase, QStringList{url.toString()}});
  return issues;
}

QVector<Issue> BackendPrivate::validateForOpenFile(const QUrl &url)
{
  QVector<Issue> issues;

  const auto localFileName = url.path();
  const auto fileInfo = QFileInfo(localFileName);
  if (!fileInfo.exists()) {
    issues.append({eIssue::FileDoesntExist, QStringList()});
    return issues;
  }

  if (!fileInfo.permissions().testFlag(QFileDevice::ReadUser)) {
    issues.append({eIssue::NotReadable, QStringList()});
    return issues;
  }

  constexpr auto expectedFileSize = std::max({bytesToInferType});
  const auto fileSize = fileInfo.size();
  if (fileSize < expectedFileSize) {
    issues.append({eIssue::TooShort, QStringList()});
    return issues;
  }

  return issues;
}

QVector<Issue> BackendPrivate::validateForSaveFile(const QUrl &url)
{
  QVector<Issue> issues;

  const auto localFileName = url.path();
  const auto fileInfo = QFileInfo(localFileName);
  if (fileInfo.exists() && !fileInfo.permissions().testFlag(QFileDevice::WriteUser))
    issues.append({eIssue::NotWriteable, QStringList()});

  return issues;
}

QVector<Issue> BackendPrivate::checkDatabaseExistForOpening(const QUrl &url)
{
  QVector<Issue> issues;
  if (!checkDatabaseExist(url))
    issues.append({eIssue::NeedsDatabasePresent, QStringList{url.toString()}});

  return issues;

}

QVector<Issue> BackendPrivate::checkDatabaseExistForSaving(const QUrl &url)
{
  QVector<Issue> issues;
  if (!checkDatabaseExist(url))
    issues.append({eIssue::NeedsDatabaseCreated, QStringList{url.toString()}});

  return issues;
}

bool BackendPrivate::checkDatabaseExist(const QUrl &url)
{
  Q_Q(Backend);
  auto db = connectToDatabase(url);
  const auto storageType = q->storageType(url);

  switch (storageType) {
    case eType::kmm_MySql:
    case eType::kmmn_MySql:
      if (!mysql(db)->databaseExists(url))
        return false;
      else
        mysql(db)->selectDatabase(url); // move that out of this method
      return true;
    case eType::kmm_PostgreSql:
    case eType::kmmn_PostgreSql:
      if (!postgresql(db)->databaseExists(url))
        return false;
      else
        postgresql(db)->selectDatabase(url); // move that out of this method
      return true;
    default:
      return false;
  }
  return true;
}

QVector<Issue> BackendPrivate::checkIfServerIsRunning(const QUrl &url)
{
  QVector<Issue> issues;

  Url enhancedUrl(url);
  const auto declaredStorageType = enhancedUrl.decodeStorageType();

  auto db = connectToDatabase(url);
  if (db.isOpen())
    return issues;

  bool isServerRunning = true;
  switch (declaredStorageType) {
    case eType::kmm_MySql:
    case eType::kmmn_MySql:
      isServerRunning = mysql(db)->isServerRunning(url);
      break;

    case eType::kmm_PostgreSql:
    case eType::kmmn_PostgreSql:
      isServerRunning = postgresql(db)->isServerRunning(url);
      break;
    default:
      break;
  }

  if (!isServerRunning) {
    auto lastError = db.lastError();
    db = QSqlDatabase(); // to unuse a connection
    resetCache(url);
    issues.append({eIssue::ServerNotRunning, QStringList{lastError.databaseText()}});
  }

  return issues;
}

QVector<Issue> BackendPrivate::validateForAccess(const QUrl &url)
{
  QVector<Issue> issues;

  const QVector<eType> typesNeedingPassphrase {
    eType::kmm_SqlCipher, eType::kmmn_SqlCipher
  };

  const QVector<eType> typesNeedingCredentials {
    eType::kmm_MySql,       eType::kmmn_MySql,
    eType::kmm_PostgreSql,  eType::kmmn_PostgreSql
  };

  Url enhancedUrl(url);
  const auto declaredStorageType = enhancedUrl.decodeStorageType();
  if (!typesNeedingPassphrase.contains(declaredStorageType) &&
      !typesNeedingCredentials.contains(declaredStorageType))
    return issues;

  auto db = connectToDatabase(url);
  if (db.isOpen())
    return issues;

  if (typesNeedingPassphrase.contains(declaredStorageType)) {
    if (!url.password().isEmpty())
      issues.append({eIssue::BadPassphrase, QStringList()});
    else
      issues.append({eIssue::NeedsPassphrase, QStringList()});

  } else if (typesNeedingCredentials.contains(declaredStorageType)) {
    if (!url.password().isEmpty() || !url.userName().isEmpty())
      issues.append({eIssue::BadCredentials, QStringList()});
    else
      issues.append({eIssue::NeedsCredentials, QStringList()});
  }

  db = QSqlDatabase();
  resetCache(url);
  return issues;
}

QPair<QString, QDateTime> BackendPrivate::readLogonData(const QUrl &url)
{
  QPair<QString, QDateTime> logonData;

  auto db = connectToDatabase(url);

  QSqlQuery query(db);
  auto queryString = QString::fromLatin1("SELECT %1, %2 FROM %3;").
                     arg(columnName(Column::FileInfo::LogonUser)).
                     arg(columnName(Column::FileInfo::LogonAt)).
                     arg(tableName(Table::FileInfo));
  query.exec(queryString);
  if (query.first()) {
    auto logonUser = query.value(0).toString();
    auto logonAt = query.value(1).toDateTime();
    logonData.first = logonUser;
    logonData.second = logonAt;
  }
  query.finish();
  return logonData;
}

void BackendPrivate::writeLogonData(const QUrl &url, const QPair<QString, QDateTime> &logonData)
{
  auto db = connectToDatabase(url);

  QSqlQuery query(db);

  const auto userName = logonData.first;
  const auto currentTime = logonData.second.toString(Qt::ISODate);

  auto queryString = QString::fromLatin1("UPDATE %1 SET %2 = %3, %4 = %5;").
                     arg(tableName(Table::FileInfo)).
                     arg(columnName(Column::FileInfo::LogonUser)).
                     arg(userName.isEmpty() ? "NULL" : QString::fromLatin1("'%1'").arg(userName)).
                     arg(columnName(Column::FileInfo::LogonAt)).
                     arg(currentTime.isEmpty() ? "NULL" : QString::fromLatin1("'%1'").arg(currentTime));

  query.exec(queryString);
  query.finish();
}

void BackendPrivate::writeLogonDataForFile(const QUrl &url)
{
  QPair<QString, QDateTime> logonData;
  #if defined(Q_OS_WIN)
    const auto userName = qgetenv("USERNAME");
  #else
    const auto userName = qgetenv("USER");
  #endif

  logonData.first = QString::fromUtf8(userName);
  logonData.second = QDateTime::currentDateTime();

  writeLogonData(url, logonData);
}

void BackendPrivate::writeLogonDataForServer(const QUrl &url)
{
  QPair<QString, QDateTime> logonData;
  logonData.first = url.userName();
  logonData.second = QDateTime::currentDateTime();

  writeLogonData(url, logonData);
}

QVector<Issue> BackendPrivate::validateForExclusivity(const QUrl &url)
{
  QVector<Issue> issues;

  auto logonData = readLogonData(url);
  if (!logonData.first.isEmpty()) {
    const QStringList logonDataList {
      logonData.first,
      logonData.second.date().toString(Qt::ISODate),
      logonData.second.time().toString("hh.mm.ss"),
    };
    issues.append({eIssue::SomebodyIsLogedIn, logonDataList});
  }

  return issues;
}

MySql::Backend *BackendPrivate::mysql(QSqlDatabase &db)
{
  auto p = m_backends.at(eDatabaseType::MySql).get();
  p->db = &db;
  return static_cast<MySql::Backend *>(p);
}

PostgresSql::Backend *BackendPrivate::postgresql(QSqlDatabase &db)
{
  auto p = m_backends.at(eDatabaseType::PostgreSql).get();
  p->db = &db;
  return static_cast<PostgresSql::Backend *>(p);
}

SQLite::Backend *BackendPrivate::sqlite(QSqlDatabase &db)
{
  auto p = m_backends.at(eDatabaseType::SQLite).get();
  p->db = &db;
  return static_cast<SQLite::Backend *>(p);
}

SQLCipher::Backend *BackendPrivate::sqlciper(QSqlDatabase &db)
{
  auto p = m_backends.at(eDatabaseType::SqlCipher).get();
  p->db = &db;
  return static_cast<SQLCipher::Backend *>(p);
}

QString BackendPrivate::uniqueConnectionName() const
{
  const QString connectionPrefix = QStringLiteral("storagesql");
  auto connectionNumber = 0;
  for (const auto &storageInfo : m_cache)
    if (storageInfo.connectionName.contains(connectionPrefix))
      connectionNumber++;

  auto connectionName = QString::fromLatin1("%1%2").
                        arg(connectionPrefix).
                        arg(connectionNumber);
  return connectionName;
}

void BackendPrivate::resetCache(const QUrl &url)
{
  auto &connectionName = m_cache.value(url).connectionName;
  if (!connectionName.isEmpty() && QSqlDatabase::contains(connectionName))
    QSqlDatabase::removeDatabase(connectionName);
  m_cache.remove(url);
}

Backend::Backend() :
  d_ptr(new BackendPrivate(this))
{
}

Backend::~Backend() = default;

void Backend::setProgressCallback(ProgressCallback callback)
{
  Q_D(Backend);
  d->m_callback = callback;
}

void Backend::openStorage(MyMoneyFile &storage, const QUrl &url)
{
  Q_D(Backend);

  const auto isStorageFileBased = Backend::isStorageFileBased(url);
  auto db = d->connectToDatabase(url);

  if (!isStorageFileBased)
    d->mysql(db)->selectDatabase(url);

  MyMoneyStorageSql SQLReader(db);
  SQLReader.readFile(storage);

  if (isStorageFileBased)
    d->writeLogonDataForFile(url);
  else
    d->writeLogonDataForServer(url);

  return;
}

void Backend::saveStorage(const MyMoneyFile &storage, const QUrl &url)
{
  Q_D(Backend);

  auto db = d->connectToDatabase(url);

  const auto isStorageFileBased = Backend::isStorageFileBased(url);

  if (isStorageFileBased &&
      QFile::exists(url.path())) {
    db.close();
    db = QSqlDatabase(); // otherwise warning that db is in use
    d->resetCache(url);
    QFile::remove(url.path());
    db = d->connectToDatabase(url);
  }

  MyMoneyStorageSql SQLReader(db);
  if (db.tables().isEmpty())
    SQLReader.initializeDatabase();

  SQLReader.writeFile(storage);

  auto logonData = d->readLogonData(url);
  if (logonData.first.isEmpty()) {
    if (isStorageFileBased)
      d->writeLogonDataForFile(url);
    else
      d->writeLogonDataForServer(url);
  }

  Url enhancedUrl(url);
  const auto storageType = enhancedUrl.decodeStorageType();
  if (isKMyMoneyType(storageType))
    convertStorage(url, eProducer::KMyMoney);
}

void Backend::upgradeStorage(const QUrl &url)
{
  Q_D(Backend);

  auto db = d->connectToDatabase(url);
  const auto storageProducer = d->storageProducer(url);
  Upgrader SqlUpgrader(d->m_callback);
  const auto storageVersion = d->storageVersion(url);

  SqlUpgrader.upgradeStorage(db, storageProducer, storageVersion);
}

QUrl Backend::convertStorage(const QUrl &url, eProducer targetProducer)
{
  Q_D(Backend);
  auto db = d->connectToDatabase(url);
  Converter sqlConverter(d->m_callback);
  sqlConverter.convertStorage(db, targetProducer);

  Url enhancedUrl(url);
  auto previousStorageType = enhancedUrl.decodeStorageType();
  eType newStorageType;
  if (targetProducer == eProducer::KMyMoney)
    newStorageType = kmmToKmmnTypeMap.key(previousStorageType);
  else
    newStorageType = kmmToKmmnTypeMap.value(previousStorageType);
  enhancedUrl.encodeStorageType(newStorageType);
  auto storageInfo = d->m_cache[url];
  storageInfo.producer = targetProducer;
  storageInfo.type = newStorageType;
  d->m_cache[enhancedUrl] = storageInfo;
  return std::move(enhancedUrl);
}

eType Backend::storageType(const QUrl &url)
{
  Q_D(Backend);

  if (d->m_cache.contains(url) && d->m_cache.value(url).type != eType::Unknown)
    return d->m_cache.value(url).type;

  eType storageType = eType::Unknown;
  eProducer storageProducer = eProducer::Unknown;
  auto db = QSqlDatabase();
  if (d->sqlite(db)->magicHeaderMatches(url)) {
    Url urlWithProbeType(url);
    urlWithProbeType.encodeStorageType(eType::kmm_SQLite);
    auto connectionName = d->uniqueConnectionName();
    db = d->connectToDatabaseFile(urlWithProbeType, connectionName);
    d->m_cache[urlWithProbeType].connectionName = db.connectionName();
    storageProducer = d->storageProducer(urlWithProbeType);
    db = QSqlDatabase();
    d->resetCache(urlWithProbeType);
    if (storageProducer == eProducer::KMyMoney)
      storageType = eType::kmm_SQLite;
    else if (storageProducer == eProducer::KMyMoneyNEXT)
      storageType = eType::kmmn_SQLite;
  }

  if (storageType != eType::Unknown) {
    d->m_cache[url].type = storageType; // cache that because that was expensive to infer
    if (storageProducer != eProducer::Unknown)
      d->m_cache[url].producer = storageProducer; // don't lose extra value

    return storageType;
  }

  Url enhancedUrl(url);
  storageType = enhancedUrl.decodeStorageType();

  return storageType;
}

QVector<eType> Backend::types() const
{
  Q_D(const Backend);
  return d->typesFiltered();
}

QVector<eType> Backend::canOpenTypes() const
{
  Q_D(const Backend);
  const QVector<eType> cannotOpenTypes {
    eType::kmm_PostgreSql,
    eType::kmmn_PostgreSql
  };
  return d->typesFiltered(cannotOpenTypes);
}

QVector<eType> Backend::canSaveTypes() const
{
  Q_D(const Backend);
  const QVector<eType> cannotSaveTypes {
    eType::kmm_PostgreSql,
    eType::kmmn_PostgreSql
  };
  return d->typesFiltered(cannotSaveTypes);
}

bool Backend::isKMyMoneyType(eType type) const
{
  if (kmmTypes.contains(type))
    return true;
  return false;
}

bool Backend::isStorageFileBased(const QUrl &url)
{
  const QVector<eType> fileBasedStorages {
    eType::kmm_SQLite,
    eType::kmm_SqlCipher,
    eType::kmmn_SQLite,
    eType::kmmn_SqlCipher
  };

  const auto storageType = Backend::storageType(url);
  if (fileBasedStorages.contains(storageType))
    return true;
  return false;
}

QString Backend::typeDisplayedName(eType storageType) const
{
  const QMap <eType, QString> typeDisplayedName {
    {eType::kmm_MySql,            i18n("KMyMoney MySQL")},
    {eType::kmm_PostgreSql,       i18n("KMyMoney PostgreSQL")},
    {eType::kmm_SQLite,           i18n("KMyMoney SQLite")},
    {eType::kmm_SqlCipher,        i18n("KMyMoney SQLCipher")},
    {eType::kmmn_MySql,           i18n("KMyMoneyNEXT MySQL")},
    {eType::kmmn_PostgreSql,      i18n("KMyMoneyNEXT PostgreSQL")},
    {eType::kmmn_SQLite,          i18n("KMyMoneyNEXT SQLite")},
    {eType::kmmn_SqlCipher,       i18n("KMyMoneyNEXT SQLCipher")}
  };
  return typeDisplayedName.value(storageType);
}

QString Backend::typeInternalName(eType storageType) const
{
  return Url::typeInternalName(storageType);
}

QVector<Issue> Backend::validateForOpen(const QUrl &url)
{
  Q_D(Backend);
  QVector<Issue> issues;

  Url enhancedUrl(url);
  const auto declaredStorageType = enhancedUrl.decodeStorageType();

  if (!canOpenTypes().contains(declaredStorageType)) {
    issues.append({eIssue::InvalidStorage, QStringList()});
    return issues;
  }

  const auto isStorageFileBased = Backend::isStorageFileBased(url);
  if (isStorageFileBased)
    issues.append(d->validateForOpenFile(url));
  else
    issues.append(d->checkIfServerIsRunning(url));

  if (!issues.isEmpty())
    return issues;

  issues.append(d->validateForAccess(url));
  if (!issues.isEmpty())
    return issues;

  if (!isStorageFileBased)
    issues.append(d->checkDatabaseExistForOpening(url));

  if (!issues.isEmpty())
    return issues;

  issues.append(d->validateForExclusivity(url));

  const auto storageProducer = d->storageProducer(url);
  const auto isKMyMoneyType = this->isKMyMoneyType(declaredStorageType);
  const auto isKMyMoneyProducer = storageProducer == eProducer::KMyMoney;
  if (isKMyMoneyType != isKMyMoneyProducer || storageProducer == eProducer::Unknown) {
    issues.append({eIssue::InvalidStorage, QStringList()});
    return issues;
  }

  const QString storageProducerString = isKMyMoneyType ? QStringLiteral("KMyMoney") : QStringLiteral("KMyMoneyNEXT");

  const auto currentVersionStatus = d->versionStatus(url);
  switch (currentVersionStatus) {
    case eVersionStatus::Current:
      break;
    case eVersionStatus::Newer:
      issues.clear();
      issues.append({eIssue::NeedsNewerSoftware, QStringList{storageProducerString}});
      return issues;
    case eVersionStatus::Older:
      issues.append({eIssue::NeedsUpgrade, QStringList{storageProducerString}});
      break;
    default:
      issues.clear();
      issues.append({eIssue::InvalidStorage, QStringList()});
      return issues;
  }

  if (isKMyMoneyType)
    issues.append({eIssue::NeedsConversion, QStringList{storageProducerString}});

  return issues;
}

QVector<Issue> Backend::validateForSave(const QUrl &url)
{
  Q_D(Backend);
  QVector<Issue> issues;

  Url enhancedUrl(url);
  const auto declaredStorageType = enhancedUrl.decodeStorageType();

  if (!canSaveTypes().contains(declaredStorageType)) {
    issues.append({eIssue::InvalidStorage, QStringList()});
    return issues;
  }

  const auto isStorageFileBased = Backend::isStorageFileBased(url);
  if (isStorageFileBased)
    issues.append(d->validateForSaveFile(url));
  else
    issues.append(d->checkIfServerIsRunning(url));

  if (!issues.isEmpty())
    return issues;

  switch (declaredStorageType) {
    case eType::kmm_SqlCipher:
    case eType::kmmn_SqlCipher:
      issues.append(d->validateForPassword(url));
      break;
    default:
      issues.append(d->validateForAccess(url));
  };

  if (!issues.isEmpty())
    return issues;

  if (!isStorageFileBased)
    issues.append(d->checkDatabaseExistForSaving(url));

  if (!issues.isEmpty())
    return issues;

  if (isKMyMoneyType(declaredStorageType))
    issues.append({eIssue::UnrecommendedType, QStringList{"KMyMoney"}});

  return issues;
}

bool Backend::createDatabase(const QUrl &url)
{
  Q_D(Backend);
  auto db = d->connectToDatabase(url);
  const auto storageType = Backend::storageType(url);

  switch (storageType) {
    case eType::kmm_MySql:
    case eType::kmmn_MySql:
      return d->mysql(db)->createDatabase(url);
    case eType::kmm_PostgreSql:
    case eType::kmmn_PostgreSql:
      return d->postgresql(db)->createDatabase(url);
    default:
      return false;
  }

}

void Backend::closeConnection(const QUrl &url)
{
  Q_D(Backend);
  d->writeLogonData(url, QPair<QString, QDateTime>{});
  d->resetCache(url);
}

} // namespace Sql
} // namespace Storage
