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

#include "converter.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QDomDocument>
#include <QDebug>
#include <QDomImplementation>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "mymoney/mymoneyexception.h"
#include "mymoney/mymoneyenums.h"

#include "domhelper.h"
#include "storage/interface/enums.h"

namespace Storage {
namespace Xml {

class ConverterPrivate
{
  Q_DISABLE_COPY(ConverterPrivate)

public:
  ConverterPrivate() = default;

  void signalProgress(int current, int total, const QString &msg = QString()) const;
  static void convertDocType(QDomDocument &doc, const QString &documentTypeString);

  static auto convertFunctionsKMyMoney();
  static void convertDocTypeToKMM(QDomDocument &doc);
  static void convertFileInfoToKMM(QDomDocument &doc);
  static void packMatchedTransactions(QDomDocument &doc);

  static auto convertFunctionsKMyMoneyNEXT();
  static void convertDocTypeToKMMN(QDomDocument &doc);
  static void convertFileInfoToKMMN(QDomDocument &doc);
  static void extractMatchedTransactions(QDomDocument &doc);

  static auto convertFunctions(Storage::eProducer storageProducer);

  Storage::ProgressCallback m_callback = nullptr;
};

void ConverterPrivate::signalProgress(int current, int total, const QString &msg) const
{
  if (m_callback)
    m_callback(current, total, msg);
}

void ConverterPrivate::convertDocType(QDomDocument &doc, const QString &documentTypeString)
{
  auto rootDocumentElement = doc.importNode(doc.documentElement(), true).toElement();
  doc = QDomDocument(documentTypeString);
  auto instruct = doc.createProcessingInstruction(QStringLiteral("xml"),
                                                  QStringLiteral("version=\"1.0\" encoding=\"utf-8\""));
  doc.appendChild(instruct);
  rootDocumentElement.setTagName(documentTypeString);
  doc.appendChild(rootDocumentElement);
}

auto ConverterPrivate::convertFunctionsKMyMoney()
{
  const QVector<void(*)(QDomDocument &)> convertFunctions {
    convertDocTypeToKMM,
    convertFileInfoToKMM,
        packMatchedTransactions
  };

  return convertFunctions;
}

void ConverterPrivate::convertDocTypeToKMM(QDomDocument &doc)
{
  convertDocType(doc, QStringLiteral("KMYMONEY-FILE"));
}

void ConverterPrivate::convertFileInfoToKMM(QDomDocument &doc)
{
  const QMap<QString, QString> versionTags {
    {QStringLiteral("VERSION"),     QStringLiteral("1")},
    {QStringLiteral("FIXVERSION"),  QStringLiteral("5")}
  };

  for (auto it = versionTags.cbegin(); it != versionTags.cend(); ++it)
    DomHelper::setTagIdValueInFileInfoNode(doc, it.key(), it.value());
}

QMap<QString, QString> keyValueMapOfNode(QDomNode &node)
{
  QMap<QString, QString> keyValueMap;
  auto kvpPairsElement = node.namedItem("KEYVALUEPAIRS");
  auto kvpPairElements = kvpPairsElement.childNodes();
  for (auto i = 0; i < kvpPairElements.length(); ++i) {
    auto kvpPairElement = kvpPairElements.at(i).toElement();
    keyValueMap.insert(kvpPairElement.attribute("key"), kvpPairElement.attribute("value"));
  }
  return keyValueMap;
}

void addKeyValueOfNode(QDomDocument &doc, QDomNode &node, const QString &key, const QString &value)
{
  auto kvpPairsElement = node.namedItem("KEYVALUEPAIRS");
  if (kvpPairsElement.isNull()) {
    kvpPairsElement = doc.createElement("KEYVALUEPAIRS");
    node.appendChild(kvpPairsElement);
  }
  auto kvpPairElements = kvpPairsElement.childNodes();
  QDomElement kvpPairElement;
  for (auto i = 0; i < kvpPairElements.length(); ++i) {
    auto potentialKvpPairElement = kvpPairElements.at(i).toElement();
    if (potentialKvpPairElement.attribute("key") == key)
      kvpPairElement = potentialKvpPairElement;
  }

  if (kvpPairElement.isNull()) {
    kvpPairElement = doc.createElement("PAIR");
    kvpPairsElement.appendChild(kvpPairElement);
  }
  kvpPairElement.setAttribute("key", key);
  kvpPairElement.setAttribute("value", value);
}

void removeKeyValueOfNode(QDomNode &node, const QString &key)
{
  auto kvpPairsElement = node.namedItem("KEYVALUEPAIRS");
  auto kvpPairElements = kvpPairsElement.childNodes();
  for (auto i = 0; i < kvpPairElements.length(); ++i) {
    auto kvpPairElement = kvpPairElements.at(i).toElement();
    if (kvpPairElement.attribute("key") == key) {
      if (kvpPairElements.length() == 1)
        node.removeChild(kvpPairsElement);
      else
        kvpPairsElement.removeChild(kvpPairElement);
    }
  }
}

void setkeyValueMapOfNode(QDomDocument &doc, QDomElement &element, QMap<QString, QString> keyValueMap)
{
  auto kvpPairsElement = element.namedItem("KEYVALUEPAIRS").toElement();
  if (keyValueMap.isEmpty()) {
    element.removeChild(kvpPairsElement);
    return;
  }

  auto kvpPairElements = kvpPairsElement.elementsByTagName("PAIR");
  while (kvpPairElements.length())
    kvpPairsElement.removeChild(kvpPairElements.at(0));

  for (auto it = keyValueMap.cbegin(); it != keyValueMap.cend(); ++it) {
    auto kvpPairElement = doc.createElement("PAIR");
    kvpPairElement.setAttribute("key", it.key());
    kvpPairElement.setAttribute("value", it.value());
    kvpPairsElement.appendChild(kvpPairElement);
  }
}

QDomElement findTransactionByID(QDomNode transactionsNode, const QString &id)
{
  auto transactionNodes = transactionsNode.childNodes();
  for (auto i = 0; i < transactionNodes.length(); ++i) {
    auto transactionElement = transactionNodes.at(i).toElement();
    if (transactionElement.attribute("id") == id)
      return transactionElement;
  }
  return QDomElement();
}

/**
* @brief packMatchedTransactions
* @param doc as parsed document
* Changes (2020-01-01):
* 1) make two separate transactions out of two matched transactions
* 2) convert result of matching into old matched transaction
* 3) remove origin (imported, typed, matched) from transaction
* 4) replace kmm-match-transaction with kmm-matched-tx
* 5) replace two matched split IDs in kmm-match-split with one split
*/

void ConverterPrivate::packMatchedTransactions(QDomDocument &doc)
{
  auto transactionsNodes = doc.elementsByTagName("TRANSACTIONS");
  // no transactions = no problem
  if (transactionsNodes.isEmpty())
    return;

  auto transactionsNode = transactionsNodes.item(0);
  auto transactionNodes = transactionsNode.childNodes();
  if (transactionNodes.isEmpty())
    return;

  // mark imported transactions sequentially now, because latter the transactions flow could be random
  for (auto i = 0; i < transactionNodes.length(); ++i) {
    auto transactionElement = transactionNodes.item(i).toElement();
    auto transactionOriginAttribute = transactionElement.attribute("origin").toInt();

    // restore imported key value pair fo transaction
    if (transactionOriginAttribute & eMyMoney::Transaction::Origin::Imported)
      addKeyValueOfNode(doc, transactionElement, "Imported", "true");
  }


  // transactions could be picked up randomly for removal...
  // ...which could disrupt the flow of following loop...
  // ...so store them for later
  QVector<QDomNode> nodesToRemove;

  for (auto i = 0; i < transactionNodes.length(); ++i) {
    auto transactionElement = transactionNodes.item(i).toElement();
    auto transactionOriginAttribute = transactionElement.attribute("origin").toInt();

    if (transactionOriginAttribute & eMyMoney::Transaction::Origin::MatchingOutput) {
      auto splitsElement = transactionElement.namedItem("SPLITS").toElement();
      auto splitElements = splitsElement.elementsByTagName("SPLIT");
      for (auto j = 0; j < splitElements.length(); ++j) {
        auto splitElement = splitElements.at(j).toElement();
        auto keyValueMap = keyValueMapOfNode(splitElement);
        if (keyValueMap.contains("kmm-match-transaction")) {
          // read matched transaction information in the new way
          auto matchTransactionList = keyValueMap.value("kmm-match-transaction").split(";");
          auto matchSplitList = keyValueMap.value("kmm-match-split").split(";");

          auto sourceTransactionElement = findTransactionByID(transactionsNode, matchTransactionList.at(0));
          auto embeddedTransactionElement = findTransactionByID(transactionsNode, matchTransactionList.at(1));

          // write matched transaction information in the old way
          embeddedTransactionElement.setAttribute("id", QString());
          embeddedTransactionElement.removeAttribute("origin");

          auto matchDocument = QDomDocument("MATCH");
          auto containerElement = matchDocument.createElement("CONTAINER");
          matchDocument.appendChild(containerElement);
          containerElement.appendChild(embeddedTransactionElement);
          auto matchDocumentString = matchDocument.toString();
          matchDocumentString.replace(QLatin1String("<"), QLatin1String("&#60;"));

          keyValueMap.insert("kmm-matched-tx", matchDocumentString);
          keyValueMap.insert("kmm-match-split", matchSplitList.at(1));

          const auto postDate = sourceTransactionElement.attribute("postdate");
          if (!postDate.isEmpty())
            keyValueMap.insert("kmm-orig-postdate", postDate);
          auto sourceTransactionSplitsElement = sourceTransactionElement.namedItem("SPLITS").toElement();
          auto sourceTransactionSplitElements = sourceTransactionSplitsElement.childNodes();
          for (auto k = 0; k < sourceTransactionSplitElements.length(); ++k) {
            auto sourceTransactionSplitElement = sourceTransactionSplitElements.at(k).toElement();
            auto sourceTransactionSplitId = sourceTransactionSplitElement.attribute("id");
            if (sourceTransactionSplitId != matchSplitList.at(1))
              continue;

            const auto payee = sourceTransactionSplitElement.attribute("payee");
            if (payee.isEmpty()) // Lack of exclamation mark is not a typo. See transactionmatcher.cpp
              keyValueMap.insert("kmm-orig-payee", payee);

            const auto memo = sourceTransactionSplitElement.attribute("memo");
            if (!memo.isEmpty())
              keyValueMap.insert("kmm-orig-memo", memo);

            break;
          }

          keyValueMap.remove("kmm-match-transaction");

          setkeyValueMapOfNode(doc, splitElement, keyValueMap);
          transactionElement.removeAttribute("origin");
          nodesToRemove.append(sourceTransactionElement);
          nodesToRemove.append(embeddedTransactionElement);
        }
      }
    }
    transactionElement.removeAttribute("origin");
  }

  for (const auto &nodeToRemove : nodesToRemove)
    transactionsNode.removeChild(nodeToRemove);

  transactionsNode.toElement().setAttribute("count", transactionNodes.count());

  auto schedulesNodes = doc.elementsByTagName("SCHEDULES");
  // no schedules = no problem
  if (schedulesNodes.isEmpty())
    return;

  auto schedulesNode = schedulesNodes.item(0);
  auto scheduleNodes = schedulesNode.childNodes();
  if (scheduleNodes.isEmpty())
    return;

  for (auto i = 0; i < scheduleNodes.length(); ++i) {
    auto scheduleElement = scheduleNodes.item(i).toElement();
    auto transactionElement = scheduleElement.namedItem("TRANSACTION").toElement();
    auto transactionOriginAttribute = transactionElement.attribute("origin").toInt();
    transactionElement.removeAttribute("origin");
    // restore imported key value pair fo transaction
    if (!(transactionOriginAttribute & eMyMoney::Transaction::Origin::Imported))
      continue;
    addKeyValueOfNode(doc, transactionElement, "Imported", "true");
  }
}

auto ConverterPrivate::convertFunctionsKMyMoneyNEXT()
{
  const QVector<void(*)(QDomDocument &)> convertFunctions {
    convertDocTypeToKMMN,
    convertFileInfoToKMMN,
    extractMatchedTransactions
  };
  return convertFunctions;
}

void ConverterPrivate::convertFileInfoToKMMN(QDomDocument &doc)
{
  const QMap<QString, QString> versionTags {
    {QStringLiteral("VERSION"),     QStringLiteral("20200101")},
    {QStringLiteral("FIXVERSION"),  QString()}
  };

  for (auto it = versionTags.cbegin(); it != versionTags.cend(); ++it)
    DomHelper::setTagIdValueInFileInfoNode(doc, it.key(), it.value());
}

void ConverterPrivate::convertDocTypeToKMMN(QDomDocument &doc)
{
  convertDocType(doc, QStringLiteral("KMYMONEYNEXT-FILE"));
}

/**
* @brief extractMatchedTransactions
* @param doc as parsed document
* Changes (2018-08-19):
* 1) make two separate transactions out of two matched transactions
* 2) create a new tramsaction as a result of matching
* 3) add origin (imported, typed, matched) of transaction
* 4) add kmm-match-transaction containing two matched transaction IDs to every matched split
* 5) add kmm-match-split containing two matched split IDs to every matched split
*/
void ConverterPrivate::extractMatchedTransactions(QDomDocument &doc)
{
  auto transactionsNodes = doc.elementsByTagName("TRANSACTIONS");

  // no transactions = no problem
  if (transactionsNodes.isEmpty())
    return;

  auto transactionsNode = transactionsNodes.item(0);
  auto transactionNodes = transactionsNode.childNodes();
  if (transactionNodes.isEmpty())
    return;

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
    auto transactionKvpMap = keyValueMapOfNode(transactionElement);
    if (transactionKvpMap.contains("Imported")) {
      removeKeyValueOfNode(transactionElement, "Imported");
      transactionOriginAttribute = static_cast<eMyMoney::Transaction::Origin>(transactionOriginAttribute | eMyMoney::Transaction::Origin::Imported);
    } else {
      transactionOriginAttribute = static_cast<eMyMoney::Transaction::Origin>(transactionOriginAttribute | eMyMoney::Transaction::Origin::Typed);
    }

    auto splitsNode = transactionElement.elementsByTagName("SPLITS").item(0);
    auto splitNodes = splitsNode.childNodes();
    for (auto j = 0; j < splitNodes.length(); ++j) {
      auto splitElement = splitNodes.item(j).toElement();
      auto splitKVPElement = splitElement.elementsByTagName("KEYVALUEPAIRS").item(0).toElement();
      auto splitPairNodes = splitKVPElement.elementsByTagName("PAIR");

      // get a map of all key-value pairs out of a split for convenient usage
      QMap<QString, QString> splitKvpMap;
      for (auto k = 0; k < splitPairNodes.length() ; ++k) {
        auto pairNodeElement = splitPairNodes.item(k).toElement();
        splitKvpMap.insert(pairNodeElement.attribute("key"), pairNodeElement.attribute("value"));
      }

      // act on splits that embedded imported transaction
      if (splitKvpMap.contains("kmm-matched-tx")) {
        // unembedd imported transaction
        auto embeddedTransactionXML = splitKvpMap.value("kmm-matched-tx");
        const QVector<QString> possibleDocumentStartStrings {
          QStringLiteral("&lt;"), QStringLiteral("&#60;")
        };
        for (const auto &possibleDocumentStartString : possibleDocumentStartStrings) {
          if (embeddedTransactionXML.startsWith(possibleDocumentStartString)) {
            embeddedTransactionXML.replace(possibleDocumentStartString, QLatin1String("<"));
            break;
          }
        }

        QDomDocument embeddedTransactionDoc;
        QDomNode embeddedTransactionNode;
        if (!embeddedTransactionDoc.setContent(embeddedTransactionXML))
          throw MYMONEYEXCEPTION(QString::fromLatin1("Cannot convert kmm-matched-tx %1").arg(embeddedTransactionXML));
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
        if (splitKvpMap.contains("kmm-orig-postdate"))
          sourceTransactionElement.setAttribute("postdate", splitKvpMap.value("kmm-orig-postdate"));

        auto sourceTransactionSplitsNode = sourceTransactionElement.elementsByTagName("SPLITS").item(0);
        auto sourceTransactionSplitNodes = sourceTransactionSplitsNode.childNodes();
        for (auto k = 0; k < sourceTransactionSplitNodes.length(); ++k) {
          auto sourceTransactionSplitElement = sourceTransactionSplitNodes.item(k).toElement();
          auto sourceTransactionSplitID = sourceTransactionSplitElement.attribute("id");
          if (sourceTransactionSplitID == splitKvpMap.value("kmm-match-split")) {
            if (splitKvpMap.contains("kmm-orig-payee"))
              sourceTransactionSplitElement.setAttribute("payee", splitKvpMap.value("kmm-orig-payee"));
            if (splitKvpMap.contains("kmm-orig-memo"))
              sourceTransactionSplitElement.setAttribute("memo", splitKvpMap.value("kmm-orig-memo"));
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
        matchedSplitKeyValuePair.setAttribute("value", QString::fromLatin1("%1;%2").arg(splitElement.attribute("id"), splitKvpMap.value("kmm-match-split")));
        splitKVPElement.appendChild(matchedSplitKeyValuePair);

        // append new transactions now so that they will get picked by this loop
        // and their origin could be established at the beggining of this loop
        if (transactionsNode.appendChild(embeddedTransactionNode).isNull())
          throw MYMONEYEXCEPTION(QString::fromLatin1("Cannot add embedded transaction %1").arg(embeddedTransactionNode.toDocument().toString()));
        if (transactionsNode.appendChild(sourceTransactionElement).isNull())
          throw MYMONEYEXCEPTION(QString::fromLatin1("Cannot add source transaction %1").arg(sourceTransactionElement.toDocument().toString()));

        // we know that it's and output of matching
        transactionOriginAttribute = static_cast<eMyMoney::Transaction::Origin>(transactionOriginAttribute | eMyMoney::Transaction::Origin::MatchingOutput);
      }

      transactionElement.setAttribute("origin", transactionOriginAttribute);
    }
  }
  transactionsNode.toElement().setAttribute("count", transactionNodes.count());

  auto schedulesNodes = doc.elementsByTagName("SCHEDULES");
  // no schedules = no problem
  if (schedulesNodes.isEmpty())
    return;

  auto schedulesNode = schedulesNodes.item(0);
  auto scheduleNodes = schedulesNode.childNodes();
  if (scheduleNodes.isEmpty())
    return;

  for (auto i = 0; i < scheduleNodes.length(); ++i) {
    auto scheduleElement = scheduleNodes.item(i).toElement();
    auto transactionElement = scheduleElement.namedItem("TRANSACTION").toElement();
    auto transactionKvpMap = keyValueMapOfNode(transactionElement);
    auto transactionOriginAttribute = eMyMoney::Transaction::Origin::None;
    if (transactionKvpMap.contains("Imported")) {
      removeKeyValueOfNode(transactionElement, "Imported");
      transactionOriginAttribute = eMyMoney::Transaction::Origin::Imported;
    } else {
      transactionOriginAttribute = eMyMoney::Transaction::Origin::Typed;
    }
    transactionElement.setAttribute("origin", transactionOriginAttribute);
  }
}

auto ConverterPrivate::convertFunctions(Storage::eProducer storageProducer)
{
  switch (storageProducer) {
    case Storage::eProducer::KMyMoney:
      return convertFunctionsKMyMoney();
    case Storage::eProducer::KMyMoneyNEXT:
      return convertFunctionsKMyMoneyNEXT();
    default:
      throw MYMONEYEXCEPTION_CSTRING("Unrecognized storage producer.");
  }
}


Converter::Converter(Storage::ProgressCallback callback) :
d_ptr(std::make_unique<ConverterPrivate>())
{
  Q_D(Converter);
  d->m_callback = callback;
}

Converter::~Converter() = default;


QByteArray Converter::convertStorage(const QByteArray &rawStorage, eProducer targetStorageProducer)
{
  Q_D(Converter);

  QDomDocument doc;
  QString errMsg;
  int errRow;
  int errColumn;
  if (!doc.setContent(rawStorage, &errMsg, &errRow, &errColumn))
    throw MYMONEYEXCEPTION(QString::fromLatin1("Failed to parse XML file on row: %1, column: %2, message: %3\n").arg(errRow).arg(errColumn).arg(errMsg));

  const auto functions = d->convertFunctions(targetStorageProducer);
  for (const auto &function : functions)
    function(doc);

  return doc.toByteArray();
}

} // namespace Xml
} // namespace Storage
