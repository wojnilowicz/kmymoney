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

#include "opensave.h"

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

#include "frontend/opensave_p.h"

namespace Storage {
namespace Sql {
namespace Quick {

class OpenSavePrivate : public Sql::OpenSavePrivate
{
public:
  OpenSavePrivate();
  ~OpenSavePrivate() = default;

  QScopedPointer<QObject, QScopedPointerDeleteLater> m_uiPart;
  const QScopedPointer<QQmlApplicationEngine, QScopedPointerDeleteLater> m_engine;
};

OpenSavePrivate::OpenSavePrivate() :
  m_engine(new QQmlApplicationEngine)
{
}

OpenSave::OpenSave(Backend *backend, eType storageType, eAction actionType, const QUrl &startDirectory) :
  Sql::OpenSave(*new OpenSavePrivate, backend, storageType, actionType)
{
  Q_D(OpenSave);
  QQmlComponent component(d->m_engine.get(), QUrl(QLatin1String("qrc:/storage/sql/opensave.qml")));
  d->m_engine->rootContext()->setContextProperty("openBackend", this);
  const QMap<eAction, bool> fileDialogModeFromAction {
    {eAction::Open, true},
    {eAction::Save, false}
  };
  d->m_engine->rootContext()->setContextProperty("selectExistingFile", fileDialogModeFromAction.value(actionType, true));

  const QMap<eAction, QString> fileDialogLabelFromAction {
    {eAction::Open, i18n("Location of storage to open")},
    {eAction::Save, i18n("Location to save storage")}
  };

  d->m_engine->rootContext()->setContextProperty("fileDialogLabel", fileDialogLabelFromAction.value(actionType));

  d->m_engine->rootContext()->setContextProperty("startDir", startDirectory);
  d->m_uiPart.reset(component.create());
}

OpenSave::~OpenSave() = default;

QObject *OpenSave::uiPart()
{
  Q_D(OpenSave);
  return d->m_uiPart.get();
}

void OpenSave::setStorageUrl(const QUrl &storageUrl)
{
  setFilePath(storageUrl.path());
}

QString OpenSave::filePath() const
{
  Q_D(const OpenSave);
  return d->m_filePath;
}

void OpenSave::setFilePath(const QString &filePath)
{
  Q_D(OpenSave);
  if (d->m_filePath != filePath) {
    Sql::OpenSave::setFilePath(filePath);
    emit filePathChanged(filePath);
    validateUserSelections();
  }
}

} // namespace Quick$
} // namespace Sql
} // namespace Storage
