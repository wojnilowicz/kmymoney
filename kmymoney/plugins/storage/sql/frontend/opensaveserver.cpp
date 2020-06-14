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
#include "opensaveserver_p.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QUrl>
#include <QStandardPaths>
#include <QDebug>
#include <QFileInfo>

// ----------------------------------------------------------------------------
// KDE Includes

#include <KLocalizedString>

// ----------------------------------------------------------------------------
// Project Includes

#include "mymoney/mymoneyexception.h"

#include "storage/interface/enums.h"
#include "backend/url.h"
#include "backend/backend.h"
#include "configuration.h"

namespace Storage {
namespace Sql {

OpenSaveServerPrivate::~OpenSaveServerPrivate() = default;

QString OpenSaveServerPrivate::validateDatabaseName() const
{
  if (m_databaseName.contains(' '))
    return i18n("Cannot contain spaces.");
  return QString();
}

OpenSaveServer::OpenSaveServer(OpenSaveServerPrivate &d, Backend *backend, eType storageType, eAction action) :
  d_ptr(&d)
{
  d.m_backend = backend;
  d.m_storageType = storageType;
  d.m_action = action;
}

OpenSaveServer::~OpenSaveServer() = default;

QUrl OpenSaveServer::storageUrl() const
{
  Q_D(const OpenSaveServer);

  QUrl url;
  Url storageUrl(url);
  storageUrl.setPluginScheme();
  storageUrl.encodeStorageType(d->m_storageType);
  storageUrl.setDatabaseName(d->m_databaseName);
  storageUrl.setHost(d->m_hostName);

  if (d->m_backend->isKMyMoneyType(d->m_storageType))
    storageUrl.setNoSwitch();

  return std::move(storageUrl);
}

void OpenSaveServer::validateUserSelections()
{
  Q_D(OpenSaveServer);
  d->m_databaseNameTooltip = d->validateDatabaseName();
  setDatabaseNameTooltip(d->m_databaseNameTooltip);

  bool isValid = !d->m_hostName.isEmpty() && !d->m_databaseName.isEmpty() && d->m_databaseNameTooltip.isEmpty();
  emit userBasedValidityChanged(isValid);
}

void OpenSaveServer::setHostName(const QString &hostName)
{
  Q_D(OpenSaveServer);
  d->m_hostName = hostName;
  validateUserSelections();
}

void OpenSaveServer::setDatabaseName(const QString &databaseName)
{
  Q_D(OpenSaveServer);
  d->m_databaseName = databaseName;
  d->validateDatabaseName();
  validateUserSelections();
}

} // namespace Sql
} // namespace Storage
