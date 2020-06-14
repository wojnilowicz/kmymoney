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

#ifndef STORAGE_XML_BACKEND_URL_H
#define STORAGE_XML_BACKEND_URL_H

// ----------------------------------------------------------------------------
// QT Includes

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "storage/interface/iurl.h"

namespace Storage {

enum class eType;

namespace Xml {

class Url : public IUrl
{
public:
  Url(const QUrl &url);

  static QString pluginScheme();
  void setPluginScheme();

  eType decodeStorageType() const;
  void encodeStorageType(eType type);

  QStringList encryptionKeys() const;
  void setEncryptionKeys(const QStringList &keyIDs);

  static QString typeInternalName(eType storageType);

};

} // namespace Xml
} // namespace Storage

#endif
