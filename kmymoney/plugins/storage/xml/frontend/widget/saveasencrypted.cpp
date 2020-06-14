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

// ----------------------------------------------------------------------------
// KDE Includes

#include <KLineEdit>

// ----------------------------------------------------------------------------
// Project Includes

#include "mymoney/mymoneyexception.h"

#include "storage/interface/widget/messagebox.h"
#include "storage/interface/issuesprocessor.h"
#include "storage/interface/ivalidator.h"
#include "storage/interface/enums.h"

#include "frontend/saveasencrypted_p.h"
#include "frontend/widget/questioner.h"

#include "backend/url.h"
#include "backend/backend.h"

#include "gpg/frontend/widget/keydownloader.h"
#include "gpg/frontend/widget/keysselector.h"
#include "gpg/frontend/widget/unusedkeysselector.h"
#include "gpg/backend/keysmodel.h"
#include "gpg/backend/backend.h"

#include "ui_saveasencrypted.h"

namespace Storage {
namespace Xml {
namespace Widget {

class SaveAsEncryptedPrivate : public Xml::SaveAsEncryptedPrivate
{
public:
  SaveAsEncryptedPrivate(SaveAsEncrypted *q);
  ~SaveAsEncryptedPrivate() = default;

  std::shared_ptr<Gpg::KeyDownloader> keyDownloaderFactory() override final;
  std::shared_ptr<IQuestioner> questionerFactory() override final;

  const QScopedPointer<Ui::SaveAsEncrypted> m_ui;
  const QScopedPointer<QWidget, QScopedPointerDeleteLater> m_container;
};

SaveAsEncryptedPrivate::SaveAsEncryptedPrivate(SaveAsEncrypted *q) :
  Xml::SaveAsEncryptedPrivate(*q, *new Gpg::Widget::KeysSelector),
  m_ui(new Ui::SaveAsEncrypted),
  m_container(new QWidget)
{  
}

std::shared_ptr<Gpg::KeyDownloader> SaveAsEncryptedPrivate::keyDownloaderFactory()
{
  return std::make_shared<Gpg::Widget::KeyDownloader>(m_parent);
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
  d->m_ui->setupUi(d->m_container.get());

  d->m_ui->fileDialog->setFilter(QStringLiteral("*.kmy|KMyMoneyNEXT files\n"));
  d->m_ui->fileDialog->setStartDir(startDirectory);

  // don't change to KLineEdit::textEdited because that doesn't emit signal if path is selected by file dialog
  connect(d->m_ui->fileDialog->lineEdit(), &KLineEdit::textChanged,
          this, &SaveAsEncrypted::setFilePath);

  auto layout = qobject_cast<QVBoxLayout *>(d->m_container->layout());
  layout->insertWidget(layout->count(), qobject_cast<QWidget *>(d->m_keySelector->uiPart()));
}

SaveAsEncrypted::~SaveAsEncrypted() = default;

QObject *SaveAsEncrypted::uiPart()
{
  Q_D(SaveAsEncrypted);
  return d->m_container.get();
}

void SaveAsEncrypted::setStorageUrl(const QUrl &storageUrl)
{
  Q_D(SaveAsEncrypted);
  d->m_ui->fileDialog->lineEdit()->setText(storageUrl.path());
  Xml::SaveAsEncrypted::setStorageUrl(storageUrl);
}

} // namespace Widget
} // namespace Xml
} // namespace Storage
