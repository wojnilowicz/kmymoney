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

#include "credentials.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QDialog>
#include <QUrl>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "ui_credentials.h"

namespace Storage {
namespace Sql {
namespace Widget {

Credentials::Credentials(const QString &requestText, QObject *parent) :
  m_ui(new Ui::Credentials),
  m_dialog(new QDialog(qobject_cast<QWidget *>(parent)))
{
  m_ui->setupUi(m_dialog.get());
  m_ui->requestLabel->setText(requestText);
  connect(m_ui->dialogButtons, &QDialogButtonBox::accepted, m_dialog.get(), &QDialog::accept);
  connect(m_ui->dialogButtons, &QDialogButtonBox::rejected, m_dialog.get(), &QDialog::reject);

  connect(m_dialog.get(), &QDialog::accepted, [=]() {emit credentials(m_ui->userNameLineEdit->text(), m_ui->passphraseLineEdit->text());} );
  connect(m_dialog.get(), &QDialog::rejected, [=]() {emit credentials(QString(), QString());} );
}

Credentials::~Credentials() = default;

void Credentials::getCredentials()
{
//   emit credentials("kmymoneynextuser", "passphrase");
 m_dialog->open();
  // kmymoneynextuser passphrase
}

void Credentials::getPassphrase()
{
  m_ui->userNameLabel->hide();
  m_ui->userNameLineEdit->hide();
  m_dialog->open();
}

} // namespace Widget
} // namespace Sql
} // namespace Storage
