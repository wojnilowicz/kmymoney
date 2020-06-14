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

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// QT Includes

#include <QStringListModel>

// ----------------------------------------------------------------------------
// Project Includes

#ifndef STORAGE_GPG_KEYSMODEL_H
#define STORAGE_GPG_KEYSMODEL_H

namespace Storage {
namespace Gpg {

class KeysModel : public QStringListModel
{
  Q_OBJECT

public:
  KeysModel();
  ~KeysModel();

  Q_INVOKABLE QStringList notSelectedKeys();
  Q_INVOKABLE QStringList selectedKeys(bool pretty = true);
  Q_INVOKABLE void appendKeyIDs(const QStringList &keyIDs);
  void removeKeyIDs(const QModelIndexList &index);
  Q_INVOKABLE void removeKeyIDs(const QList<int> &indexes);
  void removeAllKeyIDs();

  QHash<int, QByteArray> roleNames() const override final;
};

} // namespace Gpg
} // namespace Storage

#endif
