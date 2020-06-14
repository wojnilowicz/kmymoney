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

#ifndef STORAGE_XML_COMPARATOR_H
#define STORAGE_XML_COMPARATOR_H

class QString;
class QDomNodeList;
class QDomNamedNodeMap;
class QDomNode;

namespace Storage {
namespace Xml {
namespace Comparator {

  QString addSuffixToFileName(const QString &filePath, const QString &suffixToAdd);
  QString changeFileExtension(const QString &filePath, const QString &newExtension);
  bool areXmlAttributesEqual(const QString &firstAttributeValue, const QString &secondAttributeValue);
  bool areAttributesEqual(const QDomNamedNodeMap &firstAttributes, const QDomNamedNodeMap &secondAttributes);
  bool areChildNodesEqual(QDomNodeList &firstNodes, QDomNodeList &secondNodes);
  bool areNodesEqual(QDomNode &firstNode, QDomNode &secondNode);
  bool areStoragesEqual(const QString &firstFilePath, const QString &secondFilePath);

} // namespace Comparator
} // namespace Xml
} // namespace Storage

#endif
