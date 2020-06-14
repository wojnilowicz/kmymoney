/*
 * Copyright 2019       Łukasz Wojniłowicz <lukasz.wojnilowicz@gmail.com>
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

#ifndef STORAGE_INTERFACE_IURL_H
#define STORAGE_INTERFACE_IURL_H

#include <kmm_istorage_export.h>

// ----------------------------------------------------------------------------
// QT Includes

#include <QUrl>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

namespace Storage {

enum class eUrl {
  Plugin,
  Type,
  NoSwitch
};

enum class eType;

class KMM_ISTORAGE_EXPORT IUrl : public QUrl
{
public:
  IUrl(const QUrl &url);

  QString pluginScheme() const;
  void setPluginScheme(const QString &name);

  QString storageType() const;

  bool noSwitch() const;
  void setNoSwitch();

  static QUrl consistentUrlForStoring(const QUrl &url);

protected:
  static QString keyName(eUrl key);
};

/* URL schemas for reading/saving/editing storages:
 * 1. URL for file-based storage: xml:/path/to/file
 * 2. URL for sql-based storage: sql:/path/to/file or sql:/host/databaseName
 * 3. URL for file-based storage with query field "type": xml:/path/to/file?type=xml-gzip
 * 4. Handled query fields
 *    type=xml-gzip
 *    keys=FBEF708DD0AA691B;556DFE5CBAF4A563
 *    noswitch=true
*/

} // namespace Storage

#endif
