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

#ifndef STORAGE_GPG_FRONTEND_KEYSSELECTOR_H
#define STORAGE_GPG_FRONTEND_KEYSSELECTOR_H

#include <memory>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// QT Includes

#include <QObject>

// ----------------------------------------------------------------------------
// Project Includes

namespace Storage {
namespace Gpg {

class KeysSelectorPrivate;
class KeysSelector : public QObject
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(KeysSelector)
//  Q_PROPERTY(QStringList keysList READ keysList WRITE setKeysList NOTIFY keysListChanged)

public:
  virtual ~KeysSelector() = 0;

  Q_INVOKABLE virtual QObject *uiPart() = 0;
  void setKeys(const QStringList &keyIDs);

//  QStringList keysList() const;
//  void setKeysList(const QStringList &keysList);

Q_SIGNALS:
  void keysListChanged(const QStringList &keyList);

protected:
  const std::unique_ptr<KeysSelectorPrivate> d_ptr;
  KeysSelector(KeysSelectorPrivate &d);
};

} // namespace Gpg
} // namespace Storage

#endif
