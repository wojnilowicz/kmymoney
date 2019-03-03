/*
 * Copyright 2018       Łukasz Wojniłowicz <lukasz.wojnilowicz@gmail.com>
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

#include "xmlstorageupdater.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QIODevice>
#include <QDomDocument>
#include <QDebug>
#include <QFile>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "mymoneyenums.h"

namespace XMLStorageUpdater
{

  /**
 * @brief updateVersion1Fix5
 * @param doc as parsed document
 * Changes (2018-08-19):
 * 1) make two separate transactions out of two matched transactions
 * 2) create a new tramsaction as a result of matching
 * 3) add origin (imported, typed, matched) of transaction
 * 4) add kmm-match-transaction containing two matched transaction IDs to every matched split
 * 5) add kmm-match-split containing two matched split IDs to every matched split
 * @return true if upgrade is successful
 */

bool upgradeVersion1Fix5(QDomDocument &doc)
{
  auto transactionsNodes = doc.elementsByTagName("TRANSACTIONS");

  // no transactions = no problem
  if (transactionsNodes.isEmpty())
    return true;

  auto transactionsNode = transactionsNodes.item(0);
  auto transactionNodes = transactionsNode.childNodes();
  if (transactionNodes.isEmpty())
    return true;

  // we need to know the highest unoccupied ID to know what IDs can we assign to new (reorganized) transactions
  QStringList transactionIDs;
  ulong lastTransactionID = 0;
  for (auto i = 0; i < transactionNodes.length(); ++i) {
    auto transationElement = transactionNodes.item(i).toElement();
    transactionIDs.append(transationElement.attribute("id"));
  }
  std::sort(transactionIDs.begin(), transactionIDs.end());
  lastTransactionID = transactionIDs.last().mid(1).toUInt();

  for (auto i = 0; i < transactionNodes.length(); ++i) {
    auto transactionElement = transactionNodes.item(i).toElement();

    // search if the transaction is imported in order to set its origin properly
    auto transactionOriginAttribute = static_cast<eMyMoney::Transaction::Origin>(transactionElement.attribute("origin").toInt());
    auto transactionKVPElement = transactionNodes.item(i).namedItem("KEYVALUEPAIRS").toElement();
    auto transactionPairNodes = transactionKVPElement.elementsByTagName("PAIR");
    for (auto k = 0; k < transactionPairNodes.length() ; ++k) {
      auto pairNodeElement = transactionPairNodes.item(k).toElement();
      if (pairNodeElement.attribute("key").compare("Imported", Qt::CaseInsensitive) == 0) {
        transactionKVPElement.removeChild(pairNodeElement);
        transactionOriginAttribute = static_cast<eMyMoney::Transaction::Origin>(transactionOriginAttribute | eMyMoney::Transaction::Origin::Imported);
        break;
      }
    }

    // if transaction isn't imported then it has been manually typed
    if (!(transactionOriginAttribute & eMyMoney::Transaction::Origin::Imported))
      transactionOriginAttribute = static_cast<eMyMoney::Transaction::Origin>(transactionOriginAttribute | eMyMoney::Transaction::Origin::Typed);

    auto splitsNode = transactionElement.elementsByTagName("SPLITS").item(0);
    auto splitNodes = splitsNode.childNodes();
    for (auto j = 0; j < splitNodes.length(); ++j) {
      auto splitElement = splitNodes.item(j).toElement();
      auto splitKVPElement = splitElement.elementsByTagName("KEYVALUEPAIRS").item(0).toElement();
      auto splitPairNodes = splitKVPElement.elementsByTagName("PAIR");

      // get a map of all key-value pairs out of a split for convenient usage
      QMap<QString, QString> keyValueMap;
      for (auto k = 0; k < splitPairNodes.length() ; ++k) {
        auto pairNodeElement = splitPairNodes.item(k).toElement();
        keyValueMap.insert(pairNodeElement.attribute("key"), pairNodeElement.attribute("value"));
      }

      // act on splits that embedded imported transaction
      if (keyValueMap.contains("kmm-matched-tx")) {
        // unembedd imported transaction
        auto embeddedTransactionXML = keyValueMap.value("kmm-matched-tx");
        embeddedTransactionXML.replace(QLatin1String("&lt;"), QLatin1String("<"));
        QDomDocument embeddedTransactionDoc;
        QDomNode embeddedTransactionNode;
        embeddedTransactionDoc.setContent(embeddedTransactionXML);
        embeddedTransactionNode = embeddedTransactionDoc.documentElement().firstChild().cloneNode();
        auto embeddedTransactionElement = embeddedTransactionNode.toElement();

        // assign free transaction id
        auto embeddedTransactionID = QString::fromLatin1("T%1").arg(QString::number(++lastTransactionID).rightJustified(18 , '0'));
        embeddedTransactionElement.setAttribute("id", embeddedTransactionID);

        // at this point we only know that it's an input for matching
        auto embeddedTransactionOrigin = eMyMoney::Transaction::Origin::None;
        embeddedTransactionOrigin = static_cast<eMyMoney::Transaction::Origin>(embeddedTransactionOrigin | eMyMoney::Transaction::Origin::MatchingInput);
        embeddedTransactionElement.setAttribute("origin", embeddedTransactionOrigin);

        // we don't need those attributes anymore and their values have been stored earlier
        const QVector<QString> backupValuesToBeRemoved {"kmm-orig-postdate",
                                                        "kmm-orig-payee",
                                                        "kmm-orig-memo",
                                                        "kmm-matched-tx",
                                                        "kmm-match-split"};
        for (auto k = splitPairNodes.length() - 1; k >= 0; --k) {
          auto pairNodeElement = splitPairNodes.item(k).toElement();
          if (backupValuesToBeRemoved.contains(pairNodeElement.attribute("key")))
            splitKVPElement.removeChild(splitPairNodes.item(k));
        }

        // we had two tranactions: matched and imported,
        // and now we have three: matched (result of matching) , imported (input for matching), typed (input for matching)
        auto sourceTransactionNode = transactionElement.cloneNode();

        // assign free transaction id
        auto sourceTransactionElement = sourceTransactionNode.toElement();
        auto sourceTransactionID = QString::fromLatin1("T%1").arg(QString::number(++lastTransactionID).rightJustified(18 , '0'));
        sourceTransactionElement.setAttribute("id", sourceTransactionID);

        // at this point we only know that it's an input for matching
        auto sourceTransactionOrigin = eMyMoney::Transaction::Origin::None;
        sourceTransactionOrigin = static_cast<eMyMoney::Transaction::Origin>(sourceTransactionOrigin | eMyMoney::Transaction::Origin::MatchingInput);
        sourceTransactionElement.setAttribute("origin", sourceTransactionOrigin);

        // previous matching changed input transaction a little bit, so restore its values to the original ones
        if (keyValueMap.contains("kmm-orig-postdate"))
          sourceTransactionElement.setAttribute("postdate", keyValueMap.value("kmm-orig-postdate"));

        auto sourceTransactionSplitsNode = sourceTransactionElement.elementsByTagName("SPLITS").item(0);
        auto sourceTransactionSplitNodes = sourceTransactionSplitsNode.childNodes();
        for (auto k = 0; k < sourceTransactionSplitNodes.length(); ++k) {
          auto sourceTransactionSplitElement = sourceTransactionSplitNodes.item(k).toElement();
          auto sourceTransactionSplitID = sourceTransactionSplitElement.attribute("id");
          if (sourceTransactionSplitID == keyValueMap.value("kmm-match-split")) {
            if (keyValueMap.contains("kmm-orig-payee"))
              sourceTransactionSplitElement.setAttribute("payee", keyValueMap.value("kmm-orig-payee"));
            if (keyValueMap.contains("kmm-orig-memo"))
              sourceTransactionSplitElement.setAttribute("memo", keyValueMap.value("kmm-orig-memo"));
            break;
          }
        }

        // matched transaction has information about transactions (their IDs) that were input for matching
        // example "T000000000000000001;T000000000000000002"
        auto matchedTransactionKeyValuePair = doc.createElement("PAIR");
        matchedTransactionKeyValuePair.setAttribute("key", "kmm-match-transaction");
        matchedTransactionKeyValuePair.setAttribute("value", QString::fromLatin1("%1;%2").arg(sourceTransactionID, embeddedTransactionID));
        splitKVPElement.appendChild(matchedTransactionKeyValuePair);

        // matched transaction has information about splits (their IDs) that were matched during matching
        // example "S0001;S0001"
        auto matchedSplitKeyValuePair = doc.createElement("PAIR");
        matchedSplitKeyValuePair.setAttribute("key", "kmm-match-split");
        matchedSplitKeyValuePair.setAttribute("value", QString::fromLatin1("%1;%2").arg(splitElement.attribute("id"), keyValueMap.value("kmm-match-split")));
        splitKVPElement.appendChild(matchedSplitKeyValuePair);

        // append new transactions now so that they will get picked by this loop
        // and their origin could be established at the beggining of this loop
        transactionsNode.appendChild(embeddedTransactionNode);
        transactionsNode.appendChild(sourceTransactionElement);

        // we know that it's and output of matching
        transactionOriginAttribute = static_cast<eMyMoney::Transaction::Origin>(transactionOriginAttribute | eMyMoney::Transaction::Origin::MatchingOutput);
      }

      transactionElement.setAttribute("origin", transactionOriginAttribute);
    }
  }
  return true;
}

