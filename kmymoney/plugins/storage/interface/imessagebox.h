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

#ifndef STORAGE_INTERFACE_IMESSAGEBOX_H
#define STORAGE_INTERFACE_IMESSAGEBOX_H

#include <kmm_istorage_export.h>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// QT Includes

#include <QObject>

// ----------------------------------------------------------------------------
// Project Includes


namespace Storage {
struct Issue;
enum class eAnswer;

class KMM_ISTORAGE_EXPORT IMessageBox : public QObject
{
  Q_OBJECT
  Q_DISABLE_COPY(IMessageBox)

public:
  IMessageBox(QObject *parent = nullptr);
  virtual ~IMessageBox() = 0;

  virtual void questionYesNo(const QString &message, const QString &title, const QStringList &data) = 0;
  virtual void warning(const QString &message, const QString &title, const QStringList &data) = 0;
  virtual void error(const QString &message, const QString &title, const QStringList &data) = 0;
  virtual void detailedError(const QString &message, const QString &title, const QStringList &data) = 0;
  virtual void informationList(const QString &message, const QString &title, const QStringList &data) = 0;
  virtual void information(const QString &message, const QString &title, const QStringList &data) = 0;

Q_SIGNALS:
  void answerAboutIssue(eAnswer answer);

protected:
  QString fillPlaceholders(const QString &message, const QStringList &data);
  QObject *m_parent;
};

typedef void(IMessageBox::*IMessageBoxFn)(const QString &message, const QString &title, const QStringList &data);

} // namespace Storage

#endif
