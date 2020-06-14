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

#include "keysmodel.h"

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// QT Includes

#include <QDebug>

// ----------------------------------------------------------------------------
// Project Includes

 #include "backend.h"

namespace Storage {
namespace Gpg {

KeysModel::KeysModel() = default;
KeysModel::~KeysModel() = default;

QStringList KeysModel::notSelectedKeys()
{
   const auto selectedKeys = KeysModel::selectedKeys();
   Gpg::Backend gpgBackend;
   const auto availableKeys = gpgBackend.systemEncryptionKeys();
   const auto availableKeysPretty = gpgBackend.prettyRepresentation(availableKeys);

  QStringList notSelectedKeys;

   for (const auto &availableKeyPretty : availableKeysPretty)
     if (!selectedKeys.contains(availableKeyPretty))
       notSelectedKeys.append(availableKeyPretty);

  return notSelectedKeys;
}

QStringList KeysModel::selectedKeys(bool pretty)
{
  QStringList selectedKeys;

  for (auto row = 0; row < rowCount(); ++row) {
    auto key = index(row).data().toString();
    if (!pretty)
      key = key.left(16);
    selectedKeys.append(key);
  }

  return selectedKeys;
}

void KeysModel::appendKeyIDs(const QStringList &keyIDs)
{
  if (keyIDs.isEmpty())
    return;
  Gpg::Backend gpgBackend;
  auto keyIDsPretty = gpgBackend.prettyRepresentation(keyIDs);
  auto startRowIdx = rowCount();
  insertRows(startRowIdx, keyIDsPretty.count());
  for (const auto &keyID: keyIDsPretty)
    setData(index(startRowIdx++), keyID, Qt::DisplayRole);
}

void KeysModel::removeKeyIDs(const QModelIndexList &indexes)
{
  for (const auto &index : indexes)
    removeRow(index.row());
}

void KeysModel::removeKeyIDs(const QList<int> &indexes)
{
  for (auto i = indexes.count() - 1; i >= 0; --i)
    removeRow(indexes.at(i));
}

void KeysModel::removeAllKeyIDs()
{
  removeRows(0, rowCount());
}

QHash<int, QByteArray> KeysModel::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles[Qt::DisplayRole] = "keyId";
  return roles;
}

} // namespace Gpg
} // namespace Storage
