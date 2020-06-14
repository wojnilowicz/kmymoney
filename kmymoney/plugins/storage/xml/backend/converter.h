/*
 * Copyright 2020       Łukasz Wojniłowicz <lukasz.wojnilowicz@gmail.com>
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

#ifndef STORAGE_XML_BACKEND_CONVERTER_H
#define STORAGE_XML_BACKEND_CONVERTER_H

#include <memory>

// ----------------------------------------------------------------------------
// QT Includes

#include <QtGlobal>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "storage/interface/icallback.h"

class QByteArray;

namespace Storage {

enum class eProducer;

namespace Xml {

class ConverterPrivate;
class Converter
{
  Q_DISABLE_COPY(Converter)
  Q_DECLARE_PRIVATE(Converter)

public:
  explicit Converter(Storage::ProgressCallback callback = nullptr);
  ~Converter();

  QByteArray convertStorage(const QByteArray &rawStorage, eProducer targetStorageProducer);

private:
  const std::unique_ptr<ConverterPrivate> d_ptr;
};

} // namespace Xml
} // namespace Storage

#endif
