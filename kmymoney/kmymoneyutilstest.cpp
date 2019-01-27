/*
 * This file is part of KMyMoney, A Personal Finance Manager for KDE
 * Copyright (C) 2012 Thomas Baumgart <ipwizard@users.sourceforge.net>
 * Copyright (C) 2015 Christian Dávid <christian-david@web.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "kmymoneyutilstest.h"

#include <QtTest/QtTest>

QTEST_MAIN(KMyMoneyUtilsTest)

void KMyMoneyUtilsTest::init()
{
}

void KMyMoneyUtilsTest::cleanup()
{
}

void KMyMoneyUtilsTest::initTestCase()
{
}

void KMyMoneyUtilsTest::testNextCheckNumber()
{
  MyMoneyAccount acc;

  // make sure first check number is 1
  QVERIFY(KMyMoneyUtils::nextCheckNumber(QString()) == QLatin1String("1"));

  // a simple increment of a plain value
  QVERIFY(KMyMoneyUtils::nextCheckNumber(QLatin1String("123")) == QLatin1String("124"));

  // a number preceded by text
  QVERIFY(KMyMoneyUtils::nextCheckNumber(QLatin1String("No 123")) == QLatin1String("No 124"));

  // a number followed by text
  QVERIFY(KMyMoneyUtils::nextCheckNumber(QLatin1String("123 ABC")) == QLatin1String("124 ABC"));

  // a number enclosed by text
  QVERIFY(KMyMoneyUtils::nextCheckNumber(QLatin1String("No 123 ABC")) == QLatin1String("No 124 ABC"));

  // a number containig a dash (e.g. invoice number)
  QVERIFY(KMyMoneyUtils::nextCheckNumber(QLatin1String("No 123-001 ABC")) == QLatin1String("No 123-002 ABC"));

  // a number containing a dot (e.g. invoice number)
  QVERIFY(KMyMoneyUtils::nextCheckNumber(QLatin1String("2012.001")) == QLatin1String("2012.002"));
}

void KMyMoneyUtilsTest::testNextStatementNumber()
{
  MyMoneyAccount acc;

  // make sure first check number is 1
  QCOMPARE(KMyMoneyUtils::nextStatementNumber(QLatin1String("")), QLatin1String("1.1"));

  QCOMPARE(KMyMoneyUtils::nextStatementNumber(QLatin1String("1.1")), QLatin1String("2.1"));

  QCOMPARE(KMyMoneyUtils::nextStatementNumber(QLatin1String("2.1")), QLatin1String("3.1"));

  QCOMPARE(KMyMoneyUtils::nextStatementNumber(QLatin1String("4/2018.1")), QLatin1String("5/2018.1"));

  QCOMPARE(KMyMoneyUtils::nextStatementNumber(QLatin1String("5/2018.1")), QLatin1String("6/2018.1"));
}

void KMyMoneyUtilsTest::testNextStatementPageNumber()
{
  MyMoneyAccount acc;
  QCOMPARE(KMyMoneyUtils::nextStatementPageNumber(QLatin1String("1.1")), QLatin1String("1.2"));

  QCOMPARE(KMyMoneyUtils::nextStatementPageNumber(QLatin1String("1.2")), QLatin1String("1.3"));

  QCOMPARE(KMyMoneyUtils::nextStatementPageNumber(QLatin1String("1.3")), QLatin1String("1.4"));

  QCOMPARE(KMyMoneyUtils::nextStatementPageNumber(QLatin1String("4/2018.1")), QLatin1String("4/2018.2"));
}
