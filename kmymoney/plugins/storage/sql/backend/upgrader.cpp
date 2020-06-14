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

#include <QMap>
#include <QVector>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "mymoney/mymoneyexception.h"

#include "storage/interface/icallback.h"
#include "storage/interface/enums.h"
#include "names.h"
#include "sqlhelper.h"
#include "mymoneydbdriver.h"
#include "mymoneydbdef.h"

namespace Storage {
namespace Sql {

class UpgraderPrivate
{
  Q_DISABLE_COPY(UpgraderPrivate)

public:
  UpgraderPrivate() = default;

  void signalProgress(int current, int total, const QString &msg = QString()) const;
  static void setFixLevel(QSqlDatabase &db, uint fixLevel);

  static auto upgradeFunctionsKMyMoney();
  static void upgradeFrom11004(QSqlDatabase &db);
  static void upgradeFrom12004(QSqlDatabase &db);
  static void upgradeFrom12005(QSqlDatabase &db);

  static auto upgradeFunctionsKMyMoneyNEXT();
  static void upgradeFrom20200101(QSqlDatabase &db);

  static auto upgradeFunctions(Storage::eProducer storageProducer);

  eProducer m_storageProducer;
  ProgressCallback m_callback = nullptr;
};

void UpgraderPrivate::signalProgress(int current, int total, const QString &msg) const
{
  if (m_callback)
    m_callback(current, total, msg);
}

void UpgraderPrivate::setFixLevel(QSqlDatabase &db, uint fixLevel)
{
  QSqlQuery query(db);
  auto queryString = QString::fromLatin1("UPDATE %1 SET %2 = %3;").
                     arg(tableName(Table::FileInfo)).
                     arg(columnName(Column::FileInfo::FixLevel)).
                     arg(fixLevel);
  query.exec(queryString);
}

auto UpgraderPrivate::upgradeFunctionsKMyMoney()
{
  QMap <unsigned int, void(*)(QSqlDatabase &db)> upgradeFunctions {
    {11004, upgradeFrom11004},
    {12004, upgradeFrom12004},
    {12005, upgradeFrom12005}
  };
  return upgradeFunctions;
}

void UpgraderPrivate::upgradeFrom11004(QSqlDatabase &db)
{
  QSqlQuery query(db);
  auto queryString = QString::fromLatin1("ALTER TABLE %1 ADD %2 %3;").
                     arg("kmmSchedules").
                     arg("lastDayInMonth").
                     arg("char(1) NOT NULL DEFAULT 'N'");
  query.exec(queryString);

  const auto roundingMethodColumnName = QStringLiteral("roundingMethod");
  const auto kmmSecuritiesTableName = QStringLiteral("kmmSecurities");

  const auto record = db.record("kmmSecurities");
  if (!record.contains(roundingMethodColumnName)) {
    auto driver = MyMoneyDbDriver::create(db.driverName());
    auto column = MyMoneyDbIntColumn("roundingMethod", MyMoneyDbIntColumn::SMALL, false, false, true, 11, 11, "7"); // 7 == AlkValue::RoundRound

    queryString = QString::fromLatin1("ALTER TABLE %1 ADD %2;").
                  arg(kmmSecuritiesTableName).
                  arg(column.generateDDL(driver));
    query.exec(queryString);
  }

  Sql::SqlHelper::setVersion(db, 12);
}

void UpgraderPrivate::upgradeFrom12004(QSqlDatabase &db)
{
  const QVector<QString> idsOfCurrenciesToChange {
    "XAG", "XAU", "XPD", "XPT"
  };

  const QString isoCodeColumnName = QStringLiteral("ISOcode");
  const QString smallestCashFractionColumnName = QStringLiteral("smallestCashFraction");
  const QString smallestAccountFractionColumnName = QStringLiteral("smallestAccountFraction");
  const QString kmmCurrenciesName = QStringLiteral("kmmCurrencies");
  QSqlQuery query(db);
  auto queryString = QString::fromLatin1("SELECT %1, %2, %3 FROM %4;").
                     arg(isoCodeColumnName).
                     arg(smallestCashFractionColumnName).
                     arg(smallestAccountFractionColumnName).
                     arg(kmmCurrenciesName);
  query.exec(queryString);
  QStringList valueUpdateQuery;
  while (query.next()) {
    const auto isoCode = query.value(isoCodeColumnName).toString();
    if (isoCode.isEmpty() || !idsOfCurrenciesToChange.contains(isoCode))
      continue;

    auto smallestCashFraction = query.value(smallestCashFractionColumnName).toString();
    auto smallestAccountFraction = query.value(smallestAccountFractionColumnName).toString();
    if (smallestCashFraction == smallestAccountFraction)
      continue;

    queryString = QString::fromLatin1("UPDATE %1 SET %2 = '%3' WHERE %4 = '%5';").
                  arg(kmmCurrenciesName).
                  arg(smallestAccountFractionColumnName).
                  arg(smallestCashFraction).
                  arg(isoCodeColumnName).
                  arg(isoCode);

    QSqlQuery updateQuery(db);
    updateQuery.exec(queryString);
  }

  setFixLevel(db, 5);
}

void UpgraderPrivate::upgradeFrom12005(QSqlDatabase &db)
{
  Q_UNUSED(db);
}

auto UpgraderPrivate::upgradeFunctionsKMyMoneyNEXT()
{
  QMap <unsigned int, void(*)(QSqlDatabase &db)> upgradeFunctions {
    {20200101, upgradeFrom20200101}
  };
  return upgradeFunctions;
}

void UpgraderPrivate::upgradeFrom20200101(QSqlDatabase &db)
{
  Q_UNUSED(db)
}

auto UpgraderPrivate::upgradeFunctions(eProducer storageProducer)
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

bool Upgrader::upgradeStorage(QSqlDatabase &db, eProducer storageProducer, unsigned int sourceVersion, unsigned int targetVersion)
{
  Q_D(Upgrader);

  const auto functions = d->upgradeFunctions(storageProducer);
  const auto versions = supportedVersions(storageProducer);

  auto targetVersionIdx = versions.indexOf(targetVersion);
  if (targetVersionIdx == -1)
    targetVersionIdx = versions.count();

  auto sourceVersionIdx = versions.indexOf(sourceVersion);
  if (sourceVersionIdx == -1)
    throw MYMONEYEXCEPTION_CSTRING("No upgrade from chosen source version.");

  for (auto idx = sourceVersionIdx; idx < targetVersionIdx; ++idx)
    functions.value(versions[idx])(db);

  return true;
}

QVector<unsigned int> Upgrader::supportedVersions(Storage::eProducer storageProducer)
{
  return UpgraderPrivate::upgradeFunctions(storageProducer).keys().toVector();
}

} // namespace Sql
} // namespace Storage
