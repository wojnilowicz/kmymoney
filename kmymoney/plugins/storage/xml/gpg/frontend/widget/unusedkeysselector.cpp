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

#include "unusedkeysselector.h"

// ----------------------------------------------------------------------------
// QT Includes

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "ui_unusedkeysselector.h"

namespace Storage {
namespace Gpg {
namespace Widget {

UnusedKeysSelector::UnusedKeysSelector(QWidget *parent, const QStringList &keyIDs) :
  QDialog(parent),
  ui(std::make_unique<Ui::UnusedKeysSelector>())
{
  ui->setupUi(this);
  ui->keysList->addItems(keyIDs);
  connect(ui->keysListButtons, &QDialogButtonBox::accepted, this, &UnusedKeysSelector::accept);
  connect(ui->keysListButtons, &QDialogButtonBox::rejected, this, &UnusedKeysSelector::reject);
}

UnusedKeysSelector::~UnusedKeysSelector() = default;

QStringList UnusedKeysSelector::selectedKeyIDs() const
{
  QStringList keyIDs;
  const auto keyIDItems = ui->keysList->selectedItems();
  for (const auto &keyIDItem : keyIDItems)
    keyIDs.append(keyIDItem->text());
  return keyIDs;
}

} // namespace Widget
} // namespace Gpg
} // namespace Storage
