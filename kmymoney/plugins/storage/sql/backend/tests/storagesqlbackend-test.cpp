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

#include "storagesqlbackend-test.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QtTest>
#include <QFileInfo>

// ----------------------------------------------------------------------------
// KDE Includes


// ----------------------------------------------------------------------------
// Project Includes

#include "../backend.cpp"
#include "storage/interface/enums.h"

QTEST_GUILESS_MAIN(Storage::Sql::BackendTest)

namespace Storage {
namespace Sql {

const QString KMMNFilesDirectory = QStringLiteral("./sqlFiles/KMyMoneyNEXT/2020-01-01/");
const QString KMMFilesDirectory = QStringLiteral("./sqlFiles/KMyMoney/2019-02-04/");

void BackendTest::storageType()
{
  Backend backend;

  auto storageUrl = QUrl::fromLocalFile(KMMNFilesDirectory + QStringLiteral("basicFile.db"));
  QCOMPARE(backend.storageType(storageUrl), eType::kmmn_SQLite);
  backend.d_ptr->resetCache(storageUrl);

  storageUrl = QUrl::fromLocalFile(KMMFilesDirectory + QStringLiteral("basicFile.db"));
  QCOMPARE(backend.storageType(storageUrl), eType::kmm_SQLite);
  backend.d_ptr->resetCache(storageUrl);
}

void BackendTest::storageProducer()
{
  Backend backend;

  auto storageUrl = QUrl::fromLocalFile(KMMNFilesDirectory + QStringLiteral("basicFile.db"));
  QCOMPARE(backend.d_ptr->storageProducer(storageUrl), eProducer::KMyMoneyNEXT);
  backend.d_ptr->resetCache(storageUrl);

  storageUrl = QUrl::fromLocalFile(KMMFilesDirectory + QStringLiteral("basicFile.db"));
  QCOMPARE(backend.d_ptr->storageProducer(storageUrl), eProducer::KMyMoney);
  backend.d_ptr->resetCache(storageUrl);
}

void BackendTest::storageVersion()
{
  Backend backend;

  auto storageUrl = QUrl::fromLocalFile(KMMNFilesDirectory + QStringLiteral("basicFile.db"));
  QCOMPARE(backend.d_ptr->storageVersion(storageUrl), 20200101);
  backend.d_ptr->resetCache(storageUrl);

  storageUrl = QUrl::fromLocalFile(KMMFilesDirectory + QStringLiteral("basicFile.db"));
  QCOMPARE(backend.d_ptr->storageVersion(storageUrl), 12005);
  backend.d_ptr->resetCache(storageUrl);
}

void BackendTest::storageVersionStatus()
{
  Backend backend;

  auto storageUrl = QUrl::fromLocalFile(KMMNFilesDirectory + QStringLiteral("basicFile.db"));
  QCOMPARE(backend.d_ptr->versionStatus(storageUrl), eVersionStatus::Current);
  backend.d_ptr->resetCache(storageUrl);

  storageUrl = QUrl::fromLocalFile(KMMFilesDirectory + QStringLiteral("basicFile.db"));
  QCOMPARE(backend.d_ptr->versionStatus(storageUrl), eVersionStatus::Current);
  backend.d_ptr->resetCache(storageUrl);
}

void BackendTest::cleanup()
{
//  if (QSqlDatabase::contains(myConnectionName))
//    QSqlDatabase::removeDatabase(myConnectionName);
}

} // namespace Sql
} // namespace Storage
