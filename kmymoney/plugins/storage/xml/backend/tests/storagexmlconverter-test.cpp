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

#include "storagexmlconverter-test.h"

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
#include "../converter.h"
#include "storage/interface/enums.h"

QTEST_GUILESS_MAIN(Storage::Xml::ConverterTest)

namespace Storage {
namespace Xml {

void testFileConversion(const QString &originalFilePath, const QString &desiredFilePath, eProducer targetProducer)
{
  Converter xmlConverter;
  Upgrader xmlUpgrader;
  const QString convertedFilePath = Comparator::addSuffixToFileName(desiredFilePath, QStringLiteral("-converted"));
  const QString upgradedDesiredFilePath = Comparator::addSuffixToFileName(desiredFilePath, QStringLiteral("-upgraded"));

  const auto originalFileHandle = std::make_unique<KCompressionDevice>(originalFilePath, KCompressionDevice::GZip);
  const auto convertedFileHandle = std::make_unique<KCompressionDevice>(convertedFilePath, KCompressionDevice::GZip);

  originalFileHandle->open(QIODevice::ReadOnly);
  convertedFileHandle->open(QIODevice::WriteOnly);

  const auto originalContent = originalFileHandle->readAll();
  const auto convertedContent = xmlConverter.convertStorage(originalContent, targetProducer);

  convertedFileHandle->write(convertedContent);
  originalFileHandle->close();
  convertedFileHandle->close();
  QVERIFY(Comparator::areStoragesEqual(desiredFilePath, convertedFilePath));
}

const QString KMyMoneyFilesDirectory = QStringLiteral("./xmlFiles/KMyMoney/latest/");
const QString KMyMoneyNEXTFilesDirectory = QStringLiteral("./xmlFiles/KMyMoneyNEXT/latest/");

void ConverterTest::convertToKMyMoney()
{
  const QString originalFilePath = KMyMoneyNEXTFilesDirectory + QStringLiteral("basicFile.kmy");
  const QString desiredFilePath = KMyMoneyFilesDirectory + QStringLiteral("basicFile.kmy");

  testFileConversion(originalFilePath, desiredFilePath, eProducer::KMyMoney);
}

void ConverterTest::convertToKMyMoneyNEXT()
{
  const QString originalFilePath = KMyMoneyFilesDirectory + QStringLiteral("basicFile.kmy");
  const QString desiredFilePath = KMyMoneyNEXTFilesDirectory + QStringLiteral("basicFile.kmy");

  testFileConversion(originalFilePath, desiredFilePath, eProducer::KMyMoneyNEXT);
}

} // namespace Xml
} // namespace Storage
