/*
 * Copyright 2000-2002  Michael Edwardes <mte@users.sourceforge.net>
 * Copyright 2001-2017  Thomas Baumgart <tbaumgart@kde.org>
 * Copyright 2001       Felix Rodriguez <frodriguez@users.sourceforge.net>
 * Copyright 2003       Kevin Tambascio <ktambascio@users.sourceforge.net>
 * Copyright 2004-2005  Ace Jones <acejones@users.sourceforge.net>
 * Copyright 2006       Darren Gould <darren_gould@gmx.de>
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

#ifndef MYMONEYTRANSACTION_P_H
#define MYMONEYTRANSACTION_P_H

// ----------------------------------------------------------------------------
// QT Includes

#include <QString>
#include <QList>
#include <QDate>
#include <QHash>

// ----------------------------------------------------------------------------
// Project Includes

#include "mymoneyobject_p.h"
#include "mymoneysplit.h"
#include "mymoneyenums.h"

using namespace eMyMoney;

class MyMoneyTransactionPrivate : public MyMoneyObjectPrivate
{
public:
  MyMoneyTransactionPrivate();
  /**
    * This method returns the next id to be used for a split
    */
  QString nextSplitID()
  {
    QString id;
    id = 'S' + id.setNum(m_nextSplitID++).rightJustified(SPLIT_ID_SIZE, '0');
    return id;
  }

  static const int SPLIT_ID_SIZE = 4;
  /** constants for unique sort key */
  static const int YEAR_SIZE = 4;
  static const int MONTH_SIZE = 2;
  static const int DAY_SIZE = 2;

  /**
    * This member contains the date when the transaction was entered
    * into the engine
    */
  QDate m_entryDate;

  /**
    * This member contains the date the transaction was posted
    */
  QDate m_postDate;

  /**
    * This member keeps the memo text associated with this transaction
    */
  QString m_memo;

  /**
    * This member contains the splits for this transaction
    */
  QList<MyMoneySplit> m_splits;

  /**
    * This member keeps the unique numbers of splits within this
    * transaction. Upon creation of a MyMoneyTransaction object this
    * value will be set to 1.
    */
  uint m_nextSplitID;

  /**
    * This member keeps the base commodity (e.g. currency) for this transaction
    */
  QString  m_commodity;

  /**
    * This member keeps the bank's unique ID for the transaction, so we can
    * avoid duplicates.  This is only used for electronic statement downloads.
    *
    * Note this is now deprecated!  Bank ID's should be set on splits, not transactions.
    */
  QString m_bankID;

  eMyMoney::Transaction::Origin m_origin;
};

MyMoneyTransactionPrivate::MyMoneyTransactionPrivate() :
  MyMoneyObjectPrivate(),
  m_origin(eMyMoney::Transaction::Origin::Typed)
{
}


#endif
