/*
 * Copyright 2018       Łukasz Wojniłowicz <lukasz.wojnilowicz@gmail.com>
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

#include "storagesqlnames-test.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QtTest>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "../names.cpp"

QTEST_GUILESS_MAIN(Storage::Sql::NamesTest)

namespace Storage {
namespace Sql {

void NamesTest::tableNames()
{
  for (auto i = (int)Table::Accounts; i <= (int)Table::FileInfo; ++i) {
    auto isEmpty = tableName(static_cast<Table>(i)).isEmpty();
    if (isEmpty)
      qWarning() << "Empty table name " << i;
    QVERIFY(!isEmpty);
  }
}

void NamesTest::fileInfoColumnNames()
{
  for (auto i = (int)Column::FileInfo::Version; i < (int)Column::FileInfo::FixLevel; ++i) {
    auto isEmpty = columnName(static_cast<Column::FileInfo>(i)).isEmpty();
    if (isEmpty)
      qWarning() << "Empty column name " << i;
    QVERIFY(!isEmpty);
  }
}

} // namespace Sql
} // namespace Storage
