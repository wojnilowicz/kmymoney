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

#include "xmlfiletesthelper.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QFileInfo>
#include <QBuffer>

// ----------------------------------------------------------------------------
// KDE Includes

#include <KCompressionDevice>

// ----------------------------------------------------------------------------
// Project Includes

#include "../mymoneystoragexml.cpp"
#include "../xmlstorageupdater.h"
#include "mymoneyexception.h"

std::unique_ptr<MyMoneyStorageMgr> XMLFileTestHelper::readFile(const QString& filePath)
{
  std::unique_ptr<QIODevice> qfile = std::make_unique<KCompressionDevice>(filePath, KCompressionDevice::GZip);
  if (!qfile->open(QIODevice::ReadOnly))
    throw MYMONEYEXCEPTION_CSTRING("The file cannot be read.");

  auto XMLByteArray = qfile->readAll();
  qfile->close();
  qfile.reset();
  qfile = std::make_unique<QBuffer>(&XMLByteArray);
  qfile->open(QIODevice::ReadWrite);

  if (!XMLStorageUpdater::updateFile(qfile.get()))
    throw MYMONEYEXCEPTION_CSTRING("The file cannot be updated.");
  qfile->seek(0);

  auto storage = std::make_unique<MyMoneyStorageMgr>();
  MyMoneyStorageXML pReader;
  pReader.readFile(qfile.get(), storage.get());
  qfile->close();
  return storage;
}

void XMLFileTestHelper::writeFile(const QString& filePath, MyMoneyStorageMgr* storage)
{
  const auto qfile = std::make_unique<KCompressionDevice>(filePath, KCompressionDevice::GZip);
  if (!qfile->open(QIODevice::WriteOnly))
    return;

  MyMoneyStorageXML pWriter;
  pWriter.writeFile(qfile.get(), storage);
  qfile->close();
}

void XMLFileTestHelper::testFile(const QString& filePath, std::function<void(const MyMoneyStorageMgr * const)> testStorage)
{
  const auto originalFileInfo = QFileInfo(filePath);
  const auto originalFileBaseName = originalFileInfo.completeBaseName();
  const auto copyFileInfo = QFileInfo(originalFileInfo.filePath().replace(originalFileBaseName, originalFileBaseName + QLatin1String("-copy")));

  // read test file...
  const auto originalFileStorage = readFile(originalFileInfo.filePath());
  // ...and check by external function if it has all expected values
  testStorage(originalFileStorage.get());

  // write test file...
  writeFile(copyFileInfo.filePath(), originalFileStorage.get());
  // ...read it again...
  const auto copyFileStorage = readFile(copyFileInfo.filePath());
  // ...and check it again (it is for testing results of writing functions)
  testStorage(copyFileStorage.get());
}

QString XMLFileTestHelper::configureFile(const QString& originalFilePath, const QVector<QVector<QString>>& replacementDatas)
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
