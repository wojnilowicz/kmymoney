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
#include <QDebug>
#include <QPushButton>
#include <QLabel>
#include <QMetaObject>
#include <QTimer>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "frontend/opensave_p.h"
#include "storage/interface/idialogpartopensave.h"
#include "storage/interface/enums.h"

#include "ui_opensave.h"

#include "configuration.h"

namespace Storage {
namespace Manager {
namespace Widget {

class OpenSavePrivate : public Manager::OpenSavePrivate
{
public:
  OpenSavePrivate(QObject *parent, eAction actionType, const StoragePluginsMap &storagePlugins);
  ~OpenSavePrivate() override final = default;

  QScopedPointer<QLabel, QScopedPointerDeleteLater> m_dummyLabel;
  QScopedPointer<QDialog, QScopedPointerDeleteLater> m_dialog;
  const QScopedPointer<Ui::OpenSave> ui;
};

OpenSavePrivate::OpenSavePrivate(QObject *parent, eAction actionType, const StoragePluginsMap &storagePlugins) :
  Manager::OpenSavePrivate(eUI::Widget, actionType, storagePlugins),
  m_dialog(new QDialog(qobject_cast<QWidget *>(parent))),
  ui(new Ui::OpenSave)
{
}

OpenSave::OpenSave(QObject *parent,
                   const StoragePluginsMap &storagePlugins,
                   eAction actionType,
                   const QUrl &presetUrl) :
  Manager::OpenSave(*new OpenSavePrivate(parent, actionType, storagePlugins))
{
  Q_D(OpenSave);

  d->ui->setupUi(d->m_dialog.get());

  const QMap<eAction, QString> windowTitles {
    {eAction::Open, i18n("Select file for opening")},
    {eAction::Save, i18n("Select file for saving")}
  };
  d->m_dialog->setWindowTitle(windowTitles.value(d->m_actionType));

  const auto storageTypes = actionType == eAction::Open ? d->storageTypesForOpen() : d->storageTypesForSave();
  const auto storageTypeNames = d->storageTypeNames(storageTypes);
  const auto presetStorageTypeIndex = d->presetStorageTypeIndex(presetUrl, storageTypes);
  const auto storageTypeIndex = presetStorageTypeIndex != -1 ? presetStorageTypeIndex : d->fallbackStorageTypeIndex(storageTypes);

  d->ui->cbStorageTypes->addItems(storageTypeNames);
  if (storageTypeIndex != -1)
    d->ui->cbStorageTypes->setCurrentIndex(storageTypeIndex);

  d->m_dialog->open();

  connect(d->ui->cbStorageTypes, &QComboBox::currentTextChanged,
          this, &OpenSave::slotStorageTypeChanged);

  // keep the dialog open because validation in plugin may be negative and user might want to retry
  connect(d->ui->buttonBox, &QDialogButtonBox::accepted, this, &OpenSave::accepted);

  // notify upper class of rejection, so that it can destroy this class
  connect(d->ui->buttonBox, &QDialogButtonBox::rejected, d->m_dialog.get(), &QDialog::reject);
  connect(d->m_dialog.get(), &QDialog::rejected, this, &OpenSave::rejected);

  const auto &okButton = d->ui->buttonBox->button(QDialogButtonBox::Ok);
  connect(this, &OpenSave::userBasedValidityChanged,
          okButton, &QPushButton::setEnabled);

  slotStorageTypeChanged(d->ui->cbStorageTypes->currentText());

  if (presetStorageTypeIndex != -1)
    if (auto cachedDialogPart = d->dialogPart(d->m_storageType))
      cachedDialogPart->setStorageUrl(presetUrl);
}

OpenSave::~OpenSave() = default;

QObject *OpenSave::uiPart() const
{
  Q_D(const OpenSave);
  return d->m_dialog.get();
}

void OpenSave::slotStorageTypeChanged(const QString &storageType)
{
  Q_D(OpenSave);
  // remove old storage widget from the layout
  auto layout = qobject_cast<QVBoxLayout *>(d->ui->fmDialogPart->layout());
  while (layout->count() > 0) {  // delete everything
    auto layoutItem = layout->takeAt(0);
    layoutItem->widget()->setParent(nullptr); // otherwise old widget stays visible in the layout
    delete layoutItem;
  }

  Manager::OpenSave::slotStorageTypeChanged(storageType);

  if (auto cachedDialogPart = d->dialogPart(d->m_storageType)) {
    // add storage widget chosen by the user to the layout
    layout->insertWidget(0, qobject_cast<QWidget *>(cachedDialogPart->uiPart()));
  } else {
    d->m_dummyLabel.reset(new QLabel("Not available."));
    d->m_dummyLabel->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    layout->insertWidget(0, qobject_cast<QWidget *>(d->m_dummyLabel.get()));
    emit userBasedValidityChanged(false);
  }

  QMetaObject::invokeMethod(this, "slotAdjustDialogSize", Qt::QueuedConnection);
}

void OpenSave::slotAdjustDialogSize()
{
  Q_D(OpenSave);
  d->m_dialog->adjustSize();
}

} // namespace Widget
} // namespace Manager
} // namespace Storage
