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

#include "storagesqlconverter-test.h"

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
#include "../converter.h"
#include "storage/interface/enums.h"

QTEST_GUILESS_MAIN(Storage::Sql::ConverterTest)

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

void testFileConversion(const QString &originalFilePath, const QString &desiredFilePath, eProducer targetProducer)
{
  Converter sqlConverter;

  const QString convertedFilePath = Comparator::addSuffixToFileName(desiredFilePath, QStringLiteral("-converted"));
  if (QFile::exists(convertedFilePath))
    QFile::remove(convertedFilePath);
  QFile::copy(originalFilePath, convertedFilePath);

  bool areStoragesEqual = false;
  { // this scope prevents "QSqlDatabasePrivate::removeDatabase: connection <name> is still in use, all queries will cease to work." in disconnectDatabases()
  auto databases = connectDatabases(convertedFilePath, desiredFilePath);

  auto &upgradedDb = databases.first;
  auto &desiredDb = databases.second;

  sqlConverter.convertStorage(upgradedDb, targetProducer);

  Comparator comparator;

  areStoragesEqual = comparator.areStoragesEqual(upgradedDb, desiredDb);
  if (!areStoragesEqual)
    comparator.saveDiffsToFile(convertedFilePath, desiredFilePath);
  }

  disconnectDatabases();
  if (!areStoragesEqual)
    QFAIL("Storages are not equal.");
}

const QString KMyMoneyFilesDirectory = QStringLiteral("./sqlFiles/KMyMoney/latest/");
const QString KMyMoneyNEXTFilesDirectory = QStringLiteral("./sqlFiles/KMyMoneyNEXT/latest/");

void ConverterTest::convertToKMyMoney()
{
  const QString originalFilePath = KMyMoneyNEXTFilesDirectory + QStringLiteral("basicFile.db");
  const QString desiredFilePath = KMyMoneyFilesDirectory + QStringLiteral("basicFile.db");

  testFileConversion(originalFilePath, desiredFilePath, eProducer::KMyMoney);
}

void ConverterTest::convertToKMyMoneyNEXT()
{
  const QString originalFilePath = KMyMoneyFilesDirectory + QStringLiteral("basicFile.db");
  const QString desiredFilePath = KMyMoneyNEXTFilesDirectory + QStringLiteral("basicFile.db");

//  testFileConversion(originalFilePath, desiredFilePath, eProducer::KMyMoneyNEXT);
}

} // namespace Sql
} // namespace Storage
