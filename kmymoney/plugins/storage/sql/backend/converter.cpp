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

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QXmlAttributes>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "mymoney/mymoneymoney.h"
#include "mymoney/mymoneyexception.h"
#include "mymoney/mymoneyenums.h"

#include "storage/interface/enums.h"
#include "names.h"
#include "sqlhelper.h"
#include "mymoneydbdriver.h"
#include "mymoneydbdef.h"

namespace Storage {
namespace Sql {

const QString kmmTransactionsTableName = QStringLiteral("kmmTransactions");
const QString kmmKeyValuePairsTableName = QStringLiteral("kmmKeyValuePairs");
const QString kmmFileInfoTableName = QStringLiteral("kmmFileInfo");
const QString kmmSplitsTableName = QStringLiteral("kmmSplits");
const QString kmmAccountsTableName = QStringLiteral("kmmAccounts");
const QString originColumnName = QStringLiteral("origin");
const QString idColumnName = QStringLiteral("id");
const QString txTypeColumnName = QStringLiteral("txType");
const QString transactionIdColumnName = QStringLiteral("transactionId");
const QString accountIdColumnName = QStringLiteral("accountId");
const QString kvpTypeColumnName = QStringLiteral("kvpType");
const QString kvpKeyColumnName = QStringLiteral("kvpKey");
const QString kvpIdColumnName = QStringLiteral("kvpId");
const QString kvpDataColumnName = QStringLiteral("kvpData");
const QString transactionTypeValue = QStringLiteral("TRANSACTION");
const QString splitTypeValue = QStringLiteral("SPLIT");
const QString importedTransactionValue = QStringLiteral("Imported");
const QString kmmMatchedTxValue = QStringLiteral("kmm-matched-tx");

class ConverterPrivate
{
  Q_DISABLE_COPY(ConverterPrivate)

public:
  ConverterPrivate() = default;

  void signalProgress(int current, int total, const QString &msg = QString()) const;
  static void updateKmmFileInfoCounts(QSqlDatabase &db);
  static QMap<QString, QString> keyValueMapOfElement(QSqlDatabase &db, const QString &kvpType, const QString &kvpId);

  static auto convertFunctionsKMyMoney();
  static void convertFileInfoToKMM(QSqlDatabase &db);
  static void packMatchedTransactions(QSqlDatabase &db);

  static auto convertFunctionsKMyMoneyNEXT();
  static void convertFileInfoToKMMN(QSqlDatabase &db);
  static void extractMatchedTransactions(QSqlDatabase &db);

  static auto convertFunctions(eProducer storageProducer);

