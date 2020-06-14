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

#include "iurl.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QMap>
#include <QUrlQuery>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

namespace Storage {

IUrl::IUrl(const QUrl &url) :
  QUrl(url)
{
}

QString IUrl::pluginScheme() const
{
  return this->scheme();
}

void IUrl::setPluginScheme(const QString &name)
{
  this->setScheme(name);
}

QString IUrl::storageType() const
{
  const auto query = QUrlQuery(*this);
  return query.queryItemValue(keyName(eUrl::Type));
}

bool IUrl::noSwitch() const
{
  const auto query = QUrlQuery(*this);
  return query.hasQueryItem(keyName(eUrl::NoSwitch));
}

void IUrl::setNoSwitch()
{
  auto query = QUrlQuery(*this);
  query.removeQueryItem(keyName(eUrl::NoSwitch));
  query.addQueryItem(keyName(eUrl::NoSwitch), QStringLiteral("true"));
  this->setQuery(query);
}

QUrl IUrl::consistentUrlForStoring(const QUrl &url)
{
  QUrl urlCopy;
  urlCopy.setScheme(url.scheme());
  if(!url.path().isEmpty())
    urlCopy.setPath(url.path());
  urlCopy.setQuery(url.query());
  if(!url.host().isEmpty())
    urlCopy.setHost(url.host());
  return urlCopy;
}

QString IUrl::keyName(eUrl key)
{
  static const QMap<eUrl, QString> keyNames {
    {eUrl::Plugin,    QStringLiteral("plugin")},
    {eUrl::Type,      QStringLiteral("type")},
    {eUrl::NoSwitch,  QStringLiteral("noswitch")}
  };
  return keyNames.value(key);
}

} // namespace Storage


