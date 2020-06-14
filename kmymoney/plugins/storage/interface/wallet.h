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

#ifndef STORAGE_INTERFACE_WALLET_H
#define STORAGE_INTERFACE_WALLET_H

#include <kmm_istorage_export.h>

#include <memory>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// QT Includes

#include <QtGlobal>

// ----------------------------------------------------------------------------
// Project Includes

class QStringList;
class QUrl;

namespace Storage {

class WalletPrivate;
class KMM_ISTORAGE_EXPORT Wallet
{
  Q_DECLARE_PRIVATE(Wallet)

public:
  Wallet();
  ~Wallet();

  void writePassphrase(const QString &keyID, const QString &password);
  QString readPassphrase(const QString &keyID);

  void storeCredentials(const QUrl &urlWithCredentials);
  QUrl restoreCredentials(const QUrl &urlWithoutCredentials);

private:
  const std::unique_ptr<WalletPrivate> d_ptr;
};

} // namespace Storage

#endif
