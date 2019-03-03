/*
 * Copyright 2018-2019  Łukasz Wojniłowicz <lukasz.wojnilowicz@gmail.com>
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

#ifndef XMLFILETESTHELPER_H
#define XMLFILETESTHELPER_H

#include <memory>
#include <functional>

template <typename T>  class QVector;
class QString;

class MyMoneyStorageMgr;

namespace XMLFileTestHelper {
  std::unique_ptr<MyMoneyStorageMgr> readFile(const QString& filePath);
  void writeFile(const QString& filePath, MyMoneyStorageMgr* storage);
  void testFile(const QString& filePath, std::function<void(const MyMoneyStorageMgr * const)> testStorage);
  QString configureFile(const QString& filePath, const QVector<QVector<QString>>& replacementDatas);
}
#endif
