/*
 * This file is part of KMyMoney, A Personal Finance Manager for KDE
 * Copyright (C) 2014 Christian Dávid <christian-david@web.de>
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

#include "kbankcodeedit.h"

#include <QDebug>
#include <QtGui/QApplication>
#include <QtGui/QCompleter>
#include <QtGui/QListView>
#include <QtGui/QStyledItemDelegate>
#include <QtGui/QPainter>
#include <QtGui/QStyle>

#include "../bankcodemodel.h"
#include "bankcodevalidator.h"

class bankCodeItemDelegate : public QStyledItemDelegate
{
public:
  explicit bankCodeItemDelegate(QObject* parent = 0) : QStyledItemDelegate(parent) {}
  void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
  virtual QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

private:
  inline QFont getSmallFont(const QStyleOptionViewItem& option) const;
};

KBankCodeEdit::KBankCodeEdit(QWidget* parent)
    : KLineEdit(parent)
{
  QCompleter* completer = new QCompleter(this);

  bankCodeModel* model = new bankCodeModel(this);
  completer->setModel(model);
  m_popupDelegate = new bankCodeItemDelegate(this);
  completer->popup()->setItemDelegate(m_popupDelegate);

  setCompleter(completer);

  bankCodeValidator *const validator = new bankCodeValidator(this);
  setValidator(validator);
}

KBankCodeEdit::~KBankCodeEdit()
{
  delete m_popupDelegate;
}

QFont bankCodeItemDelegate::getSmallFont(const QStyleOptionViewItem& option) const
{
  QFont smallFont = option.font;
  smallFont.setPointSize(0.9*smallFont.pointSize());
  return smallFont;
}

QSize bankCodeItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  QStyleOptionViewItemV4 opt = option;
  initStyleOption(&opt, index);

  QFontMetrics metrics(option.font);
  QFontMetrics smallMetrics(getSmallFont(option));
  const QStyle *style = opt.widget ? opt.widget->style() : QApplication::style();
  const int margin = style->pixelMetric(QStyle::PM_FocusFrameHMargin) + 1;

  // A bic has maximal 11 characters. So we guess, we want to display 11 characters. The name of the institution has to adapt to what is given
  return QSize(metrics.width(QLatin1Char('X')) + 2*margin, metrics.lineSpacing() + smallMetrics.lineSpacing() + smallMetrics.leading() + 2*margin);
}

/**
 * @todo enable eliding (use QFontMetrics::elidedText() )
 */
void bankCodeItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  QStyleOptionViewItemV4 opt = option;
  initStyleOption(&opt, index);

  // Background
  QStyle *style = opt.widget ? opt.widget->style() : QApplication::style();
  style->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter, opt.widget);

  const int margin = style->pixelMetric(QStyle::PM_FocusFrameHMargin) + 1;
  const QRect textArea = QRect(opt.rect.x() + margin, opt.rect.y() + margin, opt.rect.width() - 2 * margin, opt.rect.height() - 2 * margin);

  // Paint name
  painter->save();
  QFont smallFont = getSmallFont(opt);
  QFontMetrics metrics(opt.font);
  QFontMetrics smallMetrics(smallFont);
  QRect nameRect = style->alignedRect(opt.direction, Qt::AlignBottom, QSize(textArea.width(), smallMetrics.lineSpacing()), textArea);
  painter->setFont(smallFont);
  style->drawItemText(painter, nameRect, Qt::AlignBottom, QApplication::palette(), true, index.model()->data(index, bankCodeModel::InstitutionNameRole).toString(), option.state & QStyle::State_Selected ? QPalette::HighlightedText : QPalette::Mid);
  painter->restore();

  // Paint BIC
  painter->save();
  QFont normal = painter->font();
  normal.setBold(true);
  painter->setFont(normal);
  QRect bankCodeRect = style->alignedRect(opt.direction, Qt::AlignTop, QSize(textArea.width(), metrics.lineSpacing()), textArea);
  const QString bankCode = index.model()->data(index, Qt::DisplayRole).toString();
  style->drawItemText(painter, bankCodeRect, Qt::AlignTop, QApplication::palette(), true, bankCode, option.state & QStyle::State_Selected ? QPalette::HighlightedText : QPalette::Text);

  painter->restore();
}
