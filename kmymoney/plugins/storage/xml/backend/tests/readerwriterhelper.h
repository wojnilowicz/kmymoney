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

#ifndef STORAGE_XML_READERWRITERTESTHELPER_H
#define STORAGE_XML_READERWRITERTESTHELPER_H

#include <memory>
#include <functional>

template <typename T>  class QVector;
class QString;

class MyMoneyFile;

namespace Storage {
namespace Xml {

namespace ReaderWriterHelper {
  void testFile(const QString& filePath, std::function<void(const MyMoneyFile * const)> testStorage);
  QString configureFile(const QString& filePath, const QVector<QVector<QString>>& replacementDatas);
}

} // namespace Xml
} // namespace Storage

#endif
