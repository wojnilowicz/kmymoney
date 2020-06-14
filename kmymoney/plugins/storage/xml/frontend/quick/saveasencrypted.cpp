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

#include "saveasencrypted.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QUrl>
#include <QStandardPaths>
#include <QDebug>
#include <QFileInfo>

#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlApplicationEngine>
#include <QQmlContext>


// ----------------------------------------------------------------------------
// KDE Includes

#include <KLineEdit>

// ----------------------------------------------------------------------------
// Project Includes

#include "mymoney/mymoneyexception.h"

#include "storage/interface/quick/messagebox.h"
#include "storage/interface/issuesprocessor.h"
#include "storage/interface/ivalidator.h"
#include "storage/interface/enums.h"

#include "frontend/saveasencrypted_p.h"
#include "frontend/quick/questioner.h"

#include "backend/url.h"
#include "backend/backend.h"

#include "gpg/frontend/quick/keydownloader.h"
#include "gpg/frontend/quick/keysselector.h"
#include "gpg/backend/keysmodel.h"
#include "gpg/backend/backend.h"

namespace Storage {
namespace Xml {
namespace Quick {

class SaveAsEncryptedPrivate : public Xml::SaveAsEncryptedPrivate
{
public:
  SaveAsEncryptedPrivate(SaveAsEncrypted *q);
  ~SaveAsEncryptedPrivate() = default;

  std::shared_ptr<Gpg::KeyDownloader> keyDownloaderFactory() override final;
  std::shared_ptr<IQuestioner> questionerFactory() override final;

  QScopedPointer<QObject, QScopedPointerDeleteLater> m_uiPart;
  const QScopedPointer<QQmlApplicationEngine, QScopedPointerDeleteLater> m_engine;
};

SaveAsEncryptedPrivate::SaveAsEncryptedPrivate(SaveAsEncrypted *q) :
  Xml::SaveAsEncryptedPrivate(*q, *new Gpg::Quick::KeysSelector),
  m_engine(new QQmlApplicationEngine)
{
}

std::shared_ptr<Gpg::KeyDownloader> SaveAsEncryptedPrivate::keyDownloaderFactory()
{
  return std::make_shared<Gpg::Quick::KeyDownloader>(m_parent);
}

std::shared_ptr<IQuestioner> SaveAsEncryptedPrivate::questionerFactory()
{
  return std::make_shared<Questioner>(m_parent);
}

SaveAsEncrypted::SaveAsEncrypted(QObject *parent, Backend *backend, eType storageType, const QUrl &startDirectory) :
  Xml::SaveAsEncrypted(*new SaveAsEncryptedPrivate(this), backend, storageType)
{
  Q_D(SaveAsEncrypted);
  d->m_parent = parent;

  QQmlComponent component(d->m_engine.get(), QUrl(QLatin1String("qrc:/storage/xml/saveasencrypted.qml")));
  d->m_engine->rootContext()->setContextProperty("saveAsEncryptedBackend", this);
  d->m_engine->rootContext()->setContextProperty("keysSelector", d->m_keySelector.get());
  d->m_engine->rootContext()->setContextProperty("startDir", startDirectory);
  d->m_uiPart.reset(component.create());
}

SaveAsEncrypted::~SaveAsEncrypted() = default;

QObject *SaveAsEncrypted::uiPart()
{
  Q_D(SaveAsEncrypted);
  return d->m_uiPart.get();
}

QString SaveAsEncrypted::filePath() const
{
  Q_D(const SaveAsEncrypted);
  return d->m_filePath;
}

void SaveAsEncrypted::setFilePath(const QString &filePath)
{
  Q_D(SaveAsEncrypted);
  if (d->m_filePath != filePath) {
    Xml::SaveAsEncrypted::setFilePath(filePath);
    emit filePathChanged(filePath);
  }
}

} // namespace Quick$
} // namespace Xml
} // namespace Storage