bool updateFile(QIODevice* pXMLDevice)
{
  QDomDocument doc;
  auto ret = doc.setContent(pXMLDevice);
  if (!ret)
    return false;

  auto fixVersionElement = doc.elementsByTagName("FIXVERSION").item(0).toElement();
  const auto fixVersion = fixVersionElement.attribute("id").toUInt();
  auto versionElement = doc.elementsByTagName("VERSION").item(0).toElement();
  const auto version = versionElement.attribute("id").toUInt();

  auto isUpdateSuccessful = true;

  switch (version) {
    case 1:
      switch (fixVersion) {
        case 1:
        case 2:
        case 3:
        case 4:
          isUpdateSuccessful |= upgradeVersion1Fix5(doc);
          // intentional fall through
        case 5:
          break;
        default:
          break;
      }
      // intentional fall through
    default:
      break;
  }

  if (!(version == 1 && fixVersion < 4)) {
    versionElement.setAttribute("id", "1");
    fixVersionElement.setAttribute("id", "5");
  }

  if (isUpdateSuccessful) {
    pXMLDevice->close();
    pXMLDevice->open(QIODevice::ReadWrite | QIODevice::Truncate);
    pXMLDevice->write(doc.toByteArray());
  }

  return isUpdateSuccessful;
}

}
