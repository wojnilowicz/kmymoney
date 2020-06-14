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

#include "keysselector.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QDebug>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "gpg/frontend/keysselector_p.h"

#include "ui_keysselector.h"

#include "unusedkeysselector.h"
#include "gpg/backend/keysmodel.h"

namespace Storage {
namespace Gpg {
namespace Widget {

class KeysSelectorPrivate : public Gpg::KeysSelectorPrivate
{
public:
  KeysSelectorPrivate();
  ~KeysSelectorPrivate() = default;

  const QScopedPointer<Ui::KeysSelector> m_ui;
  const QScopedPointer<QWidget, QScopedPointerDeleteLater> m_container;
};

KeysSelectorPrivate::KeysSelectorPrivate() :
  m_ui(new Ui::KeysSelector),
  m_container(new QWidget)
{
}

KeysSelector::KeysSelector() :
  Gpg::KeysSelector(*new KeysSelectorPrivate)
{
  Q_D(KeysSelector);
  d->m_ui->setupUi(d->m_container.get());

  connect(d->m_ui->addButton,    &QPushButton::clicked, this, &KeysSelector::slotAddKeysClicked);
  connect(d->m_ui->removeButton, &QPushButton::clicked, this, &KeysSelector::slotRemoveKeysClicked);
  d->m_ui->keysList->setModel(d->m_keysModel.get());
}

KeysSelector::~KeysSelector() = default;

QObject *KeysSelector::uiPart()
{
  Q_D(KeysSelector);
  return d->m_container.get();
}

void KeysSelector::slotAddKeysClicked()
{
  Q_D(KeysSelector);
  auto dialog = std::make_shared<UnusedKeysSelector>(d->m_container.get(), d->m_keysModel->notSelectedKeys());
  connect(dialog.get(), &QDialog::finished, this, [=](int result) mutable {
    if (result == QDialog::Accepted) {
      d->m_keysModel->appendKeyIDs(dialog->selectedKeyIDs());
      emit keysListChanged(d->m_keysModel->selectedKeys(false));
    }
    dialog.reset();
  });
  dialog->open();
}

void KeysSelector::slotRemoveKeysClicked()
{
  Q_D(KeysSelector);
  auto selectionModel = d->m_ui->keysList->selectionModel();
  d->m_keysModel->removeKeyIDs(selectionModel->selectedIndexes());
  emit keysListChanged(d->m_keysModel->selectedKeys(false));
}

} // namespace Widget
} // namespace Gpg
} // namespace Storage
