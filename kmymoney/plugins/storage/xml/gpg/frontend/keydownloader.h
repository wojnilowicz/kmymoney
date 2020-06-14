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

#ifndef STORAGE_GPG_FRONTEND_KEYDOWNLOADER_H
#define STORAGE_GPG_FRONTEND_KEYDOWNLOADER_H

#include <memory>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// QT Includes

#include <QObject>

// ----------------------------------------------------------------------------
// Project Includes

namespace Storage {
class IQuestioner;

namespace Gpg {

class KeyDownloaderPrivate;
class KeyDownloader : public QObject
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(KeyDownloader)

public:
  KeyDownloader(std::unique_ptr<IQuestioner> questioner);
  virtual ~KeyDownloader() = 0;

  void downloadRecoveryKey();

Q_SIGNALS:
  void result(bool success);

private:
  const std::unique_ptr<KeyDownloaderPrivate> d_ptr;
};

} // namespace Gpg
} // namespace Storage

#endif
