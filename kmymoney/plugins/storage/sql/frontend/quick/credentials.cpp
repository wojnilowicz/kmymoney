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

#include "credentials.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QQmlApplicationEngine>
#include <QQmlContext>

// ----------------------------------------------------------------------------
// KDE Includes

#include <KLocalizedString>

// ----------------------------------------------------------------------------
// Project Includes

namespace Storage {
namespace Sql {
namespace Quick {

Credentials::Credentials(const QString &requestText, QObject *parent) :
  m_engine(new QQmlApplicationEngine(parent))
{
  m_engine->rootContext()->setContextProperty("backend", this);
  m_engine->rootContext()->setContextProperty("requestLabelText", requestText);
}

Credentials::~Credentials() = default;

void Credentials::getCredentials()
{
  m_engine->rootContext()->setContextProperty("isUserNameVisible", true);
  m_engine->load(QUrl(QLatin1String("qrc:/storage/sql/credentials.qml")));

//  emit credentials("kmymoneynextuser", "passphrase");
}

void Credentials::getPassphrase()
{
  m_engine->rootContext()->setContextProperty("isUserNameVisible", false);
  m_engine->load(QUrl(QLatin1String("qrc:/storage/sql/credentials.qml")));
}


} // namespace Quick$
} // namespace Sql
} // namespace Storage
