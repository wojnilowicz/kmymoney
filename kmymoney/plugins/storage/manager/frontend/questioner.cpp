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

#include "questioner.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QDebug>
#include <QMap>
#include <QVector>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "storage/interface/enums.h"
#include "storage/interface/ivalidator.h"
#include "storage/interface/imessagebox.h"

#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))

namespace Storage {
namespace Manager {

typedef eAnswer(Questioner::*MessageBoxFn)(const Issue &issue);

void Questioner::questionAboutIssue(const Issue &issue)
{
  const QMap<eIssue , IMessageBoxFn> messageBoxes {
    {eIssue::MissingPlugins,  &IMessageBox::warning},
    {eIssue::MissingPlugin,   &IMessageBox::warning},
  };

  auto message = standardMessage(issue.code);
  auto title = standardTitle(issue.code);
  auto data = issue.data;
  auto messageBox = messageBoxes.value(issue.code, &IMessageBox::error);
  CALL_MEMBER_FN(*m_messageBox, messageBox)(message, title, data);
}

} // namespace Manager
} // namespace Storage
