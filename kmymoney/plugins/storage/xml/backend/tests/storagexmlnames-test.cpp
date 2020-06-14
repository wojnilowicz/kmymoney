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

#include "storagexmlnames-test.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QtTest>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "plugins/xmlhelper/xmlstoragehelper.h"
#include "plugins/xmlhelper/xmlstoragehelper.cpp"
#include "../names.cpp"

QTEST_GUILESS_MAIN(Storage::Xml::NamesTest)

namespace Storage {
namespace Xml {

void NamesTest::keyValuePairElementNames()
{
  for (auto i = (int)Element::KVP::Pair; i <= (int)Element::KVP::Pair; ++i) {
    auto isEmpty = elementName(static_cast<Element::KVP>(i)).isEmpty();
    if (isEmpty)
      qWarning() << "Empty element's name " << i;
    QVERIFY(!isEmpty);
  }
}

void NamesTest::keyValuePairAttributeNames()
{
  for (auto i = (int)Attribute::KVP::Key; i < (int)Attribute::KVP::LastAttribute; ++i) {
    auto isEmpty = attributeName(static_cast<Attribute::KVP>(i)).isEmpty();
    if (isEmpty)
      qWarning() << "Empty attribute's name " << i;
    QVERIFY(!isEmpty);
  }
}

void NamesTest::transactionElementNames()
{
  for (auto i = (int)Element::Transaction::Split; i <= (int)Element::Transaction::Splits; ++i) {
    auto isEmpty = elementName(static_cast<Element::Transaction>(i)).isEmpty();
    if (isEmpty)
      qWarning() << "Empty element's name " << i;
    QVERIFY(!isEmpty);
  }
}

void NamesTest::transactionAttributeNames()
{
  for (auto i = (int)Attribute::Transaction::Name; i < (int)Attribute::Transaction::LastAttribute; ++i) {
    auto isEmpty = attributeName(static_cast<Attribute::Transaction>(i)).isEmpty();
    if (isEmpty)
      qWarning() << "Empty attribute's name " << i;
    QVERIFY(!isEmpty);
  }
}

void NamesTest::splitElementNames()
{
  for (auto i = (int)Element::Split::Split; i <= (int)Element::Split::KeyValuePairs; ++i) {
    auto isEmpty = elementName(static_cast<Element::Split>(i)).isEmpty();
    if (isEmpty)
      qWarning() << "Empty element's name " << i;
    QVERIFY(!isEmpty);
  }
}

void NamesTest::splitAttributeNames()
{
  for (auto i = (int)Attribute::Split::ID; i < (int)Attribute::Split::LastAttribute; ++i) {
    auto isEmpty = attributeName(static_cast<Attribute::Split>(i)).isEmpty();
    if (isEmpty)
      qWarning() << "Empty attribute's name " << i;
    QVERIFY(!isEmpty);
  }
}

void NamesTest::accountElementNames()
{
  for (auto i = (int)Element::Account::SubAccount; i <= (int)Element::Account::OnlineBanking; ++i) {
    auto isEmpty = elementName(static_cast<Element::Account>(i)).isEmpty();
    if (isEmpty)
      qWarning() << "Empty element's name " << i;
    QVERIFY(!isEmpty);
  }
}

void NamesTest::accountAttributeNames()
{
  for (auto i = (int)Attribute::Account::ID; i < (int)Attribute::Account::LastAttribute; ++i) {
    auto isEmpty = attributeName(static_cast<Attribute::Account>(i)).isEmpty();
    if (isEmpty)
      qWarning() << "Empty attribute's name " << i;
    QVERIFY(!isEmpty);
  }
}

void NamesTest::payeeElementNames()
{
  for (auto i = (int)Element::Payee::Address; i <= (int)Element::Payee::Address; ++i) {
    auto isEmpty = elementName(static_cast<Element::Payee>(i)).isEmpty();
    if (isEmpty)
      qWarning() << "Empty element's name " << i;
    QVERIFY(!isEmpty);
  }
}

void NamesTest::payeeAttributeNames()
{
  for (auto i = (int)Attribute::Payee::ID; i < (int)Attribute::Payee::LastAttribute; ++i) {
    auto isEmpty = attributeName(static_cast<Attribute::Payee>(i)).isEmpty();
    if (isEmpty)
      qWarning() << "Empty attribute's name " << i;
    QVERIFY(!isEmpty);
  }
}

void NamesTest::tagAttributeNames()
{
  for (auto i = (int)Attribute::Tag::Name; i < (int)Attribute::Tag::LastAttribute; ++i) {
    auto isEmpty = attributeName(static_cast<Attribute::Tag>(i)).isEmpty();
    if (isEmpty)
      qWarning() << "Empty attribute's name " << i;
    QVERIFY(!isEmpty);
  }
}

void NamesTest::securityAttributeNames()
{
  for (auto i = (int)Attribute::Security::Name; i < (int)Attribute::Security::LastAttribute; ++i) {
    auto isEmpty = attributeName(static_cast<Attribute::Security>(i)).isEmpty();
    if (isEmpty)
      qWarning() << "Empty attribute's name " << i;
    QVERIFY(!isEmpty);
  }
}

void NamesTest::institutionElementNames()
{
  for (auto i = (int)Element::Institution::AccountID; i <= (int)Element::Institution::Address; ++i) {
    auto isEmpty = elementName(static_cast<Element::Institution>(i)).isEmpty();
    if (isEmpty)
      qWarning() << "Empty element's name " << i;
    QVERIFY(!isEmpty);
  }

}

void NamesTest::institutionAttributeNames()
{
  for (auto i = (int)Attribute::Institution::ID; i < (int)Attribute::Institution::LastAttribute; ++i) {
    auto isEmpty = attributeName(static_cast<Attribute::Institution>(i)).isEmpty();
    if (isEmpty)
      qWarning() << "Empty attribute's name " << i;
    QVERIFY(!isEmpty);
  }
}

void NamesTest::reportElementNames()
{
  for (auto i = (int)Element::Report::Payee; i <= (int)Element::Report::AccountGroup; ++i) {
    auto isEmpty = MyMoneyXmlContentHandler2::elementName(static_cast<Element::Report>(i)).isEmpty();
    if (isEmpty)
      qWarning() << "Empty element's name " << i;
    QVERIFY(!isEmpty);
  }
}

void NamesTest::reportAttributeNames()
{
  for (auto i = (int)Attribute::Report::ID; i < (int)Attribute::Report::LastAttribute; ++i) {
    auto isEmpty = MyMoneyXmlContentHandler2::attributeName(static_cast<Attribute::Report>(i)).isEmpty();
    if (isEmpty)
      qWarning() << "Empty attribute's name " << i;
    QVERIFY(!isEmpty);
  }
}

void NamesTest::budgetElementNames()
{
  for (auto i = (int)Element::Budget::Budget; i <= (int)Element::Budget::Period; ++i) {
    auto isEmpty = MyMoneyXmlContentHandler2::elementName(static_cast<Element::Budget>(i)).isEmpty();
    if (isEmpty)
      qWarning() << "Empty element's name " << i;
    QVERIFY(!isEmpty);
  }
}

void NamesTest::budgetAttributeNames()
{
  for (auto i = (int)Attribute::Budget::ID; i < (int)Attribute::Budget::LastAttribute; ++i) {
    auto isEmpty = MyMoneyXmlContentHandler2::attributeName(static_cast<Attribute::Budget>(i)).isEmpty();
    if (isEmpty)
      qWarning() << "Empty attribute's name " << i;
    QVERIFY(!isEmpty);
  }
}

void NamesTest::scheduleElementNames()
{
  for (auto i = (int)Element::Schedule::Payment; i <= (int)Element::Schedule::Payments; ++i) {
    auto isEmpty = elementName(static_cast<Element::Schedule>(i)).isEmpty();
    if (isEmpty)
      qWarning() << "Empty element's name " << i;
    QVERIFY(!isEmpty);
  }
}

void NamesTest::scheduleAttributeNames()
{
  for (auto i = (int)Attribute::Schedule::Name; i < (int)Attribute::Schedule::LastAttribute; ++i) {
    auto isEmpty = attributeName(static_cast<Attribute::Schedule>(i)).isEmpty();
    if (isEmpty)
      qWarning() << "Empty attribute's name " << i;
    QVERIFY(!isEmpty);
  }
}

void NamesTest::onlineJobElementNames()
{
  for (auto i = (int)Element::OnlineJob::OnlineTask; i <= (int)Element::OnlineJob::OnlineTask; ++i) {
    auto isEmpty = elementName(static_cast<Element::OnlineJob>(i)).isEmpty();
    if (isEmpty)
      qWarning() << "Empty element's name " << i;
    QVERIFY(!isEmpty);
  }
}

void NamesTest::onlineJobAttributeNames()
{
  for (auto i = (int)Attribute::OnlineJob::Send; i < (int)Attribute::OnlineJob::LastAttribute; ++i) {
    auto isEmpty = attributeName(static_cast<Attribute::OnlineJob>(i)).isEmpty();
    if (isEmpty)
      qWarning() << "Empty attribute's name " << i;
    QVERIFY(!isEmpty);
  }
}

} // namespace Xml
} // namespace Storage
