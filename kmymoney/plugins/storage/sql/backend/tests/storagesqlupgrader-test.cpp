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

#include "storagesqlupgrader-test.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QtTest>
#include <QSqlDatabase>
#include <QSqlQuery>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "comparator.h"
#include "../upgrader.h"
#include "storage/interface/enums.h"

QTEST_GUILESS_MAIN(Storage::Sql::UpgraderTest)

const QString firstConnectionName = QStringLiteral("firstConnectionName");
const QString secondConnectionName = QStringLiteral("secondConnectionName");

namespace Storage {
namespace Sql {

auto connectDatabases(const QString &firstPath, const QString &secondPath)
{
  const QString sqlDriverName = QStringLiteral("QSQLITE");
  auto firstDb = QSqlDatabase::addDatabase(sqlDriverName, firstConnectionName);
  firstDb.setDatabaseName(firstPath);

  auto secondDb = QSqlDatabase::addDatabase(sqlDriverName, secondConnectionName);
  secondDb.setDatabaseName(secondPath);

  if (!firstDb.open() || !secondDb.open())
    throw std::exception();
  return QPair<QSqlDatabase, QSqlDatabase> {firstDb, secondDb};
}

void disconnectDatabases()
{
  QSqlDatabase::removeDatabase(firstConnectionName);
  QSqlDatabase::removeDatabase(secondConnectionName);
}

void testFileUpgrade(const QString &originalFilePath, const QString &desiredFilePath, eProducer storageProducer, unsigned int sourceVersion, unsigned int targetVersion)
{
  Upgrader sqlUpgrader;

  const QString upgradedFilePath = Comparator::addSuffixToFileName(desiredFilePath, QStringLiteral("-upgraded"));
  if (QFile::exists(upgradedFilePath))
    QFile::remove(upgradedFilePath);
  QFile::copy(originalFilePath, upgradedFilePath);

  bool areStoragesEqual = false;
  { // this scope prevents "QSqlDatabasePrivate::removeDatabase: connection <name> is still in use, all queries will cease to work." in disconnectDatabases()
  auto databases = connectDatabases(upgradedFilePath, desiredFilePath);

  auto &upgradedDb = databases.first;
  auto &desiredDb = databases.second;

  sqlUpgrader.upgradeStorage(upgradedDb, storageProducer, sourceVersion, targetVersion);

  Comparator comparator;

  areStoragesEqual = comparator.areStoragesEqual(upgradedDb, desiredDb);
  if (!areStoragesEqual)
    comparator.saveDiffsToFile(upgradedFilePath, desiredFilePath);
  }

  disconnectDatabases();
  if (!areStoragesEqual)
    QFAIL("Storages are not equal.");
}

const QString KMyMoneyFilesDirectory = QStringLiteral("./sqlFiles/KMyMoney/");

//fd6a9557c637875aff7e53415f7c042c8f636858
// v10 -> v11 2017-09-14

// commit: ee24bbff8e72d3af6a9bddf365509eb9dcc02efb
// date: 2017-11-05
void UpgraderTest::upgradeFrom11004()
{
  const QString originalFilePath = KMyMoneyFilesDirectory + QStringLiteral("2017-11-05/basicFile.db");
  const QString desiredFilePath = KMyMoneyFilesDirectory + QStringLiteral("2017-11-06/basicFile.db");

  testFileUpgrade(originalFilePath, desiredFilePath, eProducer::KMyMoney, 11004, 12004);

}

// commit: 2fe372e97b012442f6f5be462ee23ebfcd19a5ab
// date: 2019-02-03
void UpgraderTest::upgradeFrom12004()
{
  const QString originalFilePath = KMyMoneyFilesDirectory + QStringLiteral("2019-02-03/basicMetals.db");
  const QString desiredFilePath = KMyMoneyFilesDirectory + QStringLiteral("2019-02-04/basicMetals.db");

  testFileUpgrade(originalFilePath, desiredFilePath, eProducer::KMyMoney, 12004, 12005);
}

} // namespace Sql
} // namespace Storage
