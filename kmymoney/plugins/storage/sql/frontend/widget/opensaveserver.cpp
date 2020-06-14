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

#include <QUrl>
#include <QUrlQuery>
#include <QDebug>

// ----------------------------------------------------------------------------
// KDE Includes

#include <KLineEdit>

// ----------------------------------------------------------------------------
// Project Includes

#include "mymoney/mymoneyexception.h"

#include "frontend/opensaveserver_p.h"
#include "storage/interface/enums.h"
#include "backend/url.h"
#include "backend/backend.h"
#include "ui_opensaveserver.h"

namespace Storage {
namespace Sql {
namespace Widget {

class OpenSaveServerPrivate : public Sql::OpenSaveServerPrivate
{
public:
  OpenSaveServerPrivate();
  ~OpenSaveServerPrivate() = default;

  const QScopedPointer<Ui::OpenSaveServer> m_ui;
  const QScopedPointer<QWidget, QScopedPointerDeleteLater> m_container;
};

OpenSaveServerPrivate::OpenSaveServerPrivate() :
  m_ui(new Ui::OpenSaveServer),
  m_container(new QWidget)
{
}

OpenSaveServer::OpenSaveServer(Backend *backend, eType storageType, eAction action) :
  Sql::OpenSaveServer(*new OpenSaveServerPrivate, backend, storageType, action)
{
  Q_D(OpenSaveServer);
  d->m_ui->setupUi(d->m_container.get());

  connect(d->m_ui->hostNameInput, &QLineEdit::textChanged,
          this, &OpenSaveServer::setHostName);
  connect(d->m_ui->databaseNameInput, &QLineEdit::textChanged,
          this, &OpenSaveServer::setDatabaseName);
}

OpenSaveServer::~OpenSaveServer() = default;

QObject *OpenSaveServer::uiPart()
{
  Q_D(OpenSaveServer);
  return d->m_container.get();
}

void OpenSaveServer::setStorageUrl(const QUrl &storageUrl)
{
  Q_D(OpenSaveServer);
  Url enhancedUrl(storageUrl);
  d->m_ui->hostNameInput->setText(storageUrl.host());
  d->m_ui->databaseNameInput->setText(enhancedUrl.databaseName());
}

void OpenSaveServer::setDatabaseNameTooltip(const QString &databaseNameTooltip)
{
  Q_D(OpenSaveServer);
  d->m_ui->databaseNameInput->setToolTip(databaseNameTooltip);
}

} // namespace Widget
} // namespace Sql
} // namespace Storage
