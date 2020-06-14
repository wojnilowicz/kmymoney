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

#include <QUrl>
#include <QUrlQuery>
#include <QStandardPaths>
#include <QDebug>
#include <QFileInfo>

// ----------------------------------------------------------------------------
// KDE Includes

#include <KLineEdit>

// ----------------------------------------------------------------------------
// Project Includes

#include "mymoney/mymoneyexception.h"

#include "storage/interface/enums.h"

#include "frontend/opensave_p.h"

#include "backend/url.h"
#include "backend/backend.h"

#include "ui_opensave.h"

namespace Storage {
namespace Xml {
namespace Widget {

class OpenSavePrivate : public Xml::OpenSavePrivate
{
public:
  OpenSavePrivate();
  ~OpenSavePrivate() = default;

  const QScopedPointer<Ui::OpenSave> m_ui;
  const QScopedPointer<QWidget, QScopedPointerDeleteLater> m_container;
};

OpenSavePrivate::OpenSavePrivate() :
  m_ui(new Ui::OpenSave),
  m_container(new QWidget)
{
}

OpenSave::OpenSave(Backend *backend, eType storageType, eAction actionType, const QUrl &startDirectory) :
  Xml::OpenSave(*new OpenSavePrivate, backend, storageType, actionType)
{
  Q_D(OpenSave);
  d->m_ui->setupUi(d->m_container.get());
  d->m_ui->fileDialog->setFilter(QStringLiteral("*.kmy|KMyMoneyNEXT files\n"));
  d->m_ui->fileDialog->setStartDir(startDirectory);

  const QMap<eAction, QFileDialog::AcceptMode> fileDialogModeFromAction {
    {eAction::Open, QFileDialog::AcceptOpen},
    {eAction::Save, QFileDialog::AcceptSave}
  }; 
  d->m_ui->fileDialog->setAcceptMode(fileDialogModeFromAction.value(actionType, QFileDialog::AcceptOpen));

  auto fileDialogModes = KFile::File | KFile::LocalOnly;
  if (actionType == eAction::Open)
    fileDialogModes |= KFile::ExistingOnly;
  d->m_ui->fileDialog->setMode(fileDialogModes);

  const QMap<eAction, QString> fileDialogLabelFromAction {
    {eAction::Open, i18n("Location of storage to open")},
    {eAction::Save, i18n("Location to save storage")}
  };
  d->m_ui->fileDialogLabel->setText(fileDialogLabelFromAction.value(actionType));

  connect(d->m_ui->fileDialog->lineEdit(), &KLineEdit::textChanged,
          this, &OpenSave::setFilePath);
}

OpenSave::~OpenSave() = default;

QObject *OpenSave::uiPart()
{
  Q_D(OpenSave);
  return d->m_container.get();
}

void OpenSave::setStorageUrl(const QUrl &storageUrl)
{
  Q_D(OpenSave);
  d->m_ui->fileDialog->lineEdit()->setText(storageUrl.path());
}

} // namespace Widget
} // namespace Xml
} // namespace Storage
