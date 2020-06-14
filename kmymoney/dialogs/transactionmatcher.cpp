/*
 * Copyright 2008-2015  Thomas Baumgart <tbaumgart@kde.org>
 * Copyright 2015       Christian Dávid <christian-david@web.de>
 * Copyright 2017-2018  Łukasz Wojniłowicz <lukasz.wojnilowicz@gmail.com>
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

#include "transactionmatcher.h"

#include <QDate>

#include <KLocalizedString>

#include "mymoneyaccount.h"
#include "mymoneymoney.h"
#include "mymoneysecurity.h"
#include "mymoneysplit.h"
#include "mymoneytransaction.h"
#include "mymoneyutils.h"
#include "mymoneyfile.h"
#include "mymoneyexception.h"
#include "mymoneyenums.h"

class TransactionMatcherPrivate
{
  Q_DISABLE_COPY(TransactionMatcherPrivate)

public:
  TransactionMatcherPrivate()
  {
  }

  MyMoneyAccount m_account;
};

TransactionMatcher::TransactionMatcher(const MyMoneyAccount& acc) :
  d_ptr(new TransactionMatcherPrivate)
{
  Q_D(TransactionMatcher);
  d->m_account = acc;
}

TransactionMatcher::~TransactionMatcher()
{
  Q_D(TransactionMatcher);
  delete d;
}

void TransactionMatcher::match(MyMoneyTransaction tm, MyMoneySplit sm, MyMoneyTransaction ti, MyMoneySplit si, bool allowImportedTransactions)
{
  Q_D(TransactionMatcher);
  const auto &file = MyMoneyFile::instance();
  auto sec = file->security(d->m_account.currencyId());

  // Now match the transactions.
  //
  // 'Matching' the transactions entails CREATING new transaction which
  // has parameters of matched transactions. The matched transactions
  // change their origin attribute and shouldn't be visible
  // by default anywhere. Only the new transaction will be visible.
  //
  // There are a variety of ways that a transaction can conflict.
  // Post date, splits, amount are the ones that seem to matter.
  // TODO: Handle these conflicts intelligently, at least warning
  // the user, or better yet letting the user choose which to use.
  //
  // For now, we will just use the transaction details from the start
  // transaction.  The only thing we'll take from the end transaction
  // are the bank ID's.
  //
  // What we have to do here is iterate over the splits in the end
  // transaction, and find the corresponding split in the start
  // transaction.  If there is a bankID in the end split but not the
  // start split, add it to the start split.  If there is a bankID
  // in BOTH, then this transaction cannot be merged (both transactions
  // were imported!!)  If the corresponding start split cannot  be
  // found and the end split has a bankID, we should probably just fail.
  // Although we could ADD it to the transaction.

  // ipwizard: Don't know if iterating over the transactions is a good idea.
  // In case of a split transaction recorded with KMyMoney and the transaction
  // data being imported consisting only of a single category assignment, this
  // does not make much sense. The same applies for investment transactions
  // stored in KMyMoney against imported transactions. I think a better solution
  // is to just base the match on the splits referencing the same (currently
  // selected) account.

  // verify, that tm is a manual (non-matched) transaction
  // allow matching two manual transactions

  if ((!allowImportedTransactions && tm.isImported()) || sm.isMatched())
    throw MYMONEYEXCEPTION_CSTRING("First transaction does not match requirement for matching");

  // verify that the amounts are the same, otherwise we should not be matching!
  if (sm.shares() != si.shares()) {
    throw MYMONEYEXCEPTION(QString::fromLatin1("Splits for %1 have conflicting values (%2,%3)").arg(d->m_account.name(), MyMoneyUtils::formatMoney(sm.shares(), d->m_account, sec), MyMoneyUtils::formatMoney(si.shares(), d->m_account, sec)));
  }

  if (ti.isMatched())
    throw MYMONEYEXCEPTION_CSTRING("Second transaction is a product of match.");

  // if next match is comming, then we won't hide this transaction but update it
  const auto isTransactionAlreadyMatched = tm.isMatched();
  if (!isTransactionAlreadyMatched) {
    // hide start transaction (input for matching)
    tm.setOrigin(static_cast<eMyMoney::Transaction::Origin>(tm.origin() | eMyMoney::Transaction::Origin::MatchingInput));
    file->modifyTransaction(tm);
    // from now on tm is matched transaction (output of matching)
    tm.setOrigin(eMyMoney::Transaction::Origin::MatchingOutput);
  }

  // next match has matched matched transaction and it's id is meaningless in that case
  // so search for an ID of already matched transaction
  QString startTransactionID;
  if (isTransactionAlreadyMatched) {
    for (const auto &split : tm.splits()) {
      if (split.isMatched()) {
        startTransactionID = split.MyMoneyKeyValueContainer::value("kmm-match-transaction").split(';').first();
        break;
      }
    }
  } else {
    startTransactionID = tm.id();
  }

  // add details about ma
  sm.setValue("kmm-match-transaction", QString::fromLatin1("%1;%2").arg(startTransactionID, ti.id()));
  sm.setValue("kmm-match-split", QString::fromLatin1("%1;%2").arg(sm.id(), si.id()));

  // hide end transaction (input for matching)
  ti.setOrigin(static_cast<eMyMoney::Transaction::Origin>(ti.origin() | eMyMoney::Transaction::Origin::MatchingInput));
  file->modifyTransaction(ti);

  // ipwizard: I took over the code to keep the bank id found in the endMatchTransaction
  // This might not work for QIF imports as they don't setup this information. It sure
  // makes sense for OFX and HBCI.
  const QString& bankID = si.bankID();
  if (!bankID.isEmpty()) {
    try {
      if (sm.bankID().isEmpty()) {
        sm.setBankID(bankID);
        tm.modifySplit(sm);
      }
    } catch (const MyMoneyException &e) {
      throw MYMONEYEXCEPTION(QString::fromLatin1("Unable to match all splits (%1)").arg(e.what()));
    }
  }
  //
  //  we now allow matching of two non-imported transactions
  //

  // mark the split as cleared if it does not have a reconciliation information yet
  if (sm.reconcileFlag() == eMyMoney::Split::State::NotReconciled) {
    sm.setReconcileFlag(eMyMoney::Split::State::Cleared);
  }

  // if we don't have a payee assigned to the manually entered transaction
  // we use the one we found in the imported transaction
  if (sm.payeeId().isEmpty() && !si.payeeId().isEmpty())
    sm.setPayeeId(si.payeeId());

  // We use the imported postdate and keep the previous one for unmatch
  if (tm.postDate() != ti.postDate())
    tm.setPostDate(ti.postDate());

  // combine the two memos into one
  QString memo = sm.memo();
  if (!si.memo().isEmpty() && si.memo() != memo) {
    if (!memo.isEmpty())
      memo += '\n';
    memo += si.memo();
  }
  sm.setMemo(memo);

  sm.addMatch();
  tm.modifySplit(sm);

  ti.modifySplit(si);

  // if matched transaction already exists then update it
  // if not then create it
  if (isTransactionAlreadyMatched) {
    file->modifyTransaction(tm);
  } else {
    tm.clearId();
    file->addTransaction(tm);
  }
}

void TransactionMatcher::unmatch(const MyMoneyTransaction& _t, const MyMoneySplit& _s)
{
  if (!_s.isMatched())
    return;

  const auto &file = MyMoneyFile::instance();
  MyMoneyTransaction tm(_t);
  MyMoneySplit sm(_s);
  for (const auto &transactionID : sm.matchedTransactionIDs()) {
    auto inputTransaction = file->transaction(transactionID);
    // unhide start and end transaction
    inputTransaction.setOrigin(static_cast<eMyMoney::Transaction::Origin>(inputTransaction.origin() ^ eMyMoney::Transaction::Origin::MatchingInput));
    file->modifyTransaction(inputTransaction);
  }

  // effectively remove matching information from a split
  sm.MyMoneyKeyValueContainer::deletePair("kmm-match-transaction");
  sm.MyMoneyKeyValueContainer::deletePair("kmm-match-split");
  sm.removeMatch();

  tm.modifySplit(sm);
  file->modifyTransaction(tm);

  // if there is no another matched split in a transaction
  // then this transaction can be removed
  auto isAnyMatchedSplitLeft = false;
  for (const auto &split : tm.splits()) {
    if (split.isMatched()) {
      isAnyMatchedSplitLeft = true;
      break;
    }
  }

  if (!isAnyMatchedSplitLeft)
    file->removeTransaction(tm);
}

void TransactionMatcher::accept(const MyMoneyTransaction& _t, const MyMoneySplit& _s)
{
  if (!_s.isMatched())
    return;
  MyMoneyTransaction tm(_t);
  MyMoneySplit sm(_s);

  const auto &file = MyMoneyFile::instance();
  for (const auto &transactionID : sm.matchedTransactionIDs()) {
    auto inputTransaction = file->transaction(transactionID);
    file->removeTransaction(inputTransaction);
  }

  sm.MyMoneyKeyValueContainer::deletePair("kmm-match-transaction");
  sm.MyMoneyKeyValueContainer::deletePair("kmm-match-split");
  sm.removeMatch();
  tm.modifySplit(sm);

  file->modifyTransaction(tm);
}
