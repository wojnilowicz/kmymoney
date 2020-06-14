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

#include "kcm.h"

// ----------------------------------------------------------------------------
// QT Includes

// ----------------------------------------------------------------------------
// KDE Includes

#include <KPluginFactory>

// ----------------------------------------------------------------------------
// Project Includes

#include "configuration.h"
#include "ui_kcm.h"

namespace Storage {
namespace Xml {

KCM::KCM(QWidget *parent, const QVariantList& args) :
  KCModule(parent, args),
  ui(std::make_unique<Ui::KCM>())
{
  ui->setupUi(this);
  addConfig(Configuration::self(), this);

// disable features that depend on DBus and doesn't work yet on other platforms
#if defined(Q_OS_MACOS)
  ui->kcfg_useKWalletToManagePasswords->setEnabled(false);
  ui->kcfg_useKWalletToManagePasswords->setChecked(false);
  ui->kcfg_useKWalletToManagePasswords->setToolTip(i18n("Not available on this platform"));
#endif
}

KCM::~KCM() = default;

K_PLUGIN_FACTORY_WITH_JSON(KCMFactory, "kcm.json", registerPlugin<KCM>();)

} // namespace Xml
} // namespace Storage

#include "kcm.moc"
