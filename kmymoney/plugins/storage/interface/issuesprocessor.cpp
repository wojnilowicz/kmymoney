/*
 * Copyright 2020       Łukasz Wojniłowicz <lukasz.wojnilowicz@gmail.com>
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

#include "issuesprocessor.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QDebug>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "enums.h"
#include "iquestioner.h"
#include "ivalidator.h"

namespace Storage {

void IssuesProcessor::createIssuesProcessor(const QVector<Issue> &issues,
                                 FrontendCb frontendHandler,
                                 BackendCb backendHandler,
                                 FinishCb finishHandler
                                 )
{

  auto issuesProcessor = std::make_shared<IssuesProcessor>();
  auto connection = std::make_shared<QMetaObject::Connection>();
  frontendHandler();
  auto slotIssuesProcessed = [=](bool success) mutable {
    disconnect(*connection);
    frontendHandler();
    issuesProcessor.reset();
    finishHandler(success);
  };

  *connection = connect(issuesProcessor.get(), &IssuesProcessor::issuesProcessed, slotIssuesProcessed);
  issuesProcessor->processIssues(issues, frontendHandler, backendHandler);
}

void IssuesProcessor::processIssues(const QVector<Issue> &issues,
                                  FrontendCb frontendHandler,
                                  BackendCb backendHandler,
                                  int issueIdx)
{
  if (issues.isEmpty() || !(issueIdx < issues.count())) {
    emit issuesProcessed(true);
    return;
  }

  const auto &issue = issues.at(issueIdx);

  issueIdx++;

  auto connection = std::make_shared<QMetaObject::Connection>();
  auto slotIssueProcessed = [=](bool success) mutable {
    disconnect(*connection);
//    messageBox.reset();
    if (success)
      processIssues(issues, frontendHandler, backendHandler, issueIdx);
    else
      emit issuesProcessed(false);
  };
  *connection = connect(this, &IssuesProcessor::issueProcessed, slotIssueProcessed);

  auto processorCb = [=](bool success) {
    emit issueProcessed(success);
  };

  auto connection2 = std::make_shared<QMetaObject::Connection>();
  auto slotUserAnswered = [=] (eAnswer answer) {
    disconnect(*connection2);
    switch (answer) {
      case eAnswer::Ok:
        processorCb(true);
        return;
      case eAnswer::Cancel:
      case eAnswer::No:
        processorCb(false);
        return;
      default:
        backendHandler(issue.code, answer, processorCb);
    };
  };

  auto messageBox = frontendHandler();
  *connection2 = connect(messageBox, &IQuestioner::answerAboutIssue, slotUserAnswered);
  messageBox->questionAboutIssue(issue);
}

} // namespace Storage
