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

#ifndef STORAGE_XML_FRONTEND_SAVEASENCRYPTED_P_H
#define STORAGE_XML_FRONTEND_SAVEASENCRYPTED_P_H

#include <memory>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// QT Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "storage/interface/enums.h"
#include "storage/interface/issuesprocessor.h"

namespace Storage {
namespace Gpg {
class KeysSelector;
class KeyDownloader;
}
}

namespace Storage {
namespace Xml {
class Backend;

class SaveAsEncrypted;
class SaveAsEncryptedPrivate
{
  Q_DECLARE_PUBLIC(SaveAsEncrypted);

public:
  explicit SaveAsEncryptedPrivate(SaveAsEncrypted &q, Gpg::KeysSelector &keySelector);
  virtual ~SaveAsEncryptedPrivate() = 0;

  void addRecoveryKeyAsConfigured();
  void slotProcessIssues(eIssue issue, eAnswer answer, ProcessorCb success);
  void addRecoveryKey();
  bool isRecoveryKeyAdded() const;
  bool isRecoveryKeyInstalled() const;

  virtual std::shared_ptr<Gpg::KeyDownloader> keyDownloaderFactory() = 0;
  virtual std::shared_ptr<IQuestioner> questionerFactory() = 0;

  SaveAsEncrypted *q_ptr;
  Backend *m_backend = nullptr;
  eType m_storageType = eType::Unknown;
  QString m_filePath;
  QStringList m_keysList;
  QObject *m_parent = nullptr;
  bool m_dontAskAboutRecoveryKey = false;
  const std::unique_ptr<Gpg::KeysSelector> m_keySelector;
};

} // namespace Xml
} // namespace Storage

#endif
