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

#include "domhelper.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QDomDocument>
#include <QDomImplementation>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "mymoney/mymoneyexception.h"
#include "storage/interface/enums.h"

namespace Storage {
namespace Xml {
namespace DomHelper {

void setTagIdValueInFileInfoNode(QDomDocument &doc, const QString &tagName, const QString &idValue)
{
  auto fileInfoNodes = doc.elementsByTagName(QStringLiteral("FILEINFO"));
  if (Q_UNLIKELY(fileInfoNodes.isEmpty()))
    throw MYMONEYEXCEPTION_CSTRING("No FILEINFO found.");

  auto fileInfoElement = fileInfoNodes.at(0).toElement();

  // remove prvious tag because it can contain undesired attributes
  auto nodes = fileInfoElement.elementsByTagName(tagName);
  if (!nodes.isEmpty())
    fileInfoElement.removeChild(nodes.at(0));

  // if value to be set is empty then don't event create a tag for it
  if (idValue.isEmpty())
    return;

  auto node = doc.createElement(tagName);
  node.setAttribute(QStringLiteral("id"), idValue);
  fileInfoElement.appendChild(node);
}

} // namespace DomHelper
} // namespace Xml
} // namespace Storage
