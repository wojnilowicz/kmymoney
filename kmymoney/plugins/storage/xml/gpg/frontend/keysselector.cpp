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
#include "keysselector_p.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QDebug>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "gpg/backend/keysmodel.h"

namespace Storage {
namespace Gpg {

KeysSelectorPrivate::KeysSelectorPrivate() :
  m_keysModel(new KeysModel)
{
}

KeysSelector::KeysSelector(KeysSelectorPrivate &d) :
  d_ptr(&d)
{
}

KeysSelector::~KeysSelector() = default;

void KeysSelector::setKeys(const QStringList &keyIDs)
{
  Q_D(KeysSelector);
  auto currentKeyIDs = d->m_keysModel->selectedKeys(false);
  if (currentKeyIDs != keyIDs) {
    d->m_keysModel->removeAllKeyIDs();
    d->m_keysModel->appendKeyIDs(keyIDs);
  }
}

} // namespace Gpg
} // namespace Storage
