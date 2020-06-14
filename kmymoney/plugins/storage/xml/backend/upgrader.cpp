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

#include "upgrader.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QIODevice>
#include <QDomDocument>
#include <QMap>
#include <QVector>
#include <QDebug>
#include <QUuid>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "mymoney/mymoneyexception.h"

#include "storage/interface/icallback.h"
#include "storage/interface/enums.h"
#include "domhelper.h"

namespace Storage {
namespace Xml {

class UpgraderPrivate
{
  Q_DISABLE_COPY(UpgraderPrivate)

public:
  UpgraderPrivate() = default;

  void signalProgress(int current, int total, const QString &msg = QString()) const;

  static auto upgradeFunctionsKMyMoney();
  static void upgradeFrom1003(QDomDocument &doc);
  static void upgradeFrom1004(QDomDocument &doc);
  static void upgradeFrom1005(QDomDocument &doc);

  static auto upgradeFunctionsKMyMoneyNEXT();
  static void upgradeFrom20200101(QDomDocument &doc);

  static auto upgradeFunctions(Storage::eProducer storageProducer);

  eProducer m_storageProducer;
  ProgressCallback m_callback = nullptr;
};

void UpgraderPrivate::signalProgress(int current, int total, const QString &msg) const
{
  if (m_callback)
    m_callback(current, total, msg);
}

auto UpgraderPrivate::upgradeFunctionsKMyMoney()
{
  QMap <unsigned int, void(*)(QDomDocument &doc)> upgradeFunctions {
    {1003, upgradeFrom1003},
    {1004, upgradeFrom1004},
    {1005, upgradeFrom1005}
  };
  return upgradeFunctions;
}

void UpgraderPrivate::upgradeFrom1003(QDomDocument &doc)
{
  auto keyValuePairsNodes = doc.elementsByTagName("KEYVALUEPAIRS");

  if (keyValuePairsNodes.isEmpty())
    return;

  auto idElement = doc.createElement("PAIR");
  idElement.setAttribute("key", "kmm-id");
  idElement.setAttribute("value", QUuid::createUuid().toString());
  keyValuePairsNodes.item(0).appendChild(idElement);

  DomHelper::setTagIdValueInFileInfoNode(doc, "FIXVERSION", "4");
}

void UpgraderPrivate::upgradeFrom1004(QDomDocument &doc)
{
  auto currenciesNodes = doc.elementsByTagName("CURRENCIES");
  // no currencies = no problem
  if (currenciesNodes.isEmpty())
    return;

  auto currencyNodes = currenciesNodes.item(0).childNodes();

  const QVector<QString> idsOfCurrenciesToChange {
    "XAG", "XAU", "XPD", "XPT"
  };

  for (auto i = 0 ; i < currencyNodes.size(); ++i) {
    auto currencyNode = currencyNodes.at(i);
    if (currencyNode.nodeName() != "CURRENCY")
      continue;
    auto currencyNodeAttributes = currencyNode.attributes();
    auto idAttribute = currencyNodeAttributes.namedItem("id");
    if (idAttribute.isNull())
      continue;

    auto currencyId = idAttribute.nodeValue();
    if (currencyId.isEmpty() || !idsOfCurrenciesToChange.contains(currencyId))
      continue;

    auto scfAttribute = currencyNodeAttributes.namedItem("scf");
    auto safAttribute = currencyNodeAttributes.namedItem("saf");
    if (scfAttribute.isNull() || safAttribute.isNull())
      continue;

    auto scfValue = scfAttribute.nodeValue();
    auto safValue = safAttribute.nodeValue();

    if (scfValue == safValue)
      continue;

    safAttribute.setNodeValue(scfValue);
  }

  DomHelper::setTagIdValueInFileInfoNode(doc, "FIXVERSION", "5");
}

void UpgraderPrivate::upgradeFrom1005(QDomDocument &doc)
{
  Q_UNUSED(doc)
}

auto UpgraderPrivate::upgradeFunctionsKMyMoneyNEXT()
{
  QMap <unsigned int, void(*)(QDomDocument &doc)> upgradeFunctions {
    {20200101, upgradeFrom20200101}
  };
  return upgradeFunctions;
}

void UpgraderPrivate::upgradeFrom20200101(QDomDocument &doc)
{
  Q_UNUSED(doc)
}

auto UpgraderPrivate::upgradeFunctions(Storage::eProducer storageProducer)
{
  switch (storageProducer) {
    case Storage::eProducer::KMyMoney:
      return upgradeFunctionsKMyMoney();
    case Storage::eProducer::KMyMoneyNEXT:
      return upgradeFunctionsKMyMoneyNEXT();
    default:
      throw MYMONEYEXCEPTION_CSTRING("Unrecognized storage producer.");
  }
}


Upgrader::Upgrader(Storage::ProgressCallback callback) :
d_ptr(std::make_unique<UpgraderPrivate>())
{
  Q_D(Upgrader);
  d->m_callback = callback;
}

Upgrader::~Upgrader() = default;

QByteArray Upgrader::upgradeStorage(const QByteArray &rawStorage, eProducer storageProducer, unsigned int sourceVersion, unsigned int targetVersion)
{
  Q_D(Upgrader);

  QDomDocument doc;
  QString errMsg;
  int errRow;
  int errColumn;
  if (!doc.setContent(rawStorage, &errMsg, &errRow, &errColumn))
    throw MYMONEYEXCEPTION(QString::fromLatin1("Failed to parse XML file on row: %1, column: %2, message: %3\n").arg(errRow).arg(errColumn).arg(errMsg));

  const auto functions = d->upgradeFunctions(storageProducer);
  const auto versions = supportedVersions(storageProducer);

  auto targetVersionIdx = versions.indexOf(targetVersion);
  if (targetVersionIdx == -1)
    targetVersionIdx = versions.count();

  auto sourceVersionIdx = versions.indexOf(sourceVersion);
  if (sourceVersionIdx == -1)
    throw MYMONEYEXCEPTION_CSTRING("No upgrade from chosen source version.");

  for (auto idx = sourceVersionIdx; idx < targetVersionIdx; ++idx)
    functions.value(versions[idx])(doc);

  return doc.toByteArray();
}

QVector<unsigned int> Upgrader::supportedVersions(Storage::eProducer storageProducer)
{
  return UpgraderPrivate::upgradeFunctions(storageProducer).keys().toVector();
}

} // namespace Xml
} // namespace Storage
