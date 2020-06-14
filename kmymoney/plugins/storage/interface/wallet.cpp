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

#include "wallet.h"

// ----------------------------------------------------------------------------
// KDE Includes

#include <KWallet>

// ----------------------------------------------------------------------------
// QT Includes

#include <QMap>
#include <QUrl>

// ----------------------------------------------------------------------------
// Project Includes

#include "iurl.h"

namespace Storage {

class WalletPrivate {

public:
  WalletPrivate();
  ~WalletPrivate();

  void openWallet();

  QString keyFromUrl(const QUrl &url) const;

  KWallet::Wallet *m_wallet = nullptr;
};

WalletPrivate::WalletPrivate()
{
  openWallet();
  const QString walletFolderName = QStringLiteral("KMyMoneyNEXT");
  m_wallet->createFolder(walletFolderName);
  m_wallet->setFolder(walletFolderName);
}

WalletPrivate::~WalletPrivate()
{
  delete m_wallet;
}

void WalletPrivate::openWallet()
{
  if (!m_wallet || !m_wallet->isOpen()) {
    delete m_wallet;
    m_wallet = KWallet::Wallet::openWallet(KWallet::Wallet::LocalWallet(), 0);
  }
}

QString WalletPrivate::keyFromUrl(const QUrl &url) const
{
  auto urlCopy = IUrl::consistentUrlForStoring(url);

  auto key = urlCopy.toString(QUrl::RemoveQuery | QUrl::RemoveUserInfo);
  return key;
}

Wallet::Wallet() :
  d_ptr(std::make_unique<WalletPrivate>())
{
}

Wallet::~Wallet() = default;

void Wallet::writePassphrase(const QString &keyID, const QString &password)
{
  Q_D(Wallet);
  d->openWallet();

  d->m_wallet->writePassword(keyID, password);
}

QString Wallet::readPassphrase(const QString &keyID)
{
  Q_D(Wallet);
  d->openWallet();

  QString password;
  d->m_wallet->readPassword(keyID, password);
  return password;
}

void Wallet::storeCredentials(const QUrl &urlWithCredentials)
{
  Q_D(Wallet);
  d->openWallet();

  auto key = d->keyFromUrl(urlWithCredentials);

  QMap<QString, QString> credentialsMap;
  if (!urlWithCredentials.userName().isEmpty())
    credentialsMap.insert("username", urlWithCredentials.userName());

  if (!urlWithCredentials.password().isEmpty())
    credentialsMap.insert("passphrase", urlWithCredentials.password());

  if (!credentialsMap.isEmpty())
    d->m_wallet->writeMap(key, credentialsMap);
}

QUrl Wallet::restoreCredentials(const QUrl &urlWithoutCredentials)
{
  Q_D(Wallet);
  d->openWallet();

  QUrl urlWithCredentials(urlWithoutCredentials);
  auto key = d->keyFromUrl(urlWithoutCredentials);

  QMap<QString, QString> credentialsMap;
  if (d->m_wallet->readMap(key, credentialsMap) != 0)
    return urlWithCredentials;

   urlWithCredentials.setUserName(credentialsMap.value("username"));
   urlWithCredentials.setPassword(credentialsMap.value("passphrase"));

  return urlWithCredentials;
}

} // namespace Storage
