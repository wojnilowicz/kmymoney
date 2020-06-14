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

#include "comparator.h"

#include <memory>

// ----------------------------------------------------------------------------
// QT Includes

#include <QFileInfo>
#include <QDomDocument>
#include <QDebug>

// ----------------------------------------------------------------------------
// KDE Includes

#include <KCompressionDevice>

// ----------------------------------------------------------------------------
// Project Includes

#include "storage/interface/enums.h"

namespace Storage {
namespace Xml {
namespace Comparator {

QString addSuffixToFileName(const QString &filePath, const QString &suffixToAdd)
{
  const auto fileInfo = QFileInfo(filePath);
  const auto oldFilePath = fileInfo.path();
  const auto oldBaseName = fileInfo.baseName();
  const auto oldExtension = fileInfo.completeSuffix();
  return QString::fromLatin1("%1/%2%3.%4").arg(oldFilePath).arg(oldBaseName).arg(suffixToAdd).arg(oldExtension);
}

QString changeFileExtension(const QString &filePath, const QString &newExtension)
{
  const auto fileInfo = QFileInfo(filePath);
  const auto oldFilePath = fileInfo.path();
  const auto oldBaseName = fileInfo.baseName();
  const auto oldExtension = fileInfo.completeSuffix();
  return QString::fromLatin1("%1/%2.%4").arg(oldFilePath).arg(oldBaseName).arg(newExtension);
}

bool areXmlAttributesEqual(const QString &firstAttributeValue, const QString &secondAttributeValue)
{
  // in case of kmm-matched-tx which contained embedded xml document
  QString documentStartString;
  const QVector<QString> possibleDocumentStartStrings {
    QStringLiteral("&lt;"), QStringLiteral("&#60;")
  };
  for (const auto &possibleDocumentStartString : possibleDocumentStartStrings) {
    if (firstAttributeValue.startsWith(possibleDocumentStartString)) {
      documentStartString = possibleDocumentStartString;
      break;
    }
  }

  if (documentStartString.isEmpty() ||
      !secondAttributeValue.startsWith(documentStartString))
    return false;

  QDomDocument firstDoc;
  QDomDocument secondDoc;

  auto firstAttributeValueXml = firstAttributeValue;
  auto secondAttributeValueXml = secondAttributeValue;
  firstAttributeValueXml.replace(documentStartString, QLatin1String("<"));
  secondAttributeValueXml.replace(documentStartString, QLatin1String("<"));

  firstDoc.setContent(firstAttributeValueXml);
  secondDoc.setContent(secondAttributeValueXml);

  auto firstNode = firstDoc.documentElement();
  auto secondNode = secondDoc.documentElement();

  auto areDocumentsInAttributesEqual = areNodesEqual(firstNode, secondNode);
  if (!areDocumentsInAttributesEqual) {
    qDebug() << "XML documents in attributes differ";
    qDebug() << "first document:  " << firstDoc.toString();
    qDebug() << "second document: " << secondDoc.toString();
  }
  return areDocumentsInAttributesEqual;
}

bool areAttributesEqual(const QDomNamedNodeMap &firstAttributes, const QDomNamedNodeMap &secondAttributes)
{
  auto firstAttributesCount = firstAttributes.count();
  auto secondAttributesCount = secondAttributes.count();

  if (firstAttributesCount != secondAttributesCount)
    return false;

  for (auto i = 0; i < firstAttributesCount; ++i) {
    auto firstAttribute = firstAttributes.item(i);
    auto firstAttributeName = firstAttribute.nodeName();
    auto firstAttributeValue = firstAttribute.nodeValue();

    if (!secondAttributes.contains(firstAttributeName))
      return false;

    auto secondAttribute = secondAttributes.namedItem(firstAttributeName);
    auto secondAttributeValue = secondAttribute.nodeValue();
    if (firstAttributeValue != secondAttributeValue)
      return areXmlAttributesEqual(firstAttributeValue, secondAttributeValue);
  }

  return true;
}

bool areChildNodesEqual(QDomNodeList &firstNodes, QDomNodeList &secondNodes)
{
  auto areChildNodesEqual = true;
  if (firstNodes.count() != secondNodes.count())
    areChildNodesEqual = false;

  for (auto i = 0; i < firstNodes.count(); ++i) {
    auto firstNodesdNode = firstNodes.at(i);
    auto foundEqualSecondNodesNode = false;
    for (auto j = 0; j < secondNodes.count(); ++j) {
      auto secondNodesdNode = secondNodes.at(j);
      if (firstNodesdNode.nodeName() != secondNodesdNode.nodeName() ||
          !areNodesEqual(firstNodesdNode, secondNodesdNode)) {
        continue;
      }

      secondNodesdNode.parentNode().removeChild(secondNodesdNode);
      firstNodesdNode.parentNode().removeChild(firstNodesdNode);
      i--;
      foundEqualSecondNodesNode = true;
      break;
    }

    if (!foundEqualSecondNodesNode)
      areChildNodesEqual = false;
  }
  return areChildNodesEqual;
}

bool areNodesEqual(QDomNode &firstNode, QDomNode &secondNode)
{
  if (firstNode.nodeName() != secondNode.nodeName())
    return false;

  if (!areAttributesEqual(firstNode.attributes(), secondNode.attributes()))
    return false;

  auto firstChildNodes = firstNode.childNodes();
  auto secondChildNodes = secondNode.childNodes();

  if (!firstChildNodes.isEmpty() &&
      !secondChildNodes.isEmpty() &&
      !areChildNodesEqual(firstChildNodes, secondChildNodes))
    return false;

  return true;
}

bool areStoragesEqual(const QString &firstFilePath, const QString &secondFilePath)
{
  const auto firstFile = std::make_unique<KCompressionDevice>(firstFilePath, KCompressionDevice::GZip);
  const auto secondFile = std::make_unique<KCompressionDevice>(secondFilePath, KCompressionDevice::GZip);

  firstFile->open(QIODevice::ReadOnly);
  secondFile->open(QIODevice::ReadOnly);

  const auto firstFileContent = firstFile->readAll();
  const auto secondFileContent = secondFile->readAll();

  QDomDocument firstDoc;
  QDomDocument secondDoc;

  firstDoc.setContent(firstFileContent);
  secondDoc.setContent(secondFileContent);

  auto firstDocChildNodes = firstDoc.childNodes();
  auto secondDocChildNodes = secondDoc.childNodes();

  auto areStoragesEqual = areChildNodesEqual(firstDocChildNodes, secondDocChildNodes);
  if (!areStoragesEqual) {
    const auto firstFileDiffFilePath = changeFileExtension(firstFilePath, QStringLiteral("diff"));
    const auto secondFileDiffFilePath = changeFileExtension(secondFilePath, QStringLiteral("diff"));

    const auto firstFileDiff = std::make_unique<QFile>(firstFileDiffFilePath);
    const auto secondFileDiff = std::make_unique<QFile>(secondFileDiffFilePath);

    firstFileDiff->open(QIODevice::WriteOnly);
    secondFileDiff->open(QIODevice::WriteOnly);

    firstFileDiff->write(firstDoc.toByteArray());
    secondFileDiff->write(secondDoc.toByteArray());
  }

  return areStoragesEqual;
}

} // namespace Comparator
} // namespace Xml
} // namespace Storage
