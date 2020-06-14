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

#include "comparator.h"

#include <memory>

// ----------------------------------------------------------------------------
// QT Includes

#include <QFileInfo>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlIndex>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "storage/interface/enums.h"
#include "storage/xml/backend/tests/comparator.h"

namespace Storage {
namespace Sql {

class ComparatorPrivate
{
public:
  bool isContentInRowEqual(const QSqlDatabase &firstDb, const QSqlDatabase &secondDb, const QString &tableName, const QMap<QString, QString> &recordId);
  bool areRowsInTableEqual(const QSqlDatabase &firstDb, const QSqlDatabase &secondDb, const QString &tableName);
  bool areTablesInStoragesEqual(const QSqlDatabase &firstDb, const QSqlDatabase &secondDb);

  QString m_firstDiff;
  QString m_secondDiff;
  QString m_tempDiff;
};

Comparator::Comparator() :
  d_ptr(new ComparatorPrivate)
{
}

Comparator::~Comparator() = default;

QString Comparator::addSuffixToFileName(const QString &filePath, const QString &suffixToAdd)
{
  const auto fileInfo = QFileInfo(filePath);
  const auto oldFilePath = fileInfo.path();
  const auto oldBaseName = fileInfo.baseName();
  const auto oldExtension = fileInfo.completeSuffix();
  return QString::fromLatin1("%1/%2%3.%4").arg(oldFilePath).arg(oldBaseName).arg(suffixToAdd).arg(oldExtension);
}

QString Comparator::changeFileExtension(const QString &filePath, const QString &newExtension)
{
  const auto fileInfo = QFileInfo(filePath);
  const auto oldFilePath = fileInfo.path();
  const auto oldBaseName = fileInfo.baseName();
  const auto oldExtension = fileInfo.completeSuffix();
  return QString::fromLatin1("%1/%2.%4").arg(oldFilePath).arg(oldBaseName).arg(newExtension);
}

void Comparator::saveDiffsToFile(const QString &firstFilePath, const QString &secondFilePath) const
{
  Q_D(const Comparator);
  const auto firstFileDiffFilePath = Comparator::changeFileExtension(firstFilePath, QStringLiteral("diff"));
  const auto secondFileDiffFilePath = Comparator::changeFileExtension(secondFilePath, QStringLiteral("diff"));

  const auto firstFileDiff = std::make_unique<QFile>(firstFileDiffFilePath);
  const auto secondFileDiff = std::make_unique<QFile>(secondFileDiffFilePath);

  firstFileDiff->open(QIODevice::WriteOnly);
  secondFileDiff->open(QIODevice::WriteOnly);

  firstFileDiff->write(d->m_firstDiff.toUtf8());
  secondFileDiff->write(d->m_secondDiff.toUtf8());
}

bool ComparatorPrivate::isContentInRowEqual(const QSqlDatabase &firstDb, const QSqlDatabase &secondDb, const QString &tableName, const QMap<QString, QString> &rowId)
{
  auto rowFieldValueMap = [&](const QSqlDatabase &db) {
    QStringList fieldNames;
    const auto record = db.record(tableName);
    for (auto i = 0; i < record.count(); ++i)
      fieldNames.append(record.fieldName(i));

    QMap<QString, QString> fieldValueMap;
    const auto primaryIndexName = db.primaryIndex(tableName).name();

    QString specificRecordCondition;
    for (auto it = rowId.cbegin(); it != rowId.cend(); ++it)
      specificRecordCondition.append(QString::fromLatin1("%1 = '%2'").arg(it.key()).arg(it.value()));

    const auto queryString = QString::fromLatin1("SELECT * FROM %1 WHERE %2;").
                             arg(tableName).
                             arg(specificRecordCondition);
    QSqlQuery query(db);
    query.exec(queryString);

    while (query.next())
      for (const auto &fieldName : fieldNames)
        fieldValueMap.insert(fieldName, query.value(fieldName).toString());

    return fieldValueMap;
  };

  bool areContentsEqual = true;

  auto firstContent = rowFieldValueMap(firstDb);
  auto secondContent = rowFieldValueMap(secondDb);

  QString diff;
  auto firstKeys = firstContent.keys();
  for (const auto& firstKey : firstKeys) {
    if (!secondContent.contains(firstKey)) {
      areContentsEqual &= false;
      diff.append(QString::fromLatin1("    field %1 doesn't exist\n").arg(firstKey));
    } else if (secondContent.value(firstKey) != firstContent.value(firstKey)) {

      auto firstAttributeValue = firstContent.value(firstKey);
      auto secondAttributeValue = secondContent.value(firstKey);
      if (!Xml::Comparator::areXmlAttributesEqual(firstAttributeValue, secondAttributeValue)) {
        areContentsEqual &= false;
        diff.append(QString::fromLatin1("    %1 = %2\n").arg(firstKey).arg(firstContent.value(firstKey)));
      } else {
        firstContent.remove(firstKey);
        secondContent.remove(firstKey);
      }
    } else {
      firstContent.remove(firstKey);
      secondContent.remove(firstKey);
    }
  }
  if (!secondContent.isEmpty())
    areContentsEqual &= false;

  m_tempDiff = diff;

  return areContentsEqual;
}

bool ComparatorPrivate::areRowsInTableEqual(const QSqlDatabase &firstDb, const QSqlDatabase &secondDb, const QString &tableName)
{
  auto tableRows = [&](const QSqlDatabase &db) {
    QStringList primaryIndexNames;
    auto primaryIndexes = db.primaryIndex(tableName);

    for (auto i = 0; i < primaryIndexes.count(); ++i)
      primaryIndexNames.append(primaryIndexes.fieldName(i));

    QSqlQuery query(db);

    // kmmFileInfo and kmmKeyValuePair do not contain any primary index...
    // ...so take any index as a reference
    if (primaryIndexNames.isEmpty()) {
      auto queryString = QString::fromLatin1("SELECT COUNT(*) FROM %1;").
                         arg(tableName);
      query.exec(queryString);
      query.first();
      auto maxIndexesUsedToDiscern = query.value(0).toInt(); // in case of kmmFileInfo it should be one
      if (maxIndexesUsedToDiscern > 3) // kmmKeyValuePair could grow in the future in number of columns so cap it safely
        maxIndexesUsedToDiscern = 3;

      const auto record = db.record(tableName);
      for (auto i = 0; i < record.count() && i < maxIndexesUsedToDiscern; ++i)
        primaryIndexNames.append(record.fieldName(i));
    }

    QVector<QMap<QString, QString>> records;
    auto queryString = QString::fromLatin1("SELECT %1 FROM %2;").
                       arg(primaryIndexNames.join(", ")).
                       arg(tableName);
    query.exec(queryString);

    while (query.next()) {
      QMap<QString, QString> record;
      for (const auto &primaryIndexName : primaryIndexNames)
        record.insert(primaryIndexName, query.value(primaryIndexName).toString());
      records.append(record);
    }

    return records;
  };

  auto firstRows = tableRows(firstDb);
  auto secondRows = tableRows(secondDb);

  bool areRowsEqual = true;

  QString diff;

  // in case the table doesn't contain any row
  if (!firstRows.length()) {
    auto firstRecords = firstDb.record(tableName);
    auto secondRecords = secondDb.record(tableName);
    for (auto firtsRecordIndex = 0;  firtsRecordIndex < firstRecords.count(); ++firtsRecordIndex) {
      auto firstFieldName = firstRecords.fieldName(firtsRecordIndex);
      if (!secondRecords.contains(firstFieldName)) {
        areRowsEqual &= false;
        diff.append(QString::fromLatin1("  record %1 doesn't exist\n").arg(firstFieldName));
      }
    }
  }

  for (auto firtsRowsIndex = 0;  firtsRowsIndex < firstRows.length();) {
    auto rowId = firstRows.at(firtsRowsIndex);
    auto secondRowsIndex = secondRows.indexOf(rowId);
    if (secondRowsIndex != - 1) {
      if (isContentInRowEqual(firstDb, secondDb,tableName, rowId)) {
        firstRows.removeAt(firtsRowsIndex);
        secondRows.removeAt(secondRowsIndex);
      } else {
        areRowsEqual &= false;
        QStringList recordIdAsList;
        for (auto it = rowId.cbegin(); it != rowId.cend(); ++it)
          recordIdAsList.append(QString::fromLatin1("%1 = %2").arg(it.key()).arg(it.value()));
        diff.append(QString::fromLatin1("  row %1 isn't equal\n").arg(recordIdAsList.join(',')));
        diff.append(m_tempDiff);
        m_tempDiff.clear();
        firtsRowsIndex++;
      }
    } else {
      areRowsEqual &= false;
      QStringList recordIdAsList;
      for (auto it = rowId.cbegin(); it != rowId.cend(); ++it)
        recordIdAsList.append(QString::fromLatin1("%1 = %2").arg(it.key()).arg(it.value()));
      diff.append(QString::fromLatin1("  row %1 doesn't exist\n").arg(recordIdAsList.join(',')));
      firtsRowsIndex++;
    }
  }

  if (!secondRows.isEmpty())
    areRowsEqual &= false;

  m_tempDiff = diff;

  return areRowsEqual;
}

bool ComparatorPrivate::areTablesInStoragesEqual(const QSqlDatabase &firstDb, const QSqlDatabase &secondDb)
{
  bool areTablesEqual = true;
  auto firstTables = firstDb.tables(QSql::Tables);
  auto secondTables = secondDb.tables(QSql::Tables);

  QString diff;
  for (auto firstIndex = 0;  firstIndex < firstTables.length();) {
    const auto &tableName = firstTables.at(firstIndex);
    auto secondIndex = secondTables.indexOf(tableName);
    if (secondIndex != - 1) {
      if (areRowsInTableEqual(firstDb, secondDb, tableName)) {
        firstTables.removeAt(firstIndex);
        secondTables.removeAt(secondIndex);
      } else {
        areTablesEqual &= false;
        diff.append(QString::fromLatin1("table %1 isn't equal\n").arg(tableName));
        diff.append(m_tempDiff);
        m_tempDiff.clear();
        firstIndex++;
      }
    } else {
      areTablesEqual &= false;
      diff.append(QString::fromLatin1("table %1 doesn't exist\n").arg(tableName));
      firstIndex++;
    }
  }

  if (!secondTables.isEmpty())
    areTablesEqual &= false;

  m_tempDiff = diff;
  return areTablesEqual;
}

bool Comparator::areStoragesEqual(const QSqlDatabase &firstDb, const QSqlDatabase &secondDb)
{
  Q_D(Comparator);
  bool areStoragesEqual = true;
  areStoragesEqual &= d->areTablesInStoragesEqual(firstDb, secondDb);
  d->m_firstDiff = d->m_tempDiff;
  d->m_tempDiff.clear();
  areStoragesEqual &= d->areTablesInStoragesEqual(secondDb, firstDb);
  d->m_secondDiff = d->m_tempDiff;

  return areStoragesEqual;
}

} // namespace Sql
} // namespace Storage
