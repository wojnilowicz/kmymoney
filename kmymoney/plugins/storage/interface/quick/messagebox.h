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

#ifndef STORAGE_INTERFACE_QUICK_MESSAGEBOX_H
#define STORAGE_INTERFACE_QUICK_MESSAGEBOX_H

#include <kmm_istorage_export.h>

#include <memory>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// QT Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "imessagebox.h"

class QQmlApplicationEngine;

namespace Storage {
namespace Quick {

class KMM_ISTORAGE_EXPORT MessageBox : public IMessageBox
{
  Q_OBJECT

public:
  MessageBox(QObject *parent = nullptr);
  ~MessageBox() override;

  void questionYesNo(const QString &message, const QString &title, const QStringList &data) override;
  void warning(const QString &message, const QString &title, const QStringList &data) override;
  void error(const QString &message, const QString &title, const QStringList &data) override;
  void detailedError(const QString &message, const QString &title, const QStringList &data) override;
  void informationList(const QString &message, const QString &title, const QStringList &data) override;
  void information(const QString &message, const QString &title, const QStringList &data) override;

  enum Answer
  {
    Yes,
    No,
    Cancel
  };
  Q_ENUMS(Answer)

public Q_SLOTS:
  void slotIssueAnswered(Answer answer);

private:
  const QScopedPointer<QQmlApplicationEngine, QScopedPointerDeleteLater> m_engine;
};

} // namespace Quick$
} // namespace Storage

#endif
