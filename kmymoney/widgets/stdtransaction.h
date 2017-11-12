/***************************************************************************
                          stdtransaction.h  -  description
                             -------------------
    begin                : Tue Jun 13 2006
    copyright            : (C) 2000-2006 by Thomas Baumgart
    email                : Thomas Baumgart <ipwizard@users.sourceforge.net>
                           (C) 2017 by Łukasz Wojniłowicz <lukasz.wojnilowicz@gmail.com>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef STDTRANSACTION_H
#define STDTRANSACTION_H

// ----------------------------------------------------------------------------
// QT Includes

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "transaction.h"

namespace KMyMoneyRegister
{
  class StdTransactionPrivate;
  class StdTransaction : public Transaction
  {
    Q_DISABLE_COPY(StdTransaction)

  public:
    explicit StdTransaction(Register* getParent, const MyMoneyTransaction& transaction, const MyMoneySplit& split, int uniqueId);
    ~StdTransaction() override;

    virtual const char* className();

    bool formCellText(QString& txt, Qt::Alignment& align, int row, int col, QPainter* painter = 0);
    void registerCellText(QString& txt, Qt::Alignment& align, int row, int col, QPainter* painter = 0);

    int registerColWidth(int col, const QFontMetrics& cellFontMetrics);
    void setupForm(KMyMoneyTransactionForm::TransactionForm* form);
    void loadTab(KMyMoneyTransactionForm::TransactionForm* form);

    int numColsForm() const;

    void arrangeWidgetsInForm(QMap<QString, QWidget*>& editWidgets);
    void arrangeWidgetsInRegister(QMap<QString, QWidget*>& editWidgets);
    void tabOrderInForm(QWidgetList& tabOrderWidgets) const;
    void tabOrderInRegister(QWidgetList& tabOrderWidgets) const;
    eWidgets::eRegister::Action actionType() const;

    int numRowsRegister(bool expanded) const;

    /**
    * Provided for internal reasons. No API change. See RegisterItem::numRowsRegister()
    */
    int numRowsRegister() const;

    TransactionEditor* createEditor(TransactionEditorContainer* regForm, const KMyMoneyRegister::SelectedTransactions& list, const QDate& lastPostDate);

    /**
    * Return information if @a row should be shown (@a true )
    * or hidden (@a false ) in the form. Default is true.
    */
    virtual bool showRowInForm(int row) const;

    /**
    * Control visibility of @a row in the transaction form.
    * Only row 0 has an effect, others return @a true.
    */
    virtual void setShowRowInForm(int row, bool show);

  protected:
    Q_DECLARE_PRIVATE(StdTransaction)
    void setupFormHeader(const QString& id);
  };
} // namespace

#endif