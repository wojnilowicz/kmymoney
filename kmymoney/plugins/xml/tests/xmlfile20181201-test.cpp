/*
 * Copyright 2002-2017  Thomas Baumgart <tbaumgart@kde.org>
 * Copyright 2018-2019  Łukasz Wojniłowicz <lukasz.wojnilowicz@gmail.com>
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

#include "xmlfile20181201-test.h"

#include <QtTest>
#include "../mymoneystoragexml.cpp"
#include "mymoneyfile.h"
#include "xmlfiletesthelper.h"

QTEST_GUILESS_MAIN(xmlFile20181201Test)

void xmlFile20181201Test::readBasicFile()
{
  std::function<void(const MyMoneyStorageMgr * const)> testStorage = [] (const MyMoneyStorageMgr * const storage) {
    QCOMPARE("John Doe", storage->user().name());
  };

  XMLFileTestHelper::testFile(QStringLiteral("./xmlFiles/2018-12-01/basicFile.kmy"), testStorage);
}

void xmlFile20181201Test::readBasicPayee()
{
  std::function<void(const MyMoneyStorageMgr * const)> testStorage = [] (const MyMoneyStorageMgr * const storage) {
    const auto payeeByName = storage->payeeByName(QStringLiteral("Jack Doe"));
    const auto payeeByID = storage->payee(QStringLiteral("P000001"));
    QVERIFY(payeeByID == payeeByName);
    QCOMPARE(payeeByID.name(), "Jack Doe");
    QCOMPARE(payeeByID.notes(), "simple note;-_/\\");
    QCOMPARE(payeeByID.email(), "jack.doe@gnu.org");
    QCOMPARE(payeeByID.address(), "51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA");
    QCOMPARE(payeeByID.telephone(), "123 456 789");
    QCOMPARE(payeeByID.postcode(), "02110-1301");
  };

  XMLFileTestHelper::testFile(QStringLiteral("./xmlFiles/2018-12-01/basicPayee.kmy"), testStorage);
}

void xmlFile20181201Test::readBasicInstitution()
{
  std::function<void(const MyMoneyStorageMgr * const)> testStorage = [] (const MyMoneyStorageMgr * const storage) {
    const auto institutionByID = storage->institution(QStringLiteral("I000001"));
    const auto institution = storage->institutionList().first();
    QVERIFY(institution == institutionByID);
    QCOMPARE(institution.name(), "name");
    QCOMPARE(institution.city(), "town");
    QCOMPARE(institution.street(), "street");
//    QCOMPARE(institution.manager(), "manager");
    QCOMPARE(institution.postcode(), "postcode");
    QCOMPARE(institution.telephone(), "telephone");
    QCOMPARE(institution.sortcode(), "routingnumber");
  };

  XMLFileTestHelper::testFile(QStringLiteral("./xmlFiles/2018-12-01/basicInstitution.kmy"), testStorage);
}

void xmlFile20181201Test::readBasicAccount()
{
  std::function<void(const MyMoneyStorageMgr * const)> testStorage = [] (const MyMoneyStorageMgr * const storage) {
    const auto accountID = QStringLiteral("A000001");
    const auto accountByName = storage->accountByName(QStringLiteral("name"));
    const auto accountByID = storage->account(accountID);
    QList<MyMoneyAccount> accountList;
    storage->accountList(accountList);
    const auto account = accountList.first();
    QVERIFY(accountByID == accountByName);
    QVERIFY(account == accountByName);
    QCOMPARE(account.name(), "name");
    QCOMPARE(account.parentAccountId(), "AStd::Asset");
    QCOMPARE(account.openingDate(), QDate(2018, 1, 1));
    QCOMPARE(account.currencyId(), "USD");
    QCOMPARE(account.accountType(), eMyMoney::Account::Type::Checkings);
    const auto assetNode = storage->account(MyMoneyAccount::stdAccName(eMyMoney::Account::Standard::Asset));
    QVERIFY(assetNode.accountList().contains(accountID));
  };

  XMLFileTestHelper::testFile(QStringLiteral("./xmlFiles/2018-12-01/basicAccount.kmy"), testStorage);
}

void xmlFile20181201Test::readBasicTransaction()
{
  std::function<void(const MyMoneyStorageMgr * const)> testStorage = [] (const MyMoneyStorageMgr * const storage) {
    const auto transactionID = QStringLiteral("T000000000000000001");
    const auto transaction = storage->transaction(transactionID);
    QCOMPARE(transaction.entryDate(), QDate(2018, 1, 3));
    QCOMPARE(transaction.postDate(), QDate(2018, 1, 2));
    QCOMPARE(transaction.commodity(), "USD");
    QCOMPARE(transaction.splits().count(), 1);
    const auto split = transaction.splits().first();
    QCOMPARE(split.memo(), "Wohnung:Miete");
    QCOMPARE(split.value(), MyMoneyMoney("96379/100"));
    QCOMPARE(split.shares(), MyMoneyMoney("96379/100"));
    QCOMPARE(split.payeeId(), "P000001");
    QCOMPARE(split.id(), "S0001");
    QCOMPARE(split.reconcileFlag(), eMyMoney::Split::State::Reconciled);
    QCOMPARE(split.reconcileDate(), QDate(2018, 1, 4));
    QCOMPARE(split.number(), "1234");
    QCOMPARE(split.accountId(), "A000001");
  };

  XMLFileTestHelper::testFile(QStringLiteral("./xmlFiles/2018-12-01/basicTransaction.kmy"), testStorage);
}

void xmlFile20181201Test::readBasicTag()
{
  std::function<void(const MyMoneyStorageMgr * const)> testStorage = [] (const MyMoneyStorageMgr * const storage) {
    const auto tagID = QStringLiteral("G000001");
    const auto tagList = storage->tagList();
    QCOMPARE(tagList.count(), 1);
    const auto tag = tagList.first();
    QCOMPARE(tag.tagColor(), QColor(Qt::black));
    QCOMPARE(tag.name(), "tagName");
    QCOMPARE(tag.id(), tagID);

    const auto transactionID = QStringLiteral("T000000000000000001");
    const auto transaction = storage->transaction(transactionID);
    QCOMPARE(transaction.splits().first().tagIdList().first(), tagID);
  };

  XMLFileTestHelper::testFile(QStringLiteral("./xmlFiles/2018-12-01/basicTag.kmy"), testStorage);
}
void xmlFile20181201Test::readBasicScheduledTransaction()
{
  std::function<void(const MyMoneyStorageMgr * const)> testStorage = [] (const MyMoneyStorageMgr * const storage) {
    const auto scheduledTXID = QStringLiteral("SCH000001");
    const auto scheduledTX = storage->schedule(scheduledTXID);
    QCOMPARE(scheduledTX.nextDueDate(), QDate::currentDate().addDays(7));
    QCOMPARE(scheduledTX.startDate(), QDate::currentDate());
    QCOMPARE(scheduledTX.endDate(), QDate::currentDate().addDays(3*7));
    QCOMPARE(scheduledTX.autoEnter(), true);
    QCOMPARE(scheduledTX.isFixed(), true);
    QCOMPARE(scheduledTX.weekendOption(), Schedule::WeekendOption::MoveNothing);
    QCOMPARE(scheduledTX.lastPayment(), QDate::currentDate());
    QCOMPARE(scheduledTX.paymentType(), Schedule::PaymentType::DirectDebit);
    QCOMPARE(scheduledTX.type(), Schedule::Type::Bill);
    QCOMPARE(scheduledTX.name(), "A Name");
    QCOMPARE(scheduledTX.occurrence(), Schedule::Occurrence::Weekly);
    QCOMPARE(scheduledTX.occurrenceMultiplier(), 1);
  };

  const auto originalFilePath = QStringLiteral("./xmlFiles/2018-12-01/basicSchedule.kmy");

  const auto startDate = QDate::currentDate();
  const auto endDate = startDate.addDays(3*7);
  const auto lastPaymentDate = QDate::currentDate();
  const auto postDate = lastPaymentDate.addDays(7);

  const QVector<QVector<QString>> replacementDatas
  {
    {"startDate",   "2018-12-01", startDate.toString(Qt::ISODate)},
    {"endDate",     "2018-12-22", endDate.toString(Qt::ISODate)},
    {"lastPayment", "2018-12-01", lastPaymentDate.toString(Qt::ISODate)},
    {"postdate",    "2018-12-08", postDate.toString(Qt::ISODate)},
  };

  // replace certain dates in the source file for proper testing
  const auto configuredFilePath = XMLFileTestHelper::configureFile(originalFilePath, replacementDatas);
  XMLFileTestHelper::testFile(configuredFilePath, testStorage);
}

void xmlFile20181201Test::overdueScheduledTransaction()
{
  // the following checks only work correctly, if currentDate() is
  // between the 1st and 27th. If it is between 28th and 31st
  // we don't perform them. Note: this should be fixed.
  if (QDate::currentDate().day() > 27 || QDate::currentDate().day() == 1) {
    qDebug() << "testOverdue() skipped because current day is between 28th and 2nd";
    return;
  }

  std::function<void(const MyMoneyStorageMgr * const)> overdueTestStorage = [] (const MyMoneyStorageMgr * const storage) {
    const auto scheduledTXID = QStringLiteral("SCH000001");
    const auto scheduledTX = storage->schedule(scheduledTXID);
    QVERIFY(scheduledTX.isOverdue());
  };

  std::function<void(const MyMoneyStorageMgr * const)> ontimeTestStorage = [] (const MyMoneyStorageMgr * const storage) {
    const auto scheduledTXID = QStringLiteral("SCH000001");
    const auto scheduledTX = storage->schedule(scheduledTXID);
    QVERIFY(!scheduledTX.isOverdue());
  };

  const auto originalFilePath = QStringLiteral("./xmlFiles/2018-12-01/overdueSchedule.kmy");

  // prepare replacement dates for overdued kmy file
  auto startDate = QDate::currentDate().addDays(-1).addMonths(-23);
  auto lastPaymentDate = QDate::currentDate().addDays(-1).addMonths(-1);
  auto endDate = startDate.addMonths(32);
  auto postDate = lastPaymentDate.addMonths(1);

  const QVector<QVector<QString>> overdueReplacementDatas
  {
    {"startDate",   "2018-12-01", startDate.toString(Qt::ISODate)},
    {"endDate",     "2021-07-01", endDate.toString(Qt::ISODate)},
    {"lastPayment", "2018-12-01", lastPaymentDate.toString(Qt::ISODate)},
    {"postdate",    "2018-12-08", postDate.toString(Qt::ISODate)},
  };

  // replace certain dates in the source file for proper testing
  auto configuredFilePath = XMLFileTestHelper::configureFile(originalFilePath, overdueReplacementDatas);
  XMLFileTestHelper::testFile(configuredFilePath, overdueTestStorage);

  // prepare replacement dates for on-time kmy file
  startDate = startDate.addDays(1);
  lastPaymentDate = lastPaymentDate.addDays(1);
  endDate = endDate.addDays(1);
  postDate = postDate.addDays(1);

  const QVector<QVector<QString>> ontimeReplacementDatas
  {
    {"startDate",   "2018-12-01", startDate.toString(Qt::ISODate)},
    {"endDate",     "2021-07-01", endDate.toString(Qt::ISODate)},
    {"lastPayment", "2018-12-01", lastPaymentDate.toString(Qt::ISODate)},
    {"postdate",    "2018-12-08", postDate.toString(Qt::ISODate)},
  };

  configuredFilePath = XMLFileTestHelper::configureFile(originalFilePath, ontimeReplacementDatas);
  XMLFileTestHelper::testFile(configuredFilePath, ontimeTestStorage);
}

void xmlFile20181201Test::nextPayment()
{
  std::function<void(const MyMoneyStorageMgr * const)> testStorage = [] (const MyMoneyStorageMgr * const storage) {
    const auto scheduledTXID = QStringLiteral("SCH000001");
    const auto scheduledTX = storage->schedule(scheduledTXID);
    QCOMPARE(scheduledTX.nextPayment(QDate(2007, 2, 14)), QDate(2007, 2, 17));
    QCOMPARE(scheduledTX.nextPayment(QDate(2007, 2, 17)), QDate(2008, 2, 17));
    QCOMPARE(scheduledTX.nextPayment(QDate(2007, 2, 18)), QDate(2008, 2, 17));
  };

  XMLFileTestHelper::testFile(QStringLiteral("./xmlFiles/2018-12-01/nextPayment.kmy"), testStorage);
}

void xmlFile20181201Test::nextPaymentOnLastDayOfMonth()
{
  std::function<void(const MyMoneyStorageMgr * const)> testStorage = [] (const MyMoneyStorageMgr * const storage) {
    const auto scheduledTXID = QStringLiteral("SCH000001");
    auto scheduledTX = storage->schedule(scheduledTXID);

    // check for the first payment to happen
    const auto nextPayment = scheduledTX.nextPayment(QDate(2014, 10, 1));
    QCOMPARE(nextPayment, QDate(2014, 10, 31));
    scheduledTX.setLastPayment(nextPayment);

    QCOMPARE(scheduledTX.nextPayment(QDate(2014, 11, 1)), QDate(2014, 11, 30));
    QCOMPARE(scheduledTX.nextPayment(QDate(2014, 12, 1)), QDate(2014, 12, 31));
    QCOMPARE(scheduledTX.nextPayment(QDate(2015, 1, 1)), QDate(2015, 1, 31));
    QCOMPARE(scheduledTX.nextPayment(QDate(2015, 2, 1)), QDate(2015, 2, 28));
    QCOMPARE(scheduledTX.nextPayment(QDate(2015, 3, 1)), QDate(2015, 3, 31));

    // now check that we also cover leap years
    QCOMPARE(scheduledTX.nextPayment(QDate(2016, 2, 1)), QDate(2016, 2, 29));
    QCOMPARE(scheduledTX.nextPayment(QDate(2016, 3, 1)), QDate(2016, 3, 31));
  };

  XMLFileTestHelper::testFile(QStringLiteral("./xmlFiles/2018-12-01/nextPaymentOnLastDayOfMonth.kmy"), testStorage);
}

void xmlFile20181201Test::paymentDates()
{
  std::function<void(const MyMoneyStorageMgr * const)> testStorage = [] (const MyMoneyStorageMgr * const storage) {
    const auto scheduledTXID = QStringLiteral("SCH000001");
    auto scheduledTX = storage->schedule(scheduledTXID);

    QDate startDate(2006, 1, 28);
    QDate endDate(2006, 5, 30);

    const auto nextPayment = scheduledTX.nextPayment(startDate);
    auto list = scheduledTX.paymentDates(nextPayment, endDate);
    QCOMPARE(list.count(), 3);
    QCOMPARE(list[0], QDate(2006, 2, 28));
    QCOMPARE(list[1], QDate(2006, 3, 31));
    // Would fall on a Sunday so gets moved back to 28th.
    QCOMPARE(list[2], QDate(2006, 4, 28));

    // Add tests for each possible occurrence.
    // Check how paymentDates is meant to work
    // Build a list of expected dates and compare
    // Schedule::Occurrence::Once
    scheduledTX.setOccurrence(Schedule::Occurrence::Once);
    startDate.setDate(2009, 1, 1);
    endDate.setDate(2009, 12, 31);
    scheduledTX.setStartDate(startDate);
    scheduledTX.setNextDueDate(startDate);
    list = scheduledTX.paymentDates(startDate, endDate);
    QCOMPARE(list.count(), 1);
    QCOMPARE(list[0], QDate(2009, 1, 1));
    // Schedule::Occurrence::Daily
    scheduledTX.setOccurrence(Schedule::Occurrence::Daily);
    startDate.setDate(2009, 1, 1);
    endDate.setDate(2009, 1, 5);
    scheduledTX.setStartDate(startDate);
    scheduledTX.setNextDueDate(startDate);
    list = scheduledTX.paymentDates(startDate, endDate);
    QCOMPARE(list.count(), 5);
    QCOMPARE(list[0], QDate(2009, 1, 1));
    QCOMPARE(list[1], QDate(2009, 1, 2));
    // Would fall on Saturday so gets moved to 2nd.
    QCOMPARE(list[2], QDate(2009, 1, 2));
    // Would fall on Sunday so gets moved to 2nd.
    QCOMPARE(list[3], QDate(2009, 1, 2));
    QCOMPARE(list[4], QDate(2009, 1, 5));
    // Schedule::Occurrence::Daily with multiplier 2
    scheduledTX.setOccurrenceMultiplier(2);
    list = scheduledTX.paymentDates(startDate.addDays(1), endDate);
    QCOMPARE(list.count(), 2);
    // Would fall on Sunday so gets moved to 2nd.
    QCOMPARE(list[0], QDate(2009, 1, 2));
    QCOMPARE(list[1], QDate(2009, 1, 5));
    scheduledTX.setOccurrenceMultiplier(1);
    // Schedule::Occurrence::Weekly
    scheduledTX.setOccurrence(Schedule::Occurrence::Weekly);
    startDate.setDate(2009, 1, 6);
    endDate.setDate(2009, 2, 4);
    scheduledTX.setStartDate(startDate);
    scheduledTX.setNextDueDate(startDate);
    list = scheduledTX.paymentDates(startDate, endDate);
    QCOMPARE(list.count(), 5);
    QCOMPARE(list[0], QDate(2009, 1, 6));
    QCOMPARE(list[1], QDate(2009, 1, 13));
    QCOMPARE(list[2], QDate(2009, 1, 20));
    QCOMPARE(list[3], QDate(2009, 1, 27));
    QCOMPARE(list[4], QDate(2009, 2, 3));
    // Schedule::Occurrence::EveryOtherWeek
    scheduledTX.setOccurrence(Schedule::Occurrence::EveryOtherWeek);
    startDate.setDate(2009, 2, 5);
    endDate.setDate(2009, 4, 3);
    scheduledTX.setStartDate(startDate);
    scheduledTX.setNextDueDate(startDate);
    list = scheduledTX.paymentDates(startDate, endDate);
    QCOMPARE(list.count(), 5);
    QCOMPARE(list[0], QDate(2009, 2, 5));
    QCOMPARE(list[1], QDate(2009, 2, 19));
    QCOMPARE(list[2], QDate(2009, 3, 5));
    QCOMPARE(list[3], QDate(2009, 3, 19));
    QCOMPARE(list[4], QDate(2009, 4, 2));
    // Schedule::Occurrence::Fortnightly
    scheduledTX.setOccurrence(Schedule::Occurrence::Fortnightly);
    startDate.setDate(2009, 4, 4);
    endDate.setDate(2009, 5, 31);
    scheduledTX.setStartDate(startDate);
    scheduledTX.setNextDueDate(startDate);
    list = scheduledTX.paymentDates(startDate, endDate);
    QCOMPARE(list.count(), 4);
    // First one would fall on a Saturday and would get moved
    // to 3rd which is before start date so is not in list.
    // Would fall on a Saturday so gets moved to 17th.
    QCOMPARE(list[0], QDate(2009, 4, 17));
    // Would fall on a Saturday so gets moved to 1st.
    QCOMPARE(list[1], QDate(2009, 5, 1));
    // Would fall on a Saturday so gets moved to 15th.
    QCOMPARE(list[2], QDate(2009, 5, 15));
    // Would fall on a Saturday so gets moved to 29th.
    QCOMPARE(list[3], QDate(2009, 5, 29));
    // Schedule::Occurrence::EveryHalfMonth
    scheduledTX.setOccurrence(Schedule::Occurrence::EveryHalfMonth);
    startDate.setDate(2009, 6, 1);
    endDate.setDate(2009, 8, 11);
    scheduledTX.setStartDate(startDate);
    scheduledTX.setNextDueDate(startDate);
    list = scheduledTX.paymentDates(startDate, endDate);
    QCOMPARE(list.count(), 5);
    QCOMPARE(list[0], QDate(2009, 6, 1));
    QCOMPARE(list[1], QDate(2009, 6, 16));
    QCOMPARE(list[2], QDate(2009, 7, 1));
    QCOMPARE(list[3], QDate(2009, 7, 16));
    // Would fall on a Saturday so gets moved to 31st.
    QCOMPARE(list[4], QDate(2009, 7, 31));
    // Schedule::Occurrence::EveryThreeWeeks
    scheduledTX.setOccurrence(Schedule::Occurrence::EveryThreeWeeks);
    startDate.setDate(2009, 8, 12);
    endDate.setDate(2009, 11, 12);
    scheduledTX.setStartDate(startDate);
    scheduledTX.setNextDueDate(startDate);
    list = scheduledTX.paymentDates(startDate, endDate);
    QCOMPARE(list.count(), 5);
    QCOMPARE(list[0], QDate(2009, 8, 12));
    QCOMPARE(list[1], QDate(2009, 9, 2));
    QCOMPARE(list[2], QDate(2009, 9, 23));
    QCOMPARE(list[3], QDate(2009, 10, 14));
    QCOMPARE(list[4], QDate(2009, 11, 4));
    // Schedule::Occurrence::EveryFourWeeks
    scheduledTX.setOccurrence(Schedule::Occurrence::EveryFourWeeks);
    startDate.setDate(2009, 11, 13);
    endDate.setDate(2010, 3, 13);
    scheduledTX.setStartDate(startDate);
    scheduledTX.setNextDueDate(startDate);
    list = scheduledTX.paymentDates(startDate, endDate);
    QCOMPARE(list.count(), 5);
    QCOMPARE(list[0], QDate(2009, 11, 13));
    QCOMPARE(list[1], QDate(2009, 12, 11));
    QCOMPARE(list[2], QDate(2010, 1, 8));
    QCOMPARE(list[3], QDate(2010, 2, 5));
    QCOMPARE(list[4], QDate(2010, 3, 5));
    // Schedule::Occurrence::EveryThirtyDays
    scheduledTX.setOccurrence(Schedule::Occurrence::EveryThirtyDays);
    startDate.setDate(2010, 3, 19);
    endDate.setDate(2010, 7, 19);
    scheduledTX.setStartDate(startDate);
    scheduledTX.setNextDueDate(startDate);
    list = scheduledTX.paymentDates(startDate, endDate);
    QCOMPARE(list.count(), 5);
    QCOMPARE(list[0], QDate(2010, 3, 19));
    // Would fall on a Sunday so gets moved to 16th.
    QCOMPARE(list[1], QDate(2010, 4, 16));
    QCOMPARE(list[2], QDate(2010, 5, 18));
    QCOMPARE(list[3], QDate(2010, 6, 17));
    // Would fall on a Saturday so gets moved to 16th.
    QCOMPARE(list[4], QDate(2010, 7, 16));
    // Schedule::Occurrence::EveryEightWeeks
    scheduledTX.setOccurrence(Schedule::Occurrence::EveryEightWeeks);
    startDate.setDate(2010, 7, 26);
    endDate.setDate(2011, 3, 26);
    scheduledTX.setStartDate(startDate);
    scheduledTX.setNextDueDate(startDate);
    list = scheduledTX.paymentDates(startDate, endDate);
    QCOMPARE(list.count(), 5);
    QCOMPARE(list[0], QDate(2010, 7, 26));
    QCOMPARE(list[1], QDate(2010, 9, 20));
    QCOMPARE(list[2], QDate(2010, 11, 15));
    QCOMPARE(list[3], QDate(2011, 1, 10));
    QCOMPARE(list[4], QDate(2011, 3, 7));
    // Schedule::Occurrence::EveryOtherMonth
    scheduledTX.setOccurrence(Schedule::Occurrence::EveryOtherMonth);
    startDate.setDate(2011, 3, 14);
    endDate.setDate(2011, 11, 20);
    scheduledTX.setStartDate(startDate);
    scheduledTX.setNextDueDate(startDate);
    list = scheduledTX.paymentDates(startDate, endDate);
    QCOMPARE(list.count(), 5);
    QCOMPARE(list[0], QDate(2011, 3, 14));
    // Would fall on a Saturday so gets moved to 13th.
    QCOMPARE(list[1], QDate(2011, 5, 13));
    QCOMPARE(list[2], QDate(2011, 7, 14));
    QCOMPARE(list[3], QDate(2011, 9, 14));
    QCOMPARE(list[4], QDate(2011, 11, 14));
    // Schedule::Occurrence::EveryThreeMonths
    scheduledTX.setOccurrence(Schedule::Occurrence::EveryThreeMonths);
    startDate.setDate(2011, 11, 15);
    endDate.setDate(2012, 11, 19);
    scheduledTX.setStartDate(startDate);
    scheduledTX.setNextDueDate(startDate);
    list = scheduledTX.paymentDates(startDate, endDate);
    QCOMPARE(list.count(), 5);
    QCOMPARE(list[0], QDate(2011, 11, 15));
    QCOMPARE(list[1], QDate(2012, 2, 15));
    QCOMPARE(list[2], QDate(2012, 5, 15));
    QCOMPARE(list[3], QDate(2012, 8, 15));
    QCOMPARE(list[4], QDate(2012, 11, 15));
    // Schedule::Occurrence::Quarterly
    scheduledTX.setOccurrence(Schedule::Occurrence::Quarterly);
    startDate.setDate(2012, 11, 20);
    endDate.setDate(2013, 11, 23);
    scheduledTX.setStartDate(startDate);
    scheduledTX.setNextDueDate(startDate);
    list = scheduledTX.paymentDates(startDate, endDate);
    QCOMPARE(list.count(), 5);
    QCOMPARE(list[0], QDate(2012, 11, 20));
    QCOMPARE(list[1], QDate(2013, 2, 20));
    QCOMPARE(list[2], QDate(2013, 5, 20));
    QCOMPARE(list[3], QDate(2013, 8, 20));
    QCOMPARE(list[4], QDate(2013, 11, 20));
    // Schedule::Occurrence::EveryFourMonths
    scheduledTX.setOccurrence(Schedule::Occurrence::EveryFourMonths);
    startDate.setDate(2013, 11, 21);
    endDate.setDate(2015, 3, 23);
    scheduledTX.setStartDate(startDate);
    scheduledTX.setNextDueDate(startDate);
    list = scheduledTX.paymentDates(startDate, endDate);
    QCOMPARE(list.count(), 5);
    QCOMPARE(list[0], QDate(2013, 11, 21));
    QCOMPARE(list[1], QDate(2014, 3, 21));
    QCOMPARE(list[2], QDate(2014, 7, 21));
    QCOMPARE(list[3], QDate(2014, 11, 21));
    // Would fall on a Saturday so gets moved to 20th.
    QCOMPARE(list[4], QDate(2015, 3, 20));
    // Schedule::Occurrence::TwiceYearly
    scheduledTX.setOccurrence(Schedule::Occurrence::TwiceYearly);
    startDate.setDate(2015, 3, 22);
    endDate.setDate(2017, 3, 29);
    scheduledTX.setStartDate(startDate);
    scheduledTX.setNextDueDate(startDate);
    list = scheduledTX.paymentDates(startDate, endDate);
    QCOMPARE(list.count(), 4);
    // First date would fall on a Sunday which would get moved
    // to 20th which is before start date so not in list.
    QCOMPARE(list[0], QDate(2015, 9, 22));
    QCOMPARE(list[1], QDate(2016, 3, 22));
    QCOMPARE(list[2], QDate(2016, 9, 22));
    QCOMPARE(list[3], QDate(2017, 3, 22));
    // Schedule::Occurrence::Yearly
    scheduledTX.setOccurrence(Schedule::Occurrence::Yearly);
    startDate.setDate(2017, 3, 23);
    endDate.setDate(2021, 3, 29);
    scheduledTX.setStartDate(startDate);
    scheduledTX.setNextDueDate(startDate);
    list = scheduledTX.paymentDates(startDate, endDate);
    QCOMPARE(list.count(), 5);
    QCOMPARE(list[0], QDate(2017, 3, 23));
    QCOMPARE(list[1], QDate(2018, 3, 23));
    // Would fall on a Saturday so gets moved to 22nd.
    QCOMPARE(list[2], QDate(2019, 3, 22));
    QCOMPARE(list[3], QDate(2020, 3, 23));
    QCOMPARE(list[4], QDate(2021, 3, 23));
    // Schedule::Occurrence::EveryOtherYear
    scheduledTX.setOccurrence(Schedule::Occurrence::EveryOtherYear);
    startDate.setDate(2021, 3, 24);
    endDate.setDate(2029, 3, 30);
    scheduledTX.setStartDate(startDate);
    scheduledTX.setNextDueDate(startDate);
    list = scheduledTX.paymentDates(startDate, endDate);
    QCOMPARE(list.count(), 5);
    QCOMPARE(list[0], QDate(2021, 3, 24));
    QCOMPARE(list[1], QDate(2023, 3, 24));
    QCOMPARE(list[2], QDate(2025, 3, 24));
    QCOMPARE(list[3], QDate(2027, 3, 24));
    // Would fall on a Saturday so gets moved to 23rd.
    QCOMPARE(list[4], QDate(2029, 3, 23));
  };

  XMLFileTestHelper::testFile(QStringLiteral("./xmlFiles/2018-12-01/paymentDates.kmy"), testStorage);
}

void xmlFile20181201Test::hasReferenceInSchedule()
{
  std::function<void(const MyMoneyStorageMgr * const)> testStorage = [] (const MyMoneyStorageMgr * const storage) {
    const auto scheduledTXID = QStringLiteral("SCH000001");
    const auto scheduledTX = storage->schedule(scheduledTXID);

    QCOMPARE(scheduledTX.hasReferenceTo(QLatin1String("P000001")), true);
    QCOMPARE(scheduledTX.hasReferenceTo(QLatin1String("A000001")), true);
    QCOMPARE(scheduledTX.hasReferenceTo(QLatin1String("A000002")), true);
    QCOMPARE(scheduledTX.hasReferenceTo(QLatin1String("USD")), true);
  };

  const auto originalFilePath = QStringLiteral("./xmlFiles/2018-12-01/hasReferenceInSchedule.kmy");

  const QVector<QVector<QString>> replacementDatas
  {
    {"startDate",   "2018-12-01", QDate::currentDate().toString(Qt::ISODate)},
    {"endDate",     "2019-01-01", QDate::currentDate().toString(Qt::ISODate)},
    {"lastPayment", "2018-12-01", QDate::currentDate().toString(Qt::ISODate)},
    {"postdate",    "2018-01-08", QDate::currentDate().toString(Qt::ISODate)},
  };

  // replace certain dates in the source file for proper testing
  const auto configuredFilePath = XMLFileTestHelper::configureFile(originalFilePath, replacementDatas);
  XMLFileTestHelper::testFile(configuredFilePath, testStorage);
}

void xmlFile20181201Test::paidEarlyOneTime()
{
  // this tries to figure out what's wrong with
  // https://bugs.kde.org/show_bug.cgi?id=231029

  std::function<void(const MyMoneyStorageMgr * const)> testStorage = [] (const MyMoneyStorageMgr * const storage) {
    const auto scheduledTXID = QStringLiteral("SCH000001");
    const auto scheduledTX = storage->schedule(scheduledTXID);

    QCOMPARE(scheduledTX.isFinished(), true);
    QCOMPARE(scheduledTX.occurrencePeriod(), Schedule::Occurrence::Monthly);
    QCOMPARE(scheduledTX.paymentDates(QDate::currentDate(), QDate::currentDate().addDays(21)).count(), 0);
  };

  const auto originalFilePath = QStringLiteral("./xmlFiles/2018-12-01/paidEarlyOneTime.kmy");

  const auto paymentInFuture = QDate::currentDate().addDays(7);

  const QVector<QVector<QString>> replacementDatas
  {
    {"startDate",   "2018-12-01", paymentInFuture.toString(Qt::ISODate)},
    {"endDate",     "2019-01-01", paymentInFuture.toString(Qt::ISODate)},
    {"lastPayment", "2018-12-01", paymentInFuture.toString(Qt::ISODate)},
    {"postdate",    "2018-12-08", paymentInFuture.toString(Qt::ISODate)},
  };

  // replace certain dates in the source file for proper testing
  const auto configuredFilePath = XMLFileTestHelper::configureFile(originalFilePath, replacementDatas);
  XMLFileTestHelper::testFile(configuredFilePath, testStorage);
}

void xmlFile20181201Test::replaceIDinSchedule()
{
  std::function<void(const MyMoneyStorageMgr * const)> testStorage = [] (const MyMoneyStorageMgr * const storage) {
    const auto scheduledTXID = QStringLiteral("SCH000001");
    auto scheduledTX = storage->schedule(scheduledTXID);

    QCOMPARE(scheduledTX.transaction().postDate().isValid(), false);
    QCOMPARE(scheduledTX.transaction().splits()[0].accountId(), QLatin1String("A000001"));
    QCOMPARE(scheduledTX.transaction().splits()[1].accountId(), QLatin1String("A000002"));
    QCOMPARE(scheduledTX.replaceId(QLatin1String("A000003"), QLatin1String("A000001")), true);
    QCOMPARE(scheduledTX.transaction().splits()[0].accountId(), QLatin1String("A000003"));
    QCOMPARE(scheduledTX.transaction().splits()[1].accountId(), QLatin1String("A000002"));
  };

  const auto originalFilePath = QStringLiteral("./xmlFiles/2018-12-01/replaceIDinSchedule.kmy");

  const auto paymentInFuture = QDate::currentDate().addDays(7);

  const QVector<QVector<QString>> replacementDatas
  {
    {"startDate",   "2018-12-01", paymentInFuture.toString(Qt::ISODate)},
    {"endDate",     "2019-01-01", paymentInFuture.toString(Qt::ISODate)},
    {"lastPayment", "2018-12-01", paymentInFuture.toString(Qt::ISODate)},
    {"postdate",    "2018-12-08", paymentInFuture.toString(Qt::ISODate)},
  };

  // replace certain dates in the source file for proper testing
  const auto configuredFilePath = XMLFileTestHelper::configureFile(originalFilePath, replacementDatas);
  XMLFileTestHelper::testFile(configuredFilePath, testStorage);
}

void xmlFile20181201Test::readMergedTransaction()
{
  std::function<void(const MyMoneyStorageMgr * const)> testStorage = [] (const MyMoneyStorageMgr * const storage) {
    QList<MyMoneyTransaction> transactionsList;
    MyMoneyTransactionFilter transactionsFilter;
    transactionsFilter.clear(); // to get rid of default origin filter
    transactionsFilter.setReportAllSplits(false);
    storage->transactionList(transactionsList, transactionsFilter);
    QCOMPARE(transactionsList.count(), 4);
    const auto matchedTransaction = transactionsList[0];
    const auto importedTransaction = transactionsList[1];
    const auto typedTransaction = transactionsList[2];
    const auto anotherTypedTransaction = transactionsList[3];

    QVERIFY(matchedTransaction.origin() & (eMyMoney::Transaction::Origin::MatchingOutput | eMyMoney::Transaction::Origin::Typed));
    QVERIFY(importedTransaction.origin() & (eMyMoney::Transaction::Origin::MatchingInput | eMyMoney::Transaction::Origin::Imported));
    QVERIFY(typedTransaction.origin() & (eMyMoney::Transaction::Origin::MatchingInput | eMyMoney::Transaction::Origin::Imported));
    QVERIFY(anotherTypedTransaction.origin() & (eMyMoney::Transaction::Origin::Typed));

    const QString matchedTransactionID = QStringLiteral("T000000000000000019");
    const QString importedTransactionID = QStringLiteral("T000000000000000020");
    const QString typedTransactionID = QStringLiteral("T000000000000000021");
    const QString anotherTypedTransactionID = QStringLiteral("T000000000000000002");

    QCOMPARE(matchedTransaction.id(), matchedTransactionID);
    QCOMPARE(importedTransaction.id(), importedTransactionID);
    QCOMPARE(typedTransaction.id(), typedTransactionID);
    QCOMPARE(anotherTypedTransaction.id(), anotherTypedTransactionID);

    QCOMPARE(matchedTransaction.entryDate(), QDate(2010, 3, 8));
    QCOMPARE(importedTransaction.entryDate(), QDate(2010, 3, 8));
    QCOMPARE(typedTransaction.entryDate(), QDate(2010, 3, 8));
    QCOMPARE(anotherTypedTransaction.entryDate(), QDate(2010, 3, 8));

    QCOMPARE(matchedTransaction.postDate(), QDate(2010, 3, 4));
    QCOMPARE(importedTransaction.postDate(), QDate(2010, 3, 4));
    QCOMPARE(typedTransaction.postDate(), QDate(2010, 3, 5));
    QCOMPARE(anotherTypedTransaction.postDate(), QDate(2011, 3, 5));

    const auto matchedSplits = matchedTransaction.splits();
    const auto importedSplits = importedTransaction.splits();
    const auto typedSplits = typedTransaction.splits();
    const auto anotherTypedSplits = anotherTypedTransaction.splits();

    QCOMPARE(matchedSplits.count(), 2);
    QCOMPARE(importedSplits.count(), 1);
    QCOMPARE(typedSplits.count(), 2);
    QCOMPARE(anotherTypedSplits.count(), 1);

    const auto matchedSplit = matchedSplits[0];
    const auto secondSplit = matchedSplits[1];

    QCOMPARE(matchedSplit.transactionId(), matchedTransactionID);
    QCOMPARE(secondSplit.transactionId(), matchedTransactionID);

    QCOMPARE(matchedSplit.payeeId(), "P000002");
    QCOMPARE(secondSplit.payeeId(), "P000002");

    QCOMPARE(matchedSplit.accountId(), "A000001");
    QCOMPARE(secondSplit.accountId(), "A000002");

    QCOMPARE(matchedSplit.memo(), "originalMemo\nUMBUCHUNG");
    QCOMPARE(secondSplit.memo(), "originalMemo");

    QCOMPARE(importedSplits[0].memo(), "UMBUCHUNG");
    QCOMPARE(typedSplits[0].memo(), "originalMemo");

    QCOMPARE(matchedSplit.pairs().count(), 2);
    QCOMPARE(secondSplit.pairs().count(), 0);

    QCOMPARE(matchedSplit.value("kmm-match-split"), "S0001;S0001");
    QVERIFY(matchedSplit.value("kmm-match-transaction").contains(importedTransactionID));
    QVERIFY(matchedSplit.value("kmm-match-transaction").contains(typedTransactionID));
  };

  XMLFileTestHelper::testFile(QStringLiteral("./xmlFiles/2018-12-01/mergedTransaction.kmy"), testStorage);
}

void xmlFile20181201Test::replaceIDinSplit()
{
  std::function<void(const MyMoneyStorageMgr * const)> testStorage = [] (const MyMoneyStorageMgr * const storage) {
    QList<MyMoneyTransaction> transactionsList;
    MyMoneyTransactionFilter transactionsFilter;
    transactionsFilter.clear(); // to get rid of default origin filter
    transactionsFilter.setReportAllSplits(false);
    storage->transactionList(transactionsList, transactionsFilter);

    auto matchedTransaction = storage->transaction(transactionsList[0].id());
    auto importedTransaction = storage->transaction(transactionsList[1].id());
    auto typedTransaction = storage->transaction(transactionsList[2].id());

    const QString originalMatchedTransactionPayeeID = QStringLiteral("P000002");
    const QString originalImportedTransactionPayeeID = QStringLiteral("P000001");
    const QString originalTypedTransactionPayeeID = QStringLiteral("P000002");
    const QString replacementPayeeID = QStringLiteral("P000003");

    QCOMPARE(matchedTransaction.splits()[0].payeeId(), originalMatchedTransactionPayeeID);
    QCOMPARE(matchedTransaction.splits()[1].payeeId(), originalMatchedTransactionPayeeID);

    QCOMPARE(importedTransaction.splits()[0].payeeId(), originalImportedTransactionPayeeID);

    QCOMPARE(typedTransaction.splits()[0].payeeId(), originalTypedTransactionPayeeID);
    QCOMPARE(typedTransaction.splits()[1].payeeId(), originalTypedTransactionPayeeID);

    const auto file = MyMoneyFile::instance();
    file->attachStorage(const_cast<MyMoneyStorageMgr *>(storage));

    QCOMPARE(matchedTransaction.replaceId("P2", "P1"), false);
    QCOMPARE(matchedTransaction.splits()[0].payeeId(), originalMatchedTransactionPayeeID);
    QCOMPARE(matchedTransaction.splits()[1].payeeId(), originalMatchedTransactionPayeeID);
    QCOMPARE(matchedTransaction.replaceId(replacementPayeeID, originalMatchedTransactionPayeeID), true);
    QCOMPARE(matchedTransaction.splits()[0].payeeId(), replacementPayeeID);
    QCOMPARE(matchedTransaction.splits()[1].payeeId(), replacementPayeeID);

    MyMoneyFileTransaction ft;
    file->modifyTransaction(matchedTransaction);
    ft.commit();

    matchedTransaction = file->transaction(transactionsList[0].id());
    importedTransaction = file->transaction(transactionsList[1].id());
    typedTransaction = file->transaction(transactionsList[2].id());

    QCOMPARE(matchedTransaction.splits()[0].payeeId(), replacementPayeeID);
    QCOMPARE(matchedTransaction.splits()[1].payeeId(), replacementPayeeID);

    QCOMPARE(importedTransaction.splits()[0].payeeId(), originalImportedTransactionPayeeID);

    QCOMPARE(typedTransaction.splits()[0].payeeId(), replacementPayeeID);
    QCOMPARE(typedTransaction.splits()[1].payeeId(), replacementPayeeID);

    // revert any change to the file, so that it can be written as it has been read
    QCOMPARE(matchedTransaction.replaceId(originalMatchedTransactionPayeeID, replacementPayeeID), true);
    ft.restart();
    file->modifyTransaction(matchedTransaction);
    ft.commit();

    MyMoneyFile::instance()->detachStorage();
  };

  XMLFileTestHelper::testFile(QStringLiteral("./xmlFiles/2018-12-01/mergedTransaction.kmy"), testStorage);
}

