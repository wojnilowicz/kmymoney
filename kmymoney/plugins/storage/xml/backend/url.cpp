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

#include "url.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QMap>
#include <QUrlQuery>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "storage/interface/enums.h"

namespace Storage {
namespace Xml {

static const QString keysName = QStringLiteral("keys");

static const QMap <eType, QString> typeValues {
  {eType::kmm_XmlGzip,       QStringLiteral("kmm-xml-gzip")},
  {eType::kmm_XmlGpg,        QStringLiteral("kmm-xml-gpg")},
  {eType::kmm_XmlPlain,      QStringLiteral("kmm-xml-plain")},
  {eType::kmm_XmlAnonymous,  QStringLiteral("kmm-xml-anonymous")},
  {eType::kmmn_XmlGzip,      QStringLiteral("kmmn-xml-gzip")},
  {eType::kmmn_XmlGpg,       QStringLiteral("kmmn-xml-gpg")},
  {eType::kmmn_XmlPlain,     QStringLiteral("kmmn-xml-plain")},
  {eType::kmmn_XmlAnonymous, QStringLiteral("kmmn-xml-anonymous")},
};

static const QString schemeString = QStringLiteral("xml");

Url::Url(const QUrl &url) :
  IUrl(url)
{
}

QString Url::pluginScheme()
{
  return schemeString;
}

void Url::setPluginScheme()
{
  IUrl::setPluginScheme(schemeString);
}

eType Url::decodeStorageType() const
{
  const auto query = QUrlQuery(*this);
  const auto typeInternalName = query.queryItemValue(keyName(eUrl::Type));
  return typeValues.key(typeInternalName, eType::Unknown);
}

void Url::encodeStorageType(eType type)
{
  auto query = QUrlQuery(*this);
  auto typeName = typeValues.value(type);
  if (typeName.isEmpty())
    return;
  query.removeQueryItem(keyName(eUrl::Type));
  query.addQueryItem(keyName(eUrl::Type), typeName);
  this->setQuery(query);
}

QStringList Url::encryptionKeys() const
{
  QStringList encryptionKeysList;
  const auto query = QUrlQuery(*this);
  const auto encryptionKeysString = query.queryItemValue(keysName);
  if (!encryptionKeysString.isEmpty())
    encryptionKeysList = encryptionKeysString.split(';');
  return encryptionKeysList;
}

void Url::setEncryptionKeys(const QStringList &keyIDs)
{
  auto query = QUrlQuery(*this);
  query.removeQueryItem(keysName);
  query.addQueryItem(keysName, keyIDs.join(';'));
  this->setQuery(query);
}

QString Url::typeInternalName(Storage::eType storageType)
{
  return typeValues.value(storageType);
}

} // namespace Xml
} // namespace Storage
