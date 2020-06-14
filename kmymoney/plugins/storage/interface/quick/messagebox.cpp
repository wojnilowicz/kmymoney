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

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "enums.h"
#include "ivalidator.h"

namespace Storage {
namespace Quick {

eAnswer buttonCodeToAnswer(MessageBox::Answer code)
{
  switch (code) {
    case MessageBox::Answer::Yes:
      return eAnswer::Yes;
    case MessageBox::Answer::No:
      return eAnswer::No;
    default:
      return eAnswer::Cancel;
  }
}

QString fillPlaceholders(const QString &message, const QStringList &data)
{
  QString messageWithData = message;
  for (const auto &dataPart : data)
    messageWithData = messageWithData.arg(dataPart);
  return messageWithData;
}

MessageBox::MessageBox(QObject *parent) :
  IMessageBox(parent),
  m_engine(new QQmlApplicationEngine(parent))
{
  auto typeId = qmlTypeId("MessageBoxAnswers", 0, 1, "Answer");
  if (typeId == -1)
    qmlRegisterType<MessageBox>("MessageBoxAnswers", 0, 1, "Answer");

  auto ctx = m_engine->rootContext();
  ctx->setContextProperty("messageBackend", this);
}

MessageBox::~MessageBox() = default;

void MessageBox::slotIssueAnswered(Answer answer)
{
  // TypeError: Cannot call method 'slotIssueAnswered' of null
  // is Qt bug https://bugreports.qt.io/browse/QTBUG-35933
  // Workaround is:
  // property var firedOnce: false

  emit answerAboutIssue(buttonCodeToAnswer(answer));
}

void MessageBox::questionYesNo(const QString &message, const QString &title, const QStringList &data)
{
  const auto messageWithData = fillPlaceholders(message, data);
  auto ctx = m_engine->rootContext();
  ctx->setContextProperty("issueMessage", messageWithData);
  ctx->setContextProperty("issueTitle", title);
  m_engine->load(QUrl("qrc:/storage/questionyesno.qml"));
}

void MessageBox::warning(const QString &message, const QString &title, const QStringList &data)
{
  const auto messageWithData = fillPlaceholders(message, data);
  auto ctx = m_engine->rootContext();
  ctx->setContextProperty("issueMessage", messageWithData);
  ctx->setContextProperty("issueTitle", title);
  m_engine->load(QUrl("qrc:/storage/warning.qml"));
}

void MessageBox::error(const QString &message, const QString &title, const QStringList &data)
{
  const auto messageWithData = fillPlaceholders(message, data);
  auto ctx = m_engine->rootContext();
  ctx->setContextProperty("issueMessage", messageWithData);
  ctx->setContextProperty("issueTitle", title);
  m_engine->load(QUrl("qrc:/storage/error.qml"));
}

void MessageBox::detailedError(const QString &message, const QString &title, const QStringList &data)
{
  auto ctx = m_engine->rootContext();
  ctx->setContextProperty("issueMessage", message);
  ctx->setContextProperty("issueTitle", title);
  ctx->setContextProperty("detailedError", data.join('\n'));
  m_engine->load(QUrl("qrc:/storage/detailederror.qml"));
}

void MessageBox::informationList(const QString &message, const QString &title, const QStringList &data)
{
  auto ctx = m_engine->rootContext();
  ctx->setContextProperty("issueMessage", message);
  ctx->setContextProperty("issueTitle", title);
  ctx->setContextProperty("issueList", data.join('\n'));
  m_engine->load(QUrl("qrc:/storage/informationList.qml"));
}

void MessageBox::information(const QString &message, const QString &title, const QStringList &data)
{
  const auto messageWithData = fillPlaceholders(message, data);
  auto ctx = m_engine->rootContext();
  ctx->setContextProperty("issueMessage", messageWithData);
  ctx->setContextProperty("issueTitle", title);
  m_engine->load(QUrl("qrc:/storage/information.qml"));
}

} // namespace Quick$
} // namespace Storage
