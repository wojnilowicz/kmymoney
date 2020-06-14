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

#ifndef STORAGE_MANAGER_FRONTEND_MESSAGE_H
#define STORAGE_MANAGER_FRONTEND_MESSAGE_H

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// QT Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "storage/interface/imessage.h"

namespace Storage {
namespace Manager {

class Message : public IMessage
{
public:
  using IMessage::IMessage;

  QString standardMessage(eIssue issue) override;
  QString standardTitle(eIssue issue) override;
};

} // namespace Manager
} // namespace Storage

#endif
