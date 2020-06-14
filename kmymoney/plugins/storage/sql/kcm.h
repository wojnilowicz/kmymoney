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

#ifndef STORAGE_SQL_KCM_H
#define STORAGE_SQL_KCM_H

#include <memory>

// ----------------------------------------------------------------------------
// QT Includes

// ----------------------------------------------------------------------------
// KDE Includes

#include <KCModule>

// ----------------------------------------------------------------------------
// Project Includes

namespace Ui { class KCM; };

namespace Storage {
namespace Sql {

class KCM : public KCModule
{
public:
  explicit KCM(QWidget *parent, const QVariantList &args);
  ~KCM() override final;

private:
  const std::unique_ptr<Ui::KCM> ui;
};

} // namespace Sql
} // namespace Storage
#endif

