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

#ifndef STORAGE_GPG_BACKEND_PASSPHRASEPROVIDER_H
#define STORAGE_GPG_BACKEND_PASSPHRASEPROVIDER_H

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// QT Includes

#include <QtGlobal>

// ----------------------------------------------------------------------------
// Project Includes

#include <../gpgme++/interfaces/passphraseprovider.h>

class QWidget;

namespace Storage {
namespace Gpg {
namespace Widget {

class PassphraseProvider : public GpgME::PassphraseProvider
{
  Q_DISABLE_COPY(PassphraseProvider);

public:
  PassphraseProvider(QWidget *parent = nullptr, bool useWallet = false);
  ~PassphraseProvider() override final;

  char *getPassphrase(const char *useridHint, const char *description,
                      bool previousWasBad, bool &canceled) override final;

private:
  bool m_triedWithWalletPassword = false;
  bool m_triedWithUserPassword = false;
  bool m_useWallet;
  QWidget *m_parent;
};

} // namespace Widget
} // namespace Gpg
} // namespace Storage

#endif
