/*
 * Copyright 2018-2019  Łukasz Wojniłowicz <lukasz.wojnilowicz@gmail.com>
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

#include "readerwriterhelper.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QFileInfo>
#include <QBuffer>
#include <QUrl>

// ----------------------------------------------------------------------------
// KDE Includes

#include <KCompressionDevice>

// ----------------------------------------------------------------------------
// Project Includes

#include "mymoney/mymoneyfile.h"
#include "mymoney/mymoneyexception.h"

#include "../readerwriter.cpp"
#include "../upgrader.h"
#include "../backend.h"
#include "../url.h"
#include "storage/interface/enums.h"

namespace Storage {
namespace Xml {

MyMoneyFile *readFile(const QString& filePath)
{
 Backend backend;
 const auto storageUrl = QUrl::fromLocalFile(filePath);
 backend.upgradeStorage(storageUrl);
 backend.openStorage(*MyMoneyFile::instance(), storageUrl);
 return MyMoneyFile::instance();
}

void writeFile(const QString& filePath, MyMoneyFile* storage)
{
  const auto storageUrl = QUrl::fromLocalFile(filePath);
  Url enhancedStorageUrl(storageUrl);
  enhancedStorageUrl.encodeStorageType(eType::kmmn_XmlGzip);
  Backend backend;
  backend.saveStorage(*storage, enhancedStorageUrl);
}

void ReaderWriterHelper::testFile(const QString& filePath, std::function<void(const MyMoneyFile * const)> testStorage)
{
  const auto originalFileInfo = QFileInfo(filePath);
  const auto originalFileBaseName = originalFileInfo.completeBaseName();
  const auto copyFileInfo = QFileInfo(originalFileInfo.filePath().replace(originalFileBaseName, originalFileBaseName + QLatin1String("-copy")));

  // read test file...
  const auto originalFileStorage = readFile(originalFileInfo.filePath());
  // ...and check by external function if it has all expected values
  testStorage(originalFileStorage);

  // write test file...
  writeFile(copyFileInfo.filePath(), originalFileStorage);
  // ...read it again...
  const auto copyFileStorage = readFile(copyFileInfo.filePath());
  // ...and check it again (it is for testing results of writing functions)
  testStorage(copyFileStorage);
}

QString ReaderWriterHelper::configureFile(const QString& originalFilePath, const QVector<QVector<QString>>& replacementDatas)
{
  const auto originalFileInfo = QFileInfo(originalFilePath);
  const auto originalFileBaseName = originalFileInfo.completeBaseName();
  const auto configuredFileInfo = QFileInfo(originalFileInfo.filePath().replace(originalFileBaseName, originalFileBaseName + QLatin1String("-configured")));

  KCompressionDevice originalKmyFile(originalFileInfo.filePath(), KCompressionDevice::GZip);
  originalKmyFile.open(QIODevice::ReadOnly);
  auto kmyFileContent = originalKmyFile.readAll();
  originalKmyFile.close();

  for (const auto &replacementData: replacementDatas)
    kmyFileContent.replace(qPrintable(QString::fromLatin1("%1=\"%2\"").arg(replacementData[0], replacementData[1])),
                           qPrintable(QString::fromLatin1("%1=\"%2\"").arg(replacementData[0], replacementData[2])));

  KCompressionDevice configuredKmyFile(configuredFileInfo.filePath(), KCompressionDevice::GZip);
  configuredKmyFile.open(QIODevice::WriteOnly);
  configuredKmyFile.write(kmyFileContent);
  configuredKmyFile.close();

  return configuredFileInfo.filePath();
}

} // namespace Xml
} // namespace Storage
