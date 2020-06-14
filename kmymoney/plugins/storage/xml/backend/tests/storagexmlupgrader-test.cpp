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

#include "storagexmlupgrader-test.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QtTest>
#include <QRegularExpression>

// ----------------------------------------------------------------------------
// KDE Includes

#include <KCompressionDevice>

// ----------------------------------------------------------------------------
// Project Includes

#include "comparator.h"
#include "../upgrader.h"
#include "storage/interface/enums.h"

QTEST_GUILESS_MAIN(Storage::Xml::UpgraderTest)

namespace Storage {
namespace Xml {

void testFileUpgrade(const QString &originalFilePath, const QString &desiredFilePath, eProducer storageProducer, unsigned int sourceVersion, unsigned int targetVersion)
{
  Upgrader xmlUpgrader;
  const QString upgradedFilePath = Comparator::addSuffixToFileName(desiredFilePath, QStringLiteral("-upgraded"));

  const auto originalFileHandle = std::make_unique<KCompressionDevice>(originalFilePath, KCompressionDevice::GZip);
  const auto upgradedFileHandle = std::make_unique<KCompressionDevice>(upgradedFilePath, KCompressionDevice::GZip);

  originalFileHandle->open(QIODevice::ReadOnly);
  upgradedFileHandle->open(QIODevice::WriteOnly);

  const auto originalContent = originalFileHandle->readAll();

  const auto upgradedContent = xmlUpgrader.upgradeStorage(originalContent, storageProducer, sourceVersion, targetVersion);
  upgradedFileHandle->write(upgradedContent);
  originalFileHandle->close();
  upgradedFileHandle->close();
}

const QString KMyMoneyFilesDirectory = QStringLiteral("./xmlFiles/KMyMoney/");

void UpgraderTest::upgradeFrom1003()
{
  const QString originalFilePath = KMyMoneyFilesDirectory + QStringLiteral("2011-08-02/basicFile.kmy");
  const QString desiredFilePath = KMyMoneyFilesDirectory + QStringLiteral("2011-08-03/basicFile.kmy");

  testFileUpgrade(originalFilePath, desiredFilePath, eProducer::KMyMoney, 1003, 1004);

  const QString upgradedFilePath = Comparator::addSuffixToFileName(desiredFilePath, QStringLiteral("-upgraded"));

  const auto upgradedFileHandle = std::make_unique<KCompressionDevice>(upgradedFilePath, KCompressionDevice::GZip);
  upgradedFileHandle->open(QIODevice::ReadOnly);
  auto upgradedContent = upgradedFileHandle->readAll();
  upgradedFileHandle->close();
  QRegularExpression re("value=\"(?P<uuid>{[-a-z0-9]*})\"");
  auto match = re.match(upgradedContent);
  if (match.hasMatch())
    upgradedContent.replace(match.captured("uuid").toUtf8().constData(), "{5cfd1dc1-da19-4559-a5b8-8045f684ff81}");

  upgradedFileHandle->open(QIODevice::WriteOnly);
  upgradedFileHandle->write(upgradedContent);
  upgradedFileHandle->close();

  QVERIFY(Comparator::areStoragesEqual(desiredFilePath, upgradedFilePath));
}

void UpgraderTest::upgradeFrom1004()
{
  const QString originalFilePath = KMyMoneyFilesDirectory + QStringLiteral("2019-02-03/basicMetals.kmy");
  const QString desiredFilePath = KMyMoneyFilesDirectory + QStringLiteral("2019-02-04/basicMetals.kmy");
  const QString upgradedFilePath = Comparator::addSuffixToFileName(desiredFilePath, QStringLiteral("-upgraded"));

  testFileUpgrade(originalFilePath, desiredFilePath, eProducer::KMyMoney, 1004, 1005);
  QVERIFY(Comparator::areStoragesEqual(desiredFilePath, upgradedFilePath));
}

} // namespace Xml
} // namespace Storage
