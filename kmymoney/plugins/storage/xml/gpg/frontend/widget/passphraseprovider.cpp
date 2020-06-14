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

#include "passphraseprovider.h"

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// QT Includes

#include <QDialog>
#include <QDebug>

// ----------------------------------------------------------------------------
// Project Includes

#include <ui_passphraseprovider.h>

#include "storage/interface/wallet.h"

namespace Storage {
namespace Gpg {
namespace Widget {

PassphraseProvider::PassphraseProvider(QWidget *parent, bool useWallet) :
  m_useWallet(useWallet),
  m_parent(parent)
{
}
PassphraseProvider::~PassphraseProvider() = default;

char *PassphraseProvider::getPassphrase(const char *useridHint, const char *description,
                    bool previousWasBad, bool &canceled)
{
  Q_UNUSED(previousWasBad);
  Q_UNUSED(description);

  QString passwordString;

  std::unique_ptr<Wallet> wallet = nullptr;
  const auto useridHintString = QString(useridHint);
  const auto keyId = useridHintString.split(' ').first();
  if(m_useWallet) {
    wallet = std::make_unique<Wallet>();
    passwordString = wallet->readPassphrase(keyId);
  }

  if (passwordString.isEmpty() || m_triedWithWalletPassword || m_triedWithUserPassword) {
    auto ui = std::make_unique<Ui::PassphraseProvider>();
    auto dialog = std::make_unique<QDialog>(m_parent);
    ui->setupUi(dialog.get());


    QString requestText;
    if (m_triedWithUserPassword)
      requestText = i18n("Entered password is wrong for\n%1.\nPlease try again.").arg(useridHint);
    else if (m_triedWithWalletPassword)
      requestText = i18n("Password from KWallet is wrong for\n%1.\nPlease enter the correct one.").arg(useridHint);
    else
      requestText = i18n("Please enter password for\n%1.").arg(useridHint);
    ui->requestLabel->setText(requestText);


    if (QDialog::Rejected == dialog->exec())
      canceled = true;
    else
      passwordString = ui->lePassphrase->text();

    m_triedWithUserPassword = true;
  } else {
    m_triedWithWalletPassword = true;
  }

  char *passwordCstring = new char[passwordString.length() + 1];
  strcpy(passwordCstring, passwordString.toUtf8().constData());

  if (m_triedWithUserPassword && m_useWallet)
    wallet->writePassphrase(keyId, passwordString);
  return passwordCstring;
}

} // namespace Widget
} // namespace Gpg
} // namespace Storage
