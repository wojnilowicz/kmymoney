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

#ifndef STORAGE_SQL_FRONTEND_QUICK_CREDENTIALS_H
#define STORAGE_SQL_FRONTEND_QUICK_CREDENTIALS_H

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// QT Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "frontend/credentials.h"

class QQmlApplicationEngine;

namespace Storage {
namespace Sql {
namespace Quick {

class Credentials : public Sql::Credentials
{
public:
  Credentials(const QString &requestText, QObject *parent = nullptr);
  ~Credentials() override final;

  void getCredentials() override final;
  void getPassphrase() override final;

private:
  const QScopedPointer<QQmlApplicationEngine, QScopedPointerDeleteLater> m_engine;
};

} // namespace Quick$
} // namespace Sql
} // namespace Storage

#endif
