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

#ifndef STORAGE_INTERFACE_ISSUESPROCESSOR_H
#define STORAGE_INTERFACE_ISSUESPROCESSOR_H

#include <kmm_istorage_export.h>
#include <memory>
#include <functional>

// ----------------------------------------------------------------------------
// QT Includes

#include <QObject>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

namespace Storage {

class IQuestioner;
class Issue;
enum class eIssue;
enum class eAnswer;

typedef const std::function<void(bool success)> & FinishCb;
typedef const std::function<void(bool success)> & ProcessorCb;
typedef const std::function<IQuestioner* ()> & FrontendCb;
typedef const std::function<void (eIssue issue, eAnswer answer, ProcessorCb success)> & BackendCb;

class KMM_ISTORAGE_EXPORT IssuesProcessor : public QObject
{
  Q_OBJECT

public:
  using QObject::QObject;

  static void createIssuesProcessor(const QVector<Issue> &issues,
                                   FrontendCb frontendHandler,
                                   BackendCb backendHandler,
                                   FinishCb finishHandler);

private:
  void processIssues(const QVector<Issue> &issues,
                     FrontendCb frontendHandler,
                     BackendCb backendHandler,
                     int issueIdx = 0);

Q_SIGNALS:
  void issueProcessed(bool success);
  void issuesProcessed(bool success);
};

} // namespace Storage

#endif
