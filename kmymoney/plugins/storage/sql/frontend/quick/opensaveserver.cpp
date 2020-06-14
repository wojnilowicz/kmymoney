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

#include "opensaveserver.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFileInfo>

// ----------------------------------------------------------------------------
// KDE Includes

#include <KLocalizedString>

// ----------------------------------------------------------------------------
// Project Includes

#include "frontend/opensaveserver_p.h"
#include "backend/url.h"

namespace Storage {
namespace Sql {
namespace Quick {

class OpenSaveServerPrivate : public Sql::OpenSaveServerPrivate
{
public:
  OpenSaveServerPrivate();
  ~OpenSaveServerPrivate() = default;

  QScopedPointer<QObject, QScopedPointerDeleteLater> m_uiPart;
  const QScopedPointer<QQmlApplicationEngine, QScopedPointerDeleteLater> m_engine;
};

OpenSaveServerPrivate::OpenSaveServerPrivate() :
  m_engine(new QQmlApplicationEngine)
{
}

OpenSaveServer::OpenSaveServer(Backend *backend, eType storageType, eAction action) :
  Sql::OpenSaveServer(*new OpenSaveServerPrivate, backend, storageType, action)
{
  Q_D(OpenSaveServer);
  QQmlComponent component(d->m_engine.get(), QUrl(QLatin1String("qrc:/storage/sql/opensaveserver.qml")));
  d->m_engine->rootContext()->setContextProperty("openBackend", this);
  d->m_uiPart.reset(component.create());
}

OpenSaveServer::~OpenSaveServer() = default;

QObject *OpenSaveServer::uiPart()
{
  Q_D(OpenSaveServer);
  return d->m_uiPart.get();
}

void OpenSaveServer::setStorageUrl(const QUrl &storageUrl)
{
  Url enhancedUrl(storageUrl);
  setHostName(storageUrl.host());
  setDatabaseName(enhancedUrl.databaseName());
}

QString OpenSaveServer::hostName() const
{
  Q_D(const OpenSaveServer);
  return d->m_hostName;
}

QString OpenSaveServer::databaseName() const
{
  Q_D(const OpenSaveServer);
  return d->m_databaseName;
}

QString OpenSaveServer::databaseNameTooltip() const
{
  Q_D(const OpenSaveServer);
  return d->m_databaseNameTooltip;
}

void OpenSaveServer::setHostName(const QString &hostName)
{
  Q_D(OpenSaveServer);
  if (d->m_hostName != hostName) {
    Sql::OpenSaveServer::setHostName(hostName);
    emit hostNameChanged(hostName);
    validateUserSelections();
  }
}

void OpenSaveServer::setDatabaseName(const QString &databaseName)
{
  Q_D(OpenSaveServer);
  if (d->m_databaseName!= databaseName) {
    Sql::OpenSaveServer::setDatabaseName(databaseName);
    emit databaseNameChanged(databaseName);

  }
}

void OpenSaveServer::setDatabaseNameTooltip(const QString &databaseNameTooltip)
{
  emit databaseNameTooltipChanged(databaseNameTooltip);
}

} // namespace Quick$
} // namespace Sql
} // namespace Storage
