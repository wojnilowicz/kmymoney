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

#ifndef XMLFILE20181201TEST_H
#define XMLFILE20181201TEST_H

#include <QObject>

class xmlFile20181201Test : public QObject
{
  Q_OBJECT

private Q_SLOTS:
  void readBasicFile();
  void readBasicPayee();
  void readBasicInstitution();
  void readBasicAccount();
  void readBasicTransaction();
  void readBasicTag();
  void readBasicScheduledTransaction();
  void overdueScheduledTransaction();
  void nextPayment();
  void nextPaymentOnLastDayOfMonth();
  void paymentDates();
  void hasReferenceInSchedule();
  void paidEarlyOneTime();
  void replaceIDinSchedule();
  void readMergedTransaction();
  void replaceIDinSplit();
};

#endif
