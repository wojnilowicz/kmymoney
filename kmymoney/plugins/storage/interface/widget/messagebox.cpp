/*
 * Copyright 2020  Łukasz Wojniłowicz <lukasz.wojnilowicz@gmail.com>
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

#include "messagebox.h"

// ----------------------------------------------------------------------------
// QT Includes

// ----------------------------------------------------------------------------
// KDE Includes

#include <KMessageBox>

// ----------------------------------------------------------------------------
// Project Includes

#include "enums.h"
#include "ivalidator.h"

namespace Storage {
namespace Widget {

eAnswer buttonCodeToAnswer(KMessageBox::ButtonCode code)
{
  switch (code) {
    case KMessageBox::ButtonCode::Yes:
      return eAnswer::Yes;
    case KMessageBox::ButtonCode::No:
      return eAnswer::No;
    default:
      return eAnswer::Cancel;
  }
}

void MessageBox::questionYesNo(const QString &message, const QString &title, const QStringList &data)
{
  const auto messageWithData = fillPlaceholders(message, data);
  const auto ret = KMessageBox::questionYesNo(qobject_cast<QWidget *>(m_parent),
                                              messageWithData,
                                              title,
                                              KStandardGuiItem::yes(),
                                              KStandardGuiItem::no(),
                                              QString(),
                                              KMessageBox::WindowModal);
  emit answerAboutIssue(buttonCodeToAnswer(ret));
}

void MessageBox::warning(const QString &message, const QString &title, const QStringList &data)
{
  const auto messageWithData = fillPlaceholders(message, data);
  KMessageBox::sorry(qobject_cast<QWidget *>(m_parent),
                     messageWithData,
                     title,
                     KMessageBox::WindowModal);
  emit answerAboutIssue(eAnswer::Cancel);
}

void MessageBox::error(const QString &message, const QString &title, const QStringList &data)
{
  const auto messageWithData = fillPlaceholders(message, data);
  KMessageBox::error(qobject_cast<QWidget *>(m_parent),
                     messageWithData,
                     title,
                     KMessageBox::WindowModal);
  emit answerAboutIssue(eAnswer::Cancel);
}

void MessageBox::detailedError(const QString &message, const QString &title, const QStringList &data)
{
  KMessageBox::detailedError(qobject_cast<QWidget *>(m_parent),
                     message,
                     data.join('\n'),
                     title,
                     KMessageBox::WindowModal);
  emit answerAboutIssue(eAnswer::Cancel);
}

void MessageBox::informationList(const QString &message, const QString &title, const QStringList &data)
{
  KMessageBox::informationList(qobject_cast<QWidget *>(m_parent),
                               message,
                               data,
                               title,
                               QString(),
                               KMessageBox::WindowModal);
  emit answerAboutIssue(eAnswer::Ok);
}

void MessageBox::information(const QString &message, const QString &title, const QStringList &data)
{
  const auto messageWithData = fillPlaceholders(message, data);
  KMessageBox::information(qobject_cast<QWidget *>(m_parent),
                           messageWithData,
                           title,
                           QString(),
                           KMessageBox::WindowModal);
  emit answerAboutIssue(eAnswer::Ok);
}


} // namespace Widget
} // namespace Storage
