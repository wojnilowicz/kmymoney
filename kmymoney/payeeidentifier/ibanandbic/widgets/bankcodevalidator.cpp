/*
 * This file is part of KMyMoney, A Personal Finance Manager for KDE
 * Copyright (C) 2014 Christian David <christian-david@web.de>
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

#include "bankcodevalidator.h"

#include <KLocalizedString>

#include "payeeidentifier/ibanandbic/ibanbic.h"

bankCodeValidator::bankCodeValidator(QObject* parent)
    : QValidator(parent)
{
}

QValidator::State bankCodeValidator::validate(QString &string, int&) const
{
  for (int i = 0; i < qMin(string.length(), 2); ++i) {
    if (!string.at(i).isLetterOrNumber())
      return Invalid;
  }

  int start = string.at(0).isLetter() ? 2 : 0;

  for (int i = start; i < qMin(string.length(), 8+start); ++i) {
  if (!string.at(i).isNumber())
    return Invalid;
  }

  if (string.length() > 10)
    return Invalid;
  else if (string.length() == 8 || string.length() == 10) {
    return Acceptable;
  }
  return Intermediate;
}

QPair< KMyMoneyValidationFeedback::MessageType, QString > bankCodeValidator::validateWithMessage(const QString& string)
{
  // Do not show an error message if no BIC is given.
  if (string.length() != 8)
    return QPair< KMyMoneyValidationFeedback::MessageType, QString >(KMyMoneyValidationFeedback::Error, i18n("A valid bank code is 8 or 10 characters long."));

//  if (payeeIdentifiers::ibanBic::isBicAllocated(string) == payeeIdentifiers::ibanBic::bicNotAllocated)
//    return QPair< KMyMoneyValidationFeedback::MessageType, QString >(KMyMoneyValidationFeedback::Error, i18n("The given bank code is not assigned to any credit institute."));

  return QPair< KMyMoneyValidationFeedback::MessageType, QString >(KMyMoneyValidationFeedback::None, QString());

}