  Storage::ProgressCallback m_callback = nullptr;
};

void ConverterPrivate::signalProgress(int current, int total, const QString &msg) const
{
  if (m_callback)
    m_callback(current, total, msg);
}

void ConverterPrivate::updateKmmFileInfoCounts(QSqlDatabase &db)
{
  QString queryString;
  QSqlQuery selectQuery(db);
  QSqlQuery updateQuery(db);

  // update transaction counts in kmmAccounts
  queryString = QString::fromLatin1("SELECT %1, COUNT(*) FROM %2 WHERE %3 = 'N' GROUP BY %1;").
                arg(accountIdColumnName).
                arg(kmmSplitsTableName).
                arg(txTypeColumnName);
  selectQuery.exec(queryString);
  while (selectQuery.next()) {
    auto accountId = selectQuery.value(0).toString();
    auto transactionsCount = selectQuery.value(1).toUInt();

    queryString = QString::fromLatin1("UPDATE %1 SET %2 = '%3' WHERE %4 = '%5';").
                  arg(kmmAccountsTableName).
                  arg("transactionCount").
                  arg(transactionsCount).
                  arg(idColumnName).
                  arg(accountId);
    updateQuery.exec(queryString);
  }

  queryString = QString::fromLatin1("SELECT %1 FROM %2 ORDER BY %1 DESC").
                arg(idColumnName).
                arg(kmmTransactionsTableName);
  selectQuery.exec(queryString);
  selectQuery.first();
  auto lastTransactionID = selectQuery.value(0).toString().mid(1).toULong();

  const QMap<QString, QString> countsToUpdate {
    {kmmKeyValuePairsTableName, QStringLiteral("kvps")},
    {kmmSplitsTableName, QStringLiteral("splits")}
  };

  for (auto it = countsToUpdate.cbegin(); it != countsToUpdate.cend(); ++it) {
    queryString = QString::fromLatin1("SELECT COUNT(*) FROM %1;").arg(it.key());
    selectQuery.exec(queryString);
    selectQuery.first();
    auto count = selectQuery.value(0).toUInt();

    queryString = QString::fromLatin1("UPDATE %1 SET %2 = '%3';").
                  arg(kmmFileInfoTableName).
                  arg(it.value()).
                  arg(count);
    updateQuery.exec(queryString);
  }

  queryString = QString::fromLatin1("SELECT COUNT(*) FROM %1 WHERE %2 = 'N';").
                arg(kmmTransactionsTableName).
                arg(txTypeColumnName);
  selectQuery.exec(queryString);
  selectQuery.first();
  auto transactionsCount = selectQuery.value(0).toUInt();

  queryString = QString::fromLatin1("UPDATE %1 SET %2 = '%3', %4 = '%5';").
                arg(kmmFileInfoTableName).
                arg("transactions").
                arg(transactionsCount).
                arg("hiTransactionId").
                arg(lastTransactionID + 1);
  updateQuery.exec(queryString);
}

QMap<QString, QString> ConverterPrivate::keyValueMapOfElement(QSqlDatabase &db, const QString &kvpType, const QString &kvpId)
{
  QMap <QString, QString> kvpMap;
  QSqlQuery query(db);
  auto queryString = QString::fromLatin1("SELECT * FROM %1 WHERE %2 = '%3' AND %4 = '%5';").
                     arg(kmmKeyValuePairsTableName).
                     arg(kvpTypeColumnName).
                     arg(kvpType).
                     arg(kvpIdColumnName).
                     arg(kvpId);
  query.exec(queryString);
  if (query.next()) {
    const auto kvpKey = query.value(kvpKeyColumnName).toString();
    const auto kvpData = query.value(kvpDataColumnName).toString();
    kvpMap.insert(kvpKey, kvpData);
  }
  return kvpMap;
}

auto ConverterPrivate::convertFunctionsKMyMoney()
{
  const QVector<void(*)(QSqlDatabase &)> convertFunctions {
    packMatchedTransactions,
    updateKmmFileInfoCounts,
    convertFileInfoToKMM
  };

  return convertFunctions;
}

void ConverterPrivate::convertFileInfoToKMM(QSqlDatabase &db)
{
  const QString kmmFileInfo = QStringLiteral("kmmFileInfo");
  QSqlQuery query(db);
  // drop fixLevel column from kmmFileInfo
  const QString fixLevelColumnName = QStringLiteral("fixLevel");
  if (!db.record(kmmFileInfo).contains(fixLevelColumnName)) {
    auto driver = MyMoneyDbDriver::create(db.driverName());
    auto column = MyMoneyDbIntColumn(fixLevelColumnName, MyMoneyDbIntColumn::MEDIUM, false);

    auto queryString = QString::fromLatin1("ALTER TABLE %1 ADD %2;").
                  arg(kmmFileInfo).
                  arg(column.generateDDL(driver));
    query.exec(queryString);
    query.finish();
  }

  auto queryString = QString::fromLatin1("UPDATE %1 SET %2 = '%3';").
                arg(kmmFileInfo).
                arg(fixLevelColumnName).
                arg(5);
  query.exec(queryString);
  query.finish();

  SqlHelper::setVersion(db, 12);
}

void ConverterPrivate::packMatchedTransactions(QSqlDatabase &db)
{  
  QSqlQuery selectQuery(db);
  QSqlQuery updateQuery(db);
  QSqlQuery deleteQuery(db);
  QSqlQuery insertQuery(db);
  QSqlQuery alterQuery(db);

  QString queryString;
  queryString = QString::fromLatin1("SELECT %1, %2 FROM %3;").
                arg(idColumnName).
                arg(originColumnName).
                arg(kmmTransactionsTableName);

  QSqlQuery transactionQuery(db);
  transactionQuery.exec(queryString);

  // mark imported transactions sequentially now, because latter the transactions flow could be random
  while (transactionQuery.next()) {
    const auto transactionOriginAttribute = transactionQuery.value(originColumnName).toInt();
    const auto transactionId = transactionQuery.value(idColumnName).toString();

    // restore imported key value pair fo transaction
    if (transactionOriginAttribute & eMyMoney::Transaction::Origin::Imported) {

      const QMap<QString, QString> fieldValueMap = {
        {"kvpType", "TRANSACTION"},
        {"kvpId", transactionId},
        {"kvpKey", "Imported"},
        {"kvpData", "true"}
      };

      queryString = QString::fromLatin1("INSERT INTO %1 (%2) VALUES ('%3')").
                    arg(kmmKeyValuePairsTableName).
                    arg(fieldValueMap.keys().join(',')).
                    arg(fieldValueMap.values().join("','"));
      insertQuery.exec(queryString);
    }
  }

  queryString = QString::fromLatin1("SELECT * FROM %1 WHERE %2 = '%3';").
                arg(kmmKeyValuePairsTableName).
                arg(kvpKeyColumnName).
                arg("kmm-match-transaction");

  QSqlQuery kvpQuery(db);
  kvpQuery.exec(queryString);
  while (kvpQuery.next()) {
    auto kvpData = kvpQuery.value(kvpDataColumnName).toString();
    auto sourceTransactionId = kvpData.split(';').at(0);
    auto embeddedTransactionId = kvpData.split(';').at(1);
    auto transactionIdWithSplit = kvpQuery.value(kvpIdColumnName).toString();
    auto transactionId = transactionIdWithSplit;
    transactionId.chop(1);

    queryString = QString::fromLatin1("SELECT * FROM %1 WHERE %2 = '%3' AND %4 = '%5';").
                  arg(kmmKeyValuePairsTableName).
                  arg(kvpKeyColumnName).
                  arg("kmm-match-split").
                  arg(kvpIdColumnName).
                  arg(transactionIdWithSplit);
    selectQuery.exec(queryString);
    selectQuery.first();
    kvpData = selectQuery.value(kvpDataColumnName).toString();
    auto sourceSplitId = kvpData.split(';').at(0).mid(1).toInt() - 1; // S0001;S0002 -> S0001 -> 0001 -> 1 -> 0
    auto embeddedSplitId = kvpData.split(';').at(1).mid(1).toInt() - 1; // S0001;S0002 -> S0002 -> 0002 -> 2 -> 1

    // get embedded transaction attributes
    QXmlStreamAttributes embeddedTransactionAttributes;
    queryString = QString::fromLatin1("SELECT * FROM %1 WHERE %2 = '%3';").
                  arg(kmmTransactionsTableName).
                  arg(idColumnName).
                  arg(embeddedTransactionId);
    selectQuery.exec(queryString);
    selectQuery.first();

    QMap<QString, QString> transactionAttributesMap {
      {"postDate", "postdate"},
      {"memo", "memo"},
      {"entryDate", "entrydate"},
      {"currencyId", "commodity"},
    };

    for (auto it = transactionAttributesMap.cbegin(); it != transactionAttributesMap.cend() ; ++it ) {
      const auto attributeName = it.value();
      const auto attributeValue = selectQuery.value(it.key()).toString();
      embeddedTransactionAttributes.append(attributeName, attributeValue);
    }

    // get embedded transaction key-value pairs
    QMap <QString, QString> embeddedTransactionKvpMap;
    queryString = QString::fromLatin1("SELECT * FROM %1 WHERE %2 = '%3' AND %4 = '%5';").
                  arg(kmmKeyValuePairsTableName).
                  arg(kvpTypeColumnName).
                  arg("TRANSACTION").
                  arg(kvpIdColumnName).
                  arg(embeddedTransactionId);
    selectQuery.exec(queryString);
    if (selectQuery.next()) {
      const auto kvpKey = selectQuery.value(kvpKeyColumnName).toString();
      const auto kvpData = selectQuery.value(kvpDataColumnName).toString();
      embeddedTransactionKvpMap.insert(kvpKey, kvpData);
    }

    // get embedded transaction splits
    QVector<QXmlStreamAttributes> embeddedTransactionSplits;
    QVector<QMap <QString, QString>> embeddedTransactionSplitKvps;
    queryString = QString::fromLatin1("SELECT * FROM %1 WHERE %2 = '%3';").
                  arg(kmmSplitsTableName).
                  arg(transactionIdColumnName).
                  arg(embeddedTransactionId);
    selectQuery.exec(queryString);

    QMap<QString, QString> splitAttributesMap {
      {"payeeId", "payee"},
      {"reconcileDate", "reconciledate"},
      {"action", "action"},
      {"reconcileFlag", "reconcileflag"},
      {"value", "value"},
      {"shares", "shares"},
      {"price", "price"},
      {"memo", "memo"},
      {"accountId", "account"},
      {"postDate", "postdate"},
      {"bankId", "bankid"},
    };

    if (selectQuery.next()) {
      QXmlStreamAttributes embeddedTransactionSplitAttributes;
      for (auto it = splitAttributesMap.cbegin(); it != splitAttributesMap.cend() ; ++it ) {
        const auto attributeName = it.value();
        const auto attributeValue = selectQuery.value(it.key()).toString();
        embeddedTransactionSplitAttributes.append(attributeName, attributeValue);
      }
      embeddedTransactionSplits.append(embeddedTransactionSplitAttributes);

      auto splitId = selectQuery.value("splitId").toInt();
      auto splitKvpId = QString::fromLatin1("%1%2").arg(embeddedTransactionId, splitId);
      auto embeddedTransactionSplitKvpMap = keyValueMapOfElement(db, "SPLIT", splitKvpId);
      embeddedTransactionSplitKvps.append(embeddedTransactionSplitKvpMap);
    }

    QString embeddedTransactionXml;
    QXmlStreamWriter xmlWriter(&embeddedTransactionXml);


    xmlWriter.writeDTD("<DOCTYPE MATCH>");
    xmlWriter.writeStartElement("CONTAINER");
    xmlWriter.writeStartElement("TRANSACTION");
    xmlWriter.writeAttributes(embeddedTransactionAttributes);

    xmlWriter.writeStartElement("SPLITS");
    for (auto i = 0 ; i < embeddedTransactionSplits.length(); ++i) {
      xmlWriter.writeStartElement("SPLIT");
      auto &splitAttributes = embeddedTransactionSplits.at(i);
      xmlWriter.writeAttributes(splitAttributes);
      auto splitKvpsMap = embeddedTransactionSplitKvps.at(i);
      if (!splitKvpsMap.isEmpty()) {
        xmlWriter.writeStartElement("KEYVALUEPAIRS");
        for (auto it = splitKvpsMap.cbegin(); it != splitKvpsMap.cend(); ++it) {
          xmlWriter.writeStartElement("PAIR");
          xmlWriter.writeAttribute("key", it.key());
          xmlWriter.writeAttribute("value", it.value());
          xmlWriter.writeEndElement(); // PAIR
        }
        xmlWriter.writeEndElement(); // KEYVALUEPAIRS
      }
      xmlWriter.writeEndElement(); // SPLIT
    }
    xmlWriter.writeEndElement(); // SPLITS
    xmlWriter.writeEndElement(); // TRANSACTION
    xmlWriter.writeEndElement(); // CONTAINER

    embeddedTransactionXml.replace(QStringLiteral("<"), QStringLiteral("&#60;"));

    // backup source transaction values before this transaction will disappear completely
    QMap<QString, QString> keyValueMap;
    keyValueMap.insert("kmm-matched-tx", embeddedTransactionXml);

    queryString = QString::fromLatin1("SELECT * FROM %1 WHERE %2 = '%3' AND %4 = '%5';").
                  arg(kmmSplitsTableName).
                  arg(transactionIdColumnName).
                  arg(sourceTransactionId).
                  arg("splitId").
                  arg(sourceSplitId);
    selectQuery.exec(queryString);
    selectQuery.first();

    auto sourcePostDate = selectQuery.value("postDate").toString();
    if (!sourcePostDate.isEmpty())
      keyValueMap.insert("kmm-orig-postdate", sourcePostDate);

    auto sourceMemo = selectQuery.value("memo").toString();
    if (!sourceMemo.isEmpty())
      keyValueMap.insert("kmm-orig-memo", sourceMemo);

    auto sourcePayee = selectQuery.value("payeeId").toString();
    if (sourcePayee.isEmpty()) // Lack of exclamation mark is not a typo. See transactionmatcher.cpp
      keyValueMap.insert("kmm-orig-payee", sourcePayee);

    keyValueMap.insert("kmm-match-split", QString::fromLatin1("S%1").arg(embeddedSplitId + 1, 4, 10, QChar('0')));

    // delete old key-value pairs to avoid duplicates
    const QStringList kvpIdsToDelete {
      "kmm-match-split",
      "kmm-match-transaction"
    };

    for (const auto &kvpIdToDelete : kvpIdsToDelete) {
      queryString = QString::fromLatin1("DELETE FROM %1 WHERE %2 = '%3' AND %4 = '%5';").
                    arg(kmmKeyValuePairsTableName).
                    arg(kvpKeyColumnName).
                    arg(kvpIdToDelete).
                    arg(kvpIdColumnName).
                    arg(transactionIdWithSplit);
      deleteQuery.exec(queryString);
    }


    for (auto it = keyValueMap.cbegin(); it != keyValueMap.cend(); ++it) {
      QMap<QString, QString> fieldValueMap {
        {"kvpType", "SPLIT"},
        {"kvpId", transactionIdWithSplit},
        {"kvpKey", it.key()},
        {"kvpData", it.value()}
      };

      queryString = QString::fromLatin1("INSERT INTO %1 (%2) VALUES ('%3')").
                    arg(kmmKeyValuePairsTableName).
                    arg(fieldValueMap.keys().join(',')).
                    arg(fieldValueMap.values().join("','"));
      insertQuery.exec(queryString);
    }


    // delete matching inputs
    const QStringList transactionIdsToDelete {
      sourceTransactionId,
      embeddedTransactionId
    };

    for (const auto &transactionIdToDelete : transactionIdsToDelete) {
      queryString = QString::fromLatin1("DELETE FROM %1 WHERE %2 = '%3'").
                    arg(kmmTransactionsTableName).
                    arg(idColumnName).
                    arg(transactionIdToDelete);
      deleteQuery.exec(queryString);
      deleteQuery.finish();

      queryString = QString::fromLatin1("DELETE FROM %1 WHERE %2 = '%3'").
                    arg(kmmSplitsTableName).
                    arg(transactionIdColumnName).
                    arg(transactionIdToDelete);
      deleteQuery.exec(queryString);
      deleteQuery.finish();

    }
  }

  // dropColumn doesn't work if any of the queries is still active
  selectQuery.finish();
  updateQuery.finish();
  deleteQuery.finish();
  insertQuery.finish();
  alterQuery.finish();
  transactionQuery.finish();
  kvpQuery.finish();

  SqlHelper::dropColumn(db, kmmTransactionsTableName, originColumnName);
}

auto ConverterPrivate::convertFunctionsKMyMoneyNEXT()
{
  const QVector<void(*)(QSqlDatabase &)> convertFunctions {
    extractMatchedTransactions,
    convertFileInfoToKMMN
  };
  return convertFunctions;
}

void ConverterPrivate::convertFileInfoToKMMN(QSqlDatabase &db)
{
  const QString kmmFileInfo = QStringLiteral("kmmFileInfo");
  QSqlQuery query(db);
  // drop fixLevel column from kmmFileInfo
  const QString fixLevelColumnName = QStringLiteral("fixLevel");
  SqlHelper::dropColumn(db, kmmFileInfo, fixLevelColumnName);
  SqlHelper::setVersion(db, 20200101);
}

/**
 * @brief upgradeToV13
 * Changes (2018-08-19):
 * 1) make two separate transactions out of two matched transactions
 * 2) create a new tramsaction as a result of matching
 * 3) add origin (imported, typed, matched) of transaction
 * 4) add kmm-match-transaction containing two matched transaction IDs to every matched split
 * 5) add kmm-match-split containing two matched split IDs to every matched split
 * @return 0 if successful, 1 if not successful
 */
void ConverterPrivate::extractMatchedTransactions(QSqlDatabase &db)
{
  QSqlQuery selectQuery(db);
  QSqlQuery updateQuery(db);
  QSqlQuery deleteQuery(db);
  QSqlQuery insertQuery(db);
  QSqlQuery alterQuery(db);

  QString queryString;

  if (!db.record(kmmTransactionsTableName).contains(originColumnName)) {
    auto driver = MyMoneyDbDriver::create(db.driverName());
    auto column = MyMoneyDbIntColumn(originColumnName, MyMoneyDbIntColumn::TINY, false);

    queryString = QString::fromLatin1("ALTER TABLE %1 ADD %2;").
                  arg(kmmTransactionsTableName).
                  arg(column.generateDDL(driver));
    alterQuery.exec(queryString);
  }

  // we need to know the highest unoccupied ID to know what IDs can we assign to new (reorganized) transactions
  queryString = QString::fromLatin1("SELECT %1 FROM %2 ORDER BY %1 DESC").
                arg(idColumnName).
                arg(kmmTransactionsTableName);
  selectQuery.exec(queryString);

  // no transactions = no problem
  if (!selectQuery.first())
    return;

  auto lastTransactionID = selectQuery.value(0).toString().mid(1).toULong();

  // set default transaction origin for every transaction
  queryString  = QString::fromLatin1("UPDATE %1 SET %2 = '%3'").
                 arg(kmmTransactionsTableName).
                 arg(originColumnName).
                 arg(QString::number(static_cast<int>(eMyMoney::Transaction::Origin::Typed)));

  updateQuery.exec(queryString);

  // correct transaction origin if it has been imported
  queryString = QString::fromLatin1("SELECT %1 FROM %2 WHERE %3 = '%4' AND %5 = '%6';").
                arg(kvpIdColumnName).
                arg(kmmKeyValuePairsTableName).
                arg(kvpTypeColumnName).
                arg(transactionTypeValue).
                arg(kvpKeyColumnName).
                arg(importedTransactionValue);
  selectQuery.exec(queryString);


  while (selectQuery.next()) {
    auto transactionID = selectQuery.value(0).toString();

    queryString = QString::fromLatin1("UPDATE %1 SET %2 = '%3' WHERE %4 = '%5';").
                  arg(kmmTransactionsTableName).
                  arg(originColumnName).
                  arg(QString::number(static_cast<int>(eMyMoney::Transaction::Origin::Imported))).
                  arg(idColumnName).
                  arg(transactionID);
    updateQuery.exec(queryString);
  }

  queryString = QString::fromLatin1("DELETE FROM %1 WHERE %2 = '%3'").
                arg(kmmKeyValuePairsTableName).
                arg(kvpKeyColumnName).
                arg(importedTransactionValue);
  deleteQuery.exec(queryString);

  // search for embedded tranactions
  QSqlQuery queryMatchedTX(db);
  queryString = QString::fromLatin1("SELECT * FROM %1 WHERE %2 = '%3' AND %4 = '%5';").
                                    arg(kmmKeyValuePairsTableName).
                                    arg(kvpTypeColumnName).
                                    arg(splitTypeValue).
                                    arg(kvpKeyColumnName).
                                    arg(kmmMatchedTxValue);
  queryMatchedTX.exec(queryString);

  while (queryMatchedTX.next()) {
    auto transactionIDAndSplitID = queryMatchedTX.value(kvpIdColumnName).toString();
    auto transactionID = transactionIDAndSplitID.left(19); // transaction id size

    // unembedd imported transaction
    auto embeddedTransactionXML = queryMatchedTX.value(kvpDataColumnName).toString();

    const QVector<QString> possibleDocumentStartStrings {
      QStringLiteral("&lt;"), QStringLiteral("&#60;")
    };
    for (const auto &possibleDocumentStartString : possibleDocumentStartStrings) {
      if (embeddedTransactionXML.startsWith(possibleDocumentStartString)) {
        embeddedTransactionXML.replace(possibleDocumentStartString, QLatin1String("<"));
        break;
      }
    }

    QXmlStreamReader reader(embeddedTransactionXML);
    QXmlStreamAttributes embeddedTransactionAttributes;
    QString embeddedTransactionID;

    auto isEmbeddedTransactionImported = false;
    // start parsing transaction in XML
    reader.readNextStartElement(); // this reads container
    while (reader.readNextStartElement()) {
      if (reader.name() == "TRANSACTION") {
        embeddedTransactionAttributes = reader.attributes();
        embeddedTransactionID = QString::fromLatin1("T%1").arg(QString::number(++lastTransactionID).rightJustified(18 , '0'));

        while (reader.readNextStartElement()) {
          if (reader.name() == "SPLITS") {
            while (reader.readNextStartElement()) {
              if (reader.name() == "SPLIT") {
                auto embeddedTransactionSplitAttributes = reader.attributes();
                auto embeddedSplitIDConvertedToSQL = QString::number(embeddedTransactionSplitAttributes.value("id").toString().mid(1).toInt() - 1);

                // SPLIT node has only key-value pairs node as subnode, so read it before all
                while (reader.readNextStartElement()) {
                  if (reader.name() == "KEYVALUEPAIRS") {
                    while (reader.readNextStartElement()) {
                      // inserting all key-value pairs of embedded transaction without filtering anything out might be dangerous
                      if (reader.name() == "PAIR") {
                        auto embeddedTransactionSplitKVPAttributes = reader.attributes();
                        const QStringList columnValuesInKVP = {
                          "SPLIT",
                          QString::fromLatin1("%1%2").arg(embeddedTransactionID, embeddedSplitIDConvertedToSQL),
                          embeddedTransactionSplitKVPAttributes.value("key").toString(),
                          embeddedTransactionSplitKVPAttributes.value("value").toString()
                        };
                        queryString = QString::fromLatin1("INSERT INTO %1 VALUES ('%2')").
                                      arg(kmmKeyValuePairsTableName).
                                      arg(columnValuesInKVP.join("','"));
                        insertQuery.exec(queryString);
                      }
                      reader.skipCurrentElement();
                    }
                  } else {
                    reader.skipCurrentElement();
                  }
                }

                // we use highest precision for formatted values because we don't know precise precision yet
                const auto valueFormatted = MyMoneyMoney(embeddedTransactionSplitAttributes.value("value").toString()).formatMoney(QString(), -1, false).replace(QChar(','), QChar('.'));
                const auto priceFormatted = MyMoneyMoney(embeddedTransactionSplitAttributes.value("price").toString()).formatMoney(QString(), 8, false).replace(QChar(','), QChar('.'));
                const auto sharesFormatted = MyMoneyMoney(embeddedTransactionSplitAttributes.value("shares").toString()).formatMoney(QString(), 8, false).replace(QChar(','), QChar('.'));

                // we unembedd split 1:1 except its ID an transaction ID
                const QStringList columnValuesInSplit = {
                  embeddedTransactionID,
                  "N",
                  embeddedSplitIDConvertedToSQL,
                  embeddedTransactionSplitAttributes.value("payee").toString(),
                  embeddedTransactionSplitAttributes.value("reconciledate").toString(),
                  embeddedTransactionSplitAttributes.value("action").toString(),
                  embeddedTransactionSplitAttributes.value("reconcileflag").toString(),
                  embeddedTransactionSplitAttributes.value("value").toString(),
                  valueFormatted,
                  embeddedTransactionSplitAttributes.value("shares").toString(),
                  sharesFormatted,
                  embeddedTransactionSplitAttributes.value("price").toString(),
                  priceFormatted,
                  embeddedTransactionSplitAttributes.value("memo").toString(),
                  embeddedTransactionSplitAttributes.value("account").toString(),
                  QString(),
                  QString(),
                  embeddedTransactionAttributes.value("postdate").toString(),
                  embeddedTransactionAttributes.value("bankid").toString()
                };

                queryString = QString::fromLatin1("INSERT INTO %1 VALUES ('%2')").
                              arg(kmmSplitsTableName).
                              arg(columnValuesInSplit.join("','"));
                insertQuery.exec(queryString);

              } else {
                reader.skipCurrentElement();
              }
            }

            // key-value pairs node as a subnode of transaction
          } else if (reader.name() == "KEYVALUEPAIRS") {
            while (reader.readNextStartElement()) {
              if (reader.name() == "PAIR") {
                auto embeddedTransactionKVPAttributes = reader.attributes();
                const QStringList columnValuesInKVP = {
                  "TRANSACTION",
                  embeddedTransactionID,
                  embeddedTransactionKVPAttributes.value("key").toString(),
                  embeddedTransactionKVPAttributes.value("value").toString()
                };

                // we don't want Imported key to be unembedded because it's replaced by origin attribute
                if (embeddedTransactionKVPAttributes.value("key") == importedTransactionValue) {
                  isEmbeddedTransactionImported = true;
                } else {
                  queryString = QString::fromLatin1("INSERT INTO %1 VALUES ('%2')").
                                arg(kmmKeyValuePairsTableName).
                                arg(columnValuesInKVP.join("','"));
                  insertQuery.exec(queryString);
                }
              } else {
                reader.skipCurrentElement();
              }

            }
          } else {
            reader.skipCurrentElement();
          }
        }
      } else {
        reader.skipCurrentElement();
      }
    }

    // inserting embedded transaction as 1:1 except its ID
    QStringList columnValuesInTransaction = {
      embeddedTransactionID,
      "N",
      embeddedTransactionAttributes.value("postdate").toString(),
      embeddedTransactionAttributes.value("memo").toString(),
      embeddedTransactionAttributes.value("entrydate").toString(),
      embeddedTransactionAttributes.value("commodity").toString(),
      QString(),
      isEmbeddedTransactionImported ?
      QString::number(static_cast<int>(eMyMoney::Transaction::Origin::Imported | eMyMoney::Transaction::Origin::MatchingInput)) :
      QString::number(static_cast<int>(eMyMoney::Transaction::Origin::Typed | eMyMoney::Transaction::Origin::MatchingInput))
    };

    queryString = QString::fromLatin1("INSERT INTO %1 VALUES ('%2');").
                  arg(kmmTransactionsTableName).
                  arg(columnValuesInTransaction.join("','"));

    insertQuery.exec(queryString);

    // we already unembedded end transaction that served as input for matching
    // and now we'll restore original state of start transaction that also served as input for matching
    // from matched transaction that is output from matching
    queryString = QString::fromLatin1("SELECT * FROM %1 WHERE %2 = '%3';").
                  arg(kmmTransactionsTableName).
                  arg(idColumnName).
                  arg(transactionID);

    selectQuery.exec(queryString);
    selectQuery.first();

    auto sourceTransactionRecord = selectQuery.record();
    auto sourceTransactionID = QString::fromLatin1("T%1").arg(QString::number(++lastTransactionID).rightJustified(18 , '0'));
    sourceTransactionRecord.setValue(idColumnName, QVariant(sourceTransactionID));
    auto sourceTransactionOrigin = sourceTransactionRecord.value(originColumnName).toInt();
    sourceTransactionOrigin = sourceTransactionOrigin | eMyMoney::Transaction::Origin::MatchingInput;
    sourceTransactionRecord.setValue(originColumnName, QVariant(sourceTransactionOrigin));

    // in matched transaction splits there are backup values of start transaction, so iterate over them
    queryString = QString::fromLatin1("SELECT * FROM %1 WHERE %2 = '%3';").
                  arg(kmmSplitsTableName).
                  arg(transactionIdColumnName).
                  arg(transactionID);
    selectQuery.exec(queryString);

    while (selectQuery.next()) {
      auto sourceTransactionSplitRecord = selectQuery.record();
      auto transactionSplitID = sourceTransactionSplitRecord.value("splitId").toInt();
      sourceTransactionSplitRecord.setValue(transactionIdColumnName, QVariant(sourceTransactionID));

      // backup values of start transaction are stored in key-value pairs of split
      QSqlQuery kvpQuery(db);
      queryString = QString::fromLatin1("SELECT * FROM %1 WHERE %2 = '%3' AND %4 = '%5%6';").
                    arg(kmmKeyValuePairsTableName).
                    arg(kvpTypeColumnName).
                    arg(splitTypeValue).
                    arg(kvpIdColumnName).
                    arg(transactionID).
                    arg(QString::number(transactionSplitID));
      kvpQuery.exec(queryString);

      // store key-value pairs because we will deltete them from database now and will be filtering them out later on
      QMap<QString, QVariant> keyValueMap;
      while (kvpQuery.next()) {
        auto kvpKey = kvpQuery.value(kvpKeyColumnName).toString();
        auto kvpData = kvpQuery.value(kvpDataColumnName);
        keyValueMap.insert(kvpKey, kvpData);
      }

      queryString = QString::fromLatin1("DELETE FROM %1 WHERE %2 = '%3' AND %4 = '%5%6';").
                    arg(kmmKeyValuePairsTableName).
                    arg(kvpTypeColumnName).
                    arg(splitTypeValue).
                    arg(kvpIdColumnName).
                    arg(transactionID).
                    arg(QString::number(transactionSplitID));
      deleteQuery.exec(queryString);

      // split containg kmm-match-split means that it was matched
      // otherwise it's a generic split and will be written only with new transaction ID
      if (keyValueMap.contains("kmm-match-split")) {

        // restore all original values from backup for source transaction and its split
        if (keyValueMap.contains("kmm-orig-postdate")) {
          sourceTransactionRecord.setValue("postDate", keyValueMap.value("kmm-orig-postdate"));
          keyValueMap.remove("kmm-orig-postdate");
        }

        auto sourceTransactionSplitID = keyValueMap.value("kmm-match-split").toString().mid(1).toInt() - 1;
        if (transactionSplitID == sourceTransactionSplitID) {
          if (keyValueMap.contains("kmm-orig-memo")) {
            sourceTransactionSplitRecord.setValue("memo", keyValueMap.value("kmm-orig-memo"));
            keyValueMap.remove("kmm-orig-memo");
          }
          if (keyValueMap.contains("kmm-orig-payee")) {
            sourceTransactionSplitRecord.setValue("payee", keyValueMap.value("kmm-orig-payee"));
            keyValueMap.remove("kmm-orig-payee");
          }
        }

        // add key-value pairs according to new way of storing information about matched split
        const QVector<QStringList> splitsToAdd {
          {
            "SPLIT",
            QString::fromLatin1("%1%2").arg(transactionID, QString::number(transactionSplitID)),
                "kmm-match-transaction",
                QString::fromLatin1("%1;%2").arg(sourceTransactionID, embeddedTransactionID)
          },

          {
            "SPLIT",
            QString::fromLatin1("%1%2").arg(transactionID, QString::number(transactionSplitID)),
                "kmm-match-split",
                QString::fromLatin1("S%1;S%2").arg(transactionSplitID + 1, 4, 10, QChar('0')).arg(sourceTransactionSplitID + 1, 4, 10, QChar('0'))
          }
        };

        for (const auto &splitToAdd : splitsToAdd) {
          queryString = QString::fromLatin1("INSERT INTO %1 VALUES ('%2');").
                        arg(kmmKeyValuePairsTableName).
                        arg(splitToAdd.join("','"));
          insertQuery.exec(queryString);
        }
      }

      QStringList columnValuesInSplit;
      for (auto i = 0; i < sourceTransactionSplitRecord.count(); ++i)
        columnValuesInSplit.append(sourceTransactionSplitRecord.value(i).toString());

      queryString = QString::fromLatin1("INSERT INTO %1 VALUES ('%2');").
                    arg(kmmSplitsTableName).
                    arg(columnValuesInSplit.join("','"));
      insertQuery.exec(queryString);

      // all key-value pairs were deleted from the database, so write only the usefull ones back
      QStringList columnValuesInKVP {
        "SPLIT",
        QString::fromLatin1("%1%2").arg(sourceTransactionID, QString::number(transactionSplitID)),
        QString(),
        QString()
      };

      for (auto it = keyValueMap.cbegin(); it != keyValueMap.cend(); ++it) {
        if (it.key() == "kmm-matched-tx" || it.key() == "kmm-match-split")
          continue;
        columnValuesInKVP.replace(2, it.key());
        columnValuesInKVP.replace(3, it.value().toString());
        queryString = QString::fromLatin1("INSERT INTO kmmKeyValuePairs VALUES ('%1')").arg(columnValuesInKVP.join("','"));
        insertQuery.exec(queryString);
      }
    }

    // inserting source transaction as it was just before matching
    columnValuesInTransaction = QStringList();
    for (auto i = 0; i < sourceTransactionRecord.count(); ++i)
      columnValuesInTransaction.append(sourceTransactionRecord.value(i).toString());

    queryString = QString::fromLatin1("INSERT INTO %1 VALUES ('%2');").
                  arg(kmmTransactionsTableName).
                  arg(columnValuesInTransaction.join("','"));
    insertQuery.exec(queryString);

    // origin of matched transaction is either typed or imported (unlikely), so add to it 'matching output' identifier
    queryString = QString::fromLatin1("SELECT %1 FROM %2 WHERE %3 = '%4';").
                  arg(originColumnName).
                  arg(kmmTransactionsTableName).
                  arg(idColumnName).
                  arg(transactionID);

    selectQuery.exec(queryString);
    selectQuery.first();
    const auto matchedTransactionOrigin = selectQuery.value(0).toInt() | eMyMoney::Transaction::Origin::MatchingOutput;
    queryString = QString::fromLatin1("UPDATE %1 SET %2 = '%3' WHERE %4 = '%5';").
                  arg(kmmTransactionsTableName).
                  arg(originColumnName).
                  arg(matchedTransactionOrigin).
                  arg(idColumnName).
                  arg(transactionID);
    updateQuery.exec(queryString);
  } // queryMatchedTX.next()

  // update transaction counts in kmmAccounts
  queryString = QString::fromLatin1("SELECT %1, COUNT(*) FROM %2 WHERE %3 = 'N' GROUP BY %1;").
                arg(accountIdColumnName).
                arg(kmmSplitsTableName).
                arg(txTypeColumnName);
  selectQuery.exec(queryString);
  while (selectQuery.next()) {
    auto accountId = selectQuery.value(0).toString();
    auto transactionsCount = selectQuery.value(1).toUInt();

    queryString = QString::fromLatin1("UPDATE %1 SET %2 = '%3' WHERE %4 = '%5';").
                  arg(kmmAccountsTableName).
                  arg("transactionCount").
                  arg(transactionsCount).
                  arg(idColumnName).
                  arg(accountId);
    updateQuery.exec(queryString);
  }

  // update counts in kmmFileInfo
  const QMap<QString, QString> countsToUpdate {
    {kmmKeyValuePairsTableName, QStringLiteral("kvps")},
    {kmmSplitsTableName, QStringLiteral("splits")}
  };

  for (auto it = countsToUpdate.cbegin(); it != countsToUpdate.cend(); ++it) {
    queryString = QString::fromLatin1("SELECT COUNT(*) FROM %1;").arg(it.key());
    selectQuery.exec(queryString);
    selectQuery.first();
    auto count = selectQuery.value(0).toUInt();

    queryString = QString::fromLatin1("UPDATE %1 SET %2 = '%3';").
                  arg(kmmFileInfoTableName).
                  arg(it.value()).
                  arg(count);
    updateQuery.exec(queryString);
  }

  queryString = QString::fromLatin1("SELECT COUNT(*) FROM %1 WHERE %2 = 'N';").
                arg(kmmTransactionsTableName).
                arg(txTypeColumnName);
  selectQuery.exec(queryString);
  selectQuery.first();
  auto transactionsCount = selectQuery.value(0).toUInt();

  queryString = QString::fromLatin1("UPDATE %1 SET %2 = '%3', %4 = '%5';").
                arg(kmmFileInfoTableName).
                arg("transactions").
                arg(transactionsCount).
                arg("hiTransactionId").
                arg(lastTransactionID + 1);
  updateQuery.exec(queryString);
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


bool Converter::convertStorage(QSqlDatabase &db, eProducer targetStorageProducer)
{
  Q_D(Converter);

  const auto functions = d->convertFunctions(targetStorageProducer);
  for (const auto &function : functions)
    function(db);

  return true;
}

} // namespace Sql
} // namespace Storage
