/***************************************************************************
                          kenterscheduledlg.cpp
                             -------------------
    begin                : Sat Apr  7 2007
    copyright            : (C) 2007 by Thomas Baumgart
    email                : Thomas Baumgart <ipwizard@users.sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "kenterscheduledlg.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QTimer>
#include <QWidget>
#include <QLabel>
#include <QResizeEvent>

// ----------------------------------------------------------------------------
// KDE Includes

#include <klocale.h>
#include <kmessagebox.h>
#include <kpushbutton.h>
#include <kiconloader.h>

// ----------------------------------------------------------------------------
// Project Includes

#include "kcurrencycalculator.h"
#include <register.h>
#include <transactionform.h>
#include <transaction.h>
#include <transactioneditor.h>
#include <kmymoneyutils.h>
#include <mymoneyfinancialcalculator.h>
#include <kmymoneylineedit.h>
#include <kmymoneycategory.h>
#include <kmymoneyaccountselector.h>
#include <kmymoneydateinput.h>
#include <ktoolinvocation.h>
#include <kmymoneyglobalsettings.h>

#include "kmymoney.h"

class KEnterScheduleDlg::Private
{
public:
  Private() : m_item(0), m_showWarningOnce(true) {}
  ~Private() {}

  MyMoneySchedule                m_schedule;
  KMyMoneyRegister::Transaction* m_item;
  QWidgetList                    m_tabOrderWidgets;
  bool                           m_showWarningOnce;
  KMyMoneyUtils::EnterScheduleResultCodeE m_extendedReturnCode;
};

KEnterScheduleDlg::KEnterScheduleDlg(QWidget *parent, const MyMoneySchedule& schedule) :
    KEnterScheduleDlgDecl(parent),
    d(new Private)
{
  d->m_schedule = schedule;
  d->m_extendedReturnCode = KMyMoneyUtils::Enter;
  buttonOk->setIcon(KIcon(KMyMoneyGlobalSettings::enterScheduleIcon()));
  buttonSkip->setIcon(KIcon("media-seek-forward"));
  buttonCancel->setGuiItem(KStandardGuiItem::cancel());
  buttonHelp->setGuiItem(KStandardGuiItem::help());
  buttonIgnore->setHidden(true);
  buttonSkip->setHidden(true);

  // make sure, we have a tabbar with the form
  KMyMoneyTransactionForm::TabBar* tabbar = m_form->tabBar(m_form->parentWidget());

  // we never need to see the register
  m_register->hide();

  // ... setup the form ...
  m_form->setupForm(d->m_schedule.account());

  // ... and the register ...
  m_register->clear();

  // ... now add the transaction to register and form ...
  MyMoneyTransaction t = transaction();
  d->m_item = KMyMoneyRegister::Register::transactionFactory(m_register, t,
              d->m_schedule.transaction().splits().isEmpty() ? MyMoneySplit() : d->m_schedule.transaction().splits().front(), 0);
  m_register->selectItem(d->m_item);
  // show the account row
  d->m_item->setShowRowInForm(0, true);

  m_form->slotSetTransaction(d->m_item);

  // no need to see the tabbar
  tabbar->hide();

  // setup name and type
  m_scheduleName->setText(d->m_schedule.name());
  m_type->setText(KMyMoneyUtils::scheduleTypeToString(d->m_schedule.type()));

  connect(buttonHelp, SIGNAL(clicked()), this, SLOT(slotShowHelp()));
  connect(buttonIgnore, SIGNAL(clicked()), this, SLOT(slotIgnore()));
  connect(buttonSkip, SIGNAL(clicked()), this, SLOT(slotSkip()));
}

KEnterScheduleDlg::~KEnterScheduleDlg()
{
  delete d;
}

KMyMoneyUtils::EnterScheduleResultCodeE KEnterScheduleDlg::resultCode() const
{
  if (result() == QDialog::Accepted)
    return d->m_extendedReturnCode;
  return KMyMoneyUtils::Cancel;
}

void KEnterScheduleDlg::showExtendedKeys(bool visible)
{
  buttonIgnore->setVisible(visible);
  buttonSkip->setVisible(visible);
}

void KEnterScheduleDlg::slotIgnore()
{
  d->m_extendedReturnCode = KMyMoneyUtils::Ignore;
  accept();
}

void KEnterScheduleDlg::slotSkip()
{
  d->m_extendedReturnCode = KMyMoneyUtils::Skip;
  accept();
}

MyMoneyTransaction KEnterScheduleDlg::transaction()
{
  MyMoneyTransaction t = d->m_schedule.transaction();

  try {
    if (d->m_schedule.type() == MyMoneySchedule::TYPE_LOANPAYMENT) {
      KMyMoneyUtils::calculateAutoLoan(d->m_schedule, t, QMap<QString, MyMoneyMoney>());
    }
  } catch (const MyMoneyException &e) {
    KMessageBox::detailedError(this, i18n("Unable to load schedule details"), e.what());
  }

  t.clearId();
  t.setEntryDate(QDate());
  return t;
}

QDate KEnterScheduleDlg::date(const QDate& _date) const
{
  QDate date(_date);
  return d->m_schedule.adjustedDate(date, d->m_schedule.weekendOption());
}

void KEnterScheduleDlg::resizeEvent(QResizeEvent* ev)
{
  m_register->resize(KMyMoneyRegister::DetailColumn);
  m_form->resize(KMyMoneyTransactionForm::ValueColumn1);
  KEnterScheduleDlgDecl::resizeEvent(ev);
}


void KEnterScheduleDlg::slotSetupSize()
{
  resize(width(), minimumSizeHint().height());
}

int KEnterScheduleDlg::exec()
{
  if (d->m_showWarningOnce) {
    d->m_showWarningOnce = false;
    KMessageBox::information(this, QString("<qt>") + i18n("<p>Please check that all the details in the following dialog are correct and press OK.</p><p>Editable data can be changed and can either be applied to just this occurrence or for all subsequent occurrences for this schedule.  (You will be asked what you intend after pressing OK in the following dialog)</p>") + QString("</qt>"), i18n("Enter scheduled transaction"), "EnterScheduleDlgInfo");
  }

  // force the initial height to be as small as possible
  QTimer::singleShot(0, this, SLOT(slotSetupSize()));

  return KEnterScheduleDlgDecl::exec();
}

TransactionEditor* KEnterScheduleDlg::startEdit()
{
  KMyMoneyRegister::SelectedTransactions list(m_register);
  TransactionEditor* editor = d->m_item->createEditor(m_form, list, QDate());
  editor->m_scheduleInfo = d->m_schedule.name();
  editor->m_paymentMethod = d->m_schedule.paymentType();

  // check that we use the same transaction commodity in all selected transactions
  // if not, we need to update this in the editor's list. The user can also bail out
  // of this operation which means that we have to stop editing here.
  if (editor) {
    if (!editor->fixTransactionCommodity(d->m_schedule.account())) {
      // if the user wants to quit, we need to destroy the editor
      // and bail out
      delete editor;
      editor = 0;
    }
  }

  if (editor) {
    connect(editor, SIGNAL(transactionDataSufficient(bool)), buttonOk, SLOT(setEnabled(bool)));
    connect(editor, SIGNAL(escapePressed()), buttonCancel, SLOT(animateClick()));
    connect(editor, SIGNAL(returnPressed()), buttonOk, SLOT(animateClick()));

    connect(MyMoneyFile::instance(), SIGNAL(dataChanged()), editor, SLOT(slotReloadEditWidgets()));
    // connect(editor, SIGNAL(finishEdit(KMyMoneyRegister::SelectedTransactions)), this, SLOT(slotLeaveEditMode(KMyMoneyRegister::SelectedTransactions)));
    connect(editor, SIGNAL(createPayee(QString,QString&)), kmymoney, SLOT(slotPayeeNew(QString,QString&)));
    connect(editor, SIGNAL(createTag(QString,QString&)), kmymoney, SLOT(slotTagNew(QString,QString&)));
    connect(editor, SIGNAL(createCategory(MyMoneyAccount&,MyMoneyAccount)), kmymoney, SLOT(slotCategoryNew(MyMoneyAccount&,MyMoneyAccount)));
    connect(editor, SIGNAL(createSecurity(MyMoneyAccount&,MyMoneyAccount)), kmymoney, SLOT(slotInvestmentNew(MyMoneyAccount&,MyMoneyAccount)));
    connect(MyMoneyFile::instance(), SIGNAL(dataChanged()), editor, SLOT(slotReloadEditWidgets()));

    // create the widgets, place them in the parent and load them with data
    // setup tab order
    d->m_tabOrderWidgets.clear();
    KMyMoneyRegister::Action action = KMyMoneyRegister::ActionWithdrawal;
    switch (d->m_schedule.type()) {
      case MyMoneySchedule::TYPE_TRANSFER:
        action = KMyMoneyRegister::ActionTransfer;
        break;
      case MyMoneySchedule::TYPE_DEPOSIT:
        action = KMyMoneyRegister::ActionDeposit;
        break;
      case MyMoneySchedule::TYPE_LOANPAYMENT:
        switch (d->m_schedule.paymentType()) {
          case MyMoneySchedule::STYPE_DIRECTDEPOSIT:
          case MyMoneySchedule::STYPE_MANUALDEPOSIT:
            action = KMyMoneyRegister::ActionDeposit;
            break;
          default:
            break;
        }
        break;
      default:
        break;
    }
    editor->setup(d->m_tabOrderWidgets, d->m_schedule.account(), action);

    MyMoneyTransaction t = d->m_schedule.transaction();
    QString num = t.splits().first().number();
    QWidget* w = editor->haveWidget("number");
    if (d->m_schedule.paymentType() == MyMoneySchedule::STYPE_WRITECHEQUE) {
      MyMoneyFile* file = MyMoneyFile::instance();
      if (file->checkNoUsed(d->m_schedule.account().id(), num)) {
        //  increment and try again
        num = KMyMoneyUtils::getAdjacentNumber(num);
      }
      num = KMyMoneyUtils::nextCheckNumber(d->m_schedule.account().value("lastNumberUsed"));
      if (w) {
        dynamic_cast<kMyMoneyLineEdit*>(w)->loadText(num);
      }
    } else {
      num = d->m_schedule.account().value("lastNumberUsed");
      if (w)
        dynamic_cast<kMyMoneyLineEdit*>(w)->loadText(num);
    }

    Q_ASSERT(!d->m_tabOrderWidgets.isEmpty());

    // editor->setup() leaves the tabbar as the last widget in the stack, but we
    // need it as first here. So we move it around.
    w = editor->haveWidget("tabbar");
    if (w) {
      int idx = d->m_tabOrderWidgets.indexOf(w);
      if (idx != -1) {
        d->m_tabOrderWidgets.removeAt(idx);
        d->m_tabOrderWidgets.push_front(w);
      }
    }

    // don't forget our three buttons
    d->m_tabOrderWidgets.append(buttonOk);
    d->m_tabOrderWidgets.append(buttonCancel);
    d->m_tabOrderWidgets.append(buttonHelp);

    for (int i = 0; i < d->m_tabOrderWidgets.size(); ++i) {
      QWidget* w = d->m_tabOrderWidgets.at(i);
      if (w) {
        w->installEventFilter(this);
        w->installEventFilter(editor);
      }
    }
    // Check if the editor has some preference on where to set the focus
    // If not, set the focus to the first widget in the tab order
    QWidget* focusWidget = editor->firstWidget();
    if (!focusWidget)
      focusWidget = d->m_tabOrderWidgets.first();
    focusWidget->setFocus();

    // Make sure, we use the adjusted date
    kMyMoneyDateInput* dateEdit = dynamic_cast<kMyMoneyDateInput*>(editor->haveWidget("postdate"));
    if (dateEdit) {
      dateEdit->setDate(d->m_schedule.adjustedNextDueDate());
    }
  }

  return editor;
}

bool KEnterScheduleDlg::focusNextPrevChild(bool next)
{
  bool rc = false;
  QWidget *w = 0;

  w = qApp->focusWidget();
  int currentWidgetIndex = d->m_tabOrderWidgets.indexOf(w);
  while (w && currentWidgetIndex == -1) {
    // qDebug("'%s' not in list, use parent", w->className());
    w = w->parentWidget();
    currentWidgetIndex = d->m_tabOrderWidgets.indexOf(w);
  }

  if (currentWidgetIndex != -1) {
    // if(w) qDebug("tab order is at '%s'", w->className());
    currentWidgetIndex += next ? 1 : -1;
    if (currentWidgetIndex < 0)
      currentWidgetIndex = d->m_tabOrderWidgets.size() - 1;
    else if (currentWidgetIndex >= d->m_tabOrderWidgets.size())
      currentWidgetIndex = 0;

    w = d->m_tabOrderWidgets[currentWidgetIndex];
    // qDebug("currentWidgetIndex = %d, w = %p", currentWidgetIndex, w);

    if (((w->focusPolicy() & Qt::TabFocus) == Qt::TabFocus) && w->isVisible() && w->isEnabled()) {
      // qDebug("Selecting '%s' as focus", w->className());
      w->setFocus();
      rc = true;
    }
  }
  return rc;
}

void KEnterScheduleDlg::slotShowHelp()
{
  KToolInvocation::invokeHelp("details.schedules.entering");
}
