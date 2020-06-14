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

#include "storagexmlbackend-test.h"

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

QTEST_GUILESS_MAIN(Storage::Xml::BackendTest)

namespace Storage {
namespace Xml {

const QString KMMNFilesDirectory = QStringLiteral("./xmlFiles/KMyMoneyNEXT/2020-01-01/");
const QString KMMFilesDirectory = QStringLiteral("./xmlFiles/KMyMoney/2019-02-04/");

void BackendTest::storageType()
{
  Backend backend;

  auto storageUrl = QUrl::fromLocalFile(KMMNFilesDirectory + QStringLiteral("basicFile.kmy"));
  QCOMPARE(backend.storageType(storageUrl), eType::kmmn_XmlGzip);
}

void BackendTest::storageProducer()
{
  Backend backend;

  auto storageUrl = QUrl::fromLocalFile(KMMNFilesDirectory + QStringLiteral("basicFile.kmy"));
  QCOMPARE(backend.d_ptr->docType(storageUrl), eProducer::KMyMoneyNEXT);

  storageUrl = QUrl::fromLocalFile(KMMFilesDirectory + QStringLiteral("basicFile.kmy"));
  QCOMPARE(backend.d_ptr->docType(storageUrl), eProducer::KMyMoney);
}

void BackendTest::storageVersion()
{
  Backend backend;

  auto storageUrl = QUrl::fromLocalFile(KMMNFilesDirectory + QStringLiteral("basicFile.kmy"));
  QCOMPARE(backend.d_ptr->storageVersion(storageUrl), 20200101);

  storageUrl = QUrl::fromLocalFile(KMMFilesDirectory + QStringLiteral("basicFile.kmy"));
  QCOMPARE(backend.d_ptr->storageVersion(storageUrl), 1005);

  storageUrl = QUrl::fromLocalFile(KMMFilesDirectory + QStringLiteral("versionsReversed.kmy"));
  QCOMPARE(backend.d_ptr->storageVersion(storageUrl), 1005);
}

void BackendTest::storageVersionStatus()
{
  Backend backend;

  auto storageUrl = QUrl::fromLocalFile(KMMNFilesDirectory + QStringLiteral("basicFile.kmy"));
  QCOMPARE(backend.d_ptr->versionStatus(storageUrl), eVersionStatus::Current);

  storageUrl = QUrl::fromLocalFile(KMMFilesDirectory + QStringLiteral("basicFile.kmy"));
  QCOMPARE(backend.d_ptr->versionStatus(storageUrl), eVersionStatus::Current);
}

} // namespace Xml
} // namespace Storage
