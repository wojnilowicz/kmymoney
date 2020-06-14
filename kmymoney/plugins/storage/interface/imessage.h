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

#ifndef STORAGE_INTERFACE_IMESSAGE_H
#define STORAGE_INTERFACE_IMESSAGE_H

#include <kmm_istorage_export.h>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// QT Includes

// ----------------------------------------------------------------------------
// Project Includes

class QString;

namespace Storage {

enum class eIssue;

class KMM_ISTORAGE_EXPORT IMessage
{
public:
  virtual ~IMessage() = 0;

  virtual QString standardMessage(eIssue issue);
  virtual QString standardTitle(eIssue issue);
};

} // namespace Storage

#endif
