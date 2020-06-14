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

#include "saveasencrypted.h"
#include "saveasencrypted_p.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QUrl>
#include <QStandardPaths>
#include <QDebug>
#include <QFileInfo>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "mymoney/mymoneyexception.h"

#include "storage/interface/issuesprocessor.h"
#include "storage/interface/ivalidator.h"
#include "storage/interface/enums.h"
#include "backend/url.h"
#include "backend/backend.h"

#include "gpg/frontend/keysselector.h"
#include "gpg/frontend/keydownloader.h"
#include "gpg/backend/backend.h"

#include "configuration.h"

namespace Storage {
namespace Xml {

SaveAsEncryptedPrivate::SaveAsEncryptedPrivate(SaveAsEncrypted &q, Gpg::KeysSelector &keySelector) :
  q_ptr(&q),
  m_keySelector(&keySelector)
{
}

SaveAsEncryptedPrivate::~SaveAsEncryptedPrivate() = default;

void SaveAsEncryptedPrivate::addRecoveryKeyAsConfigured()
{
  if (!m_dontAskAboutRecoveryKey && Configuration::encryptWithRecoveryKeyByDefault() && !isRecoveryKeyAdded()) {
    m_dontAskAboutRecoveryKey = true;

    if (!isRecoveryKeyInstalled()) {
// KHtml crashes trying to download on macOS
#if defined(Q_OS_MACOS)
      return;
#endif
      const QVector<Issue> issues = {{eIssue::MissingRecoveryKey, QStringList{m_backend->gpg()->recoveryKeyUrl()}}};

      const auto finishHandler = [&](bool success) mutable {
        if (success)
          addRecoveryKey();
      };

      auto questioner = questionerFactory();
      auto frontendHandler = [=] () {
        return questioner.get();
      };

      auto backendHandler = [&] (eIssue issue, eAnswer answer, ProcessorCb processorCb) {
        slotProcessIssues(issue, answer, processorCb);
      };

      IssuesProcessor::createIssuesProcessor(issues, frontendHandler, backendHandler, finishHandler);
    } else {
      addRecoveryKey();
    }
  }
}

void SaveAsEncryptedPrivate::slotProcessIssues(eIssue issue, eAnswer answer, ProcessorCb success)
{
  Q_Q(SaveAsEncrypted);

  switch (issue) {
    case eIssue::MissingRecoveryKey:
      if (answer == eAnswer::Yes) {
        auto keyDownloader = keyDownloaderFactory();
        auto connection = std::make_shared<QMetaObject::Connection>();
        auto slotKeyDownloaderFinished = [=](bool downloadSuccess) mutable {
          q->disconnect(*connection);
          keyDownloader.reset();
          success(downloadSuccess);
        };
        keyDownloader->downloadRecoveryKey();
        *connection = q->connect(keyDownloader.get(), &Gpg::KeyDownloader::result, slotKeyDownloaderFinished);
      } else {
        success(false);
      } return;

    default:
      throw MYMONEYEXCEPTION_CSTRING("Unknown issue encountered");
  }
}

void SaveAsEncryptedPrivate::addRecoveryKey()
{
  const auto systemEncryptionKeys = m_backend->gpg()->systemEncryptionKeys();
  const auto recoveryKeyId = m_backend->gpg()->recoveryKeyFingerprint().right(16);
  if (systemEncryptionKeys.contains(recoveryKeyId)) {
    m_keysList.append(recoveryKeyId);
    m_keySelector->setKeys(m_keysList);
  }
}

bool SaveAsEncryptedPrivate::isRecoveryKeyAdded() const
{
  if (!m_keysList.count())
    return false;

  if (!m_keysList.contains(m_backend->gpg()->recoveryKeyFingerprint().right(16)))
    return false;

  return true;
}

bool SaveAsEncryptedPrivate::isRecoveryKeyInstalled() const
{
  const auto systemEncryptionKeys = m_backend->gpg()->systemEncryptionKeys();
  if (!systemEncryptionKeys.contains(m_backend->gpg()->recoveryKeyFingerprint().right(16)))
    return false;
  return true;
}

SaveAsEncrypted::SaveAsEncrypted(SaveAsEncryptedPrivate &d, Backend *backend, eType storageType) :
  d_ptr(&d)
{
  d.m_backend = backend;
  d.m_storageType = storageType;
  connect(d.m_keySelector.get(), &Gpg::KeysSelector::keysListChanged,
          this, &SaveAsEncrypted::setKeysList);
}

SaveAsEncrypted::~SaveAsEncrypted() = default;

QUrl SaveAsEncrypted::storageUrl() const
{
  Q_D(const SaveAsEncrypted);

  QUrl url;
  Url storageUrl(url);
  storageUrl.setPluginScheme();
  storageUrl.encodeStorageType(d->m_storageType);
  storageUrl.setPath(d->m_filePath);
  storageUrl.setEncryptionKeys(d->m_keysList);

  if (d->m_backend->isKMyMoneyType(d->m_storageType))
    storageUrl.setNoSwitch();

  return std::move(storageUrl);
}

void SaveAsEncrypted::setStorageUrl(const QUrl &storageUrl)
{
  Q_D(SaveAsEncrypted);
  Url enhancedUrl(storageUrl);
  auto keys = enhancedUrl.encryptionKeys();
  auto keysPretty = d->m_backend->gpg()->prettyRepresentation(keys);
  d->m_keySelector->setKeys(keysPretty);
  if (!keysPretty.isEmpty())
    d->m_dontAskAboutRecoveryKey = true;
  setKeysList(keys);
  setFilePath(storageUrl.path());

  validateUserSelections();
}

void SaveAsEncrypted::validateUserSelections()
{
  Q_D(SaveAsEncrypted);
  const auto fileInfo = QFileInfo(d->m_filePath);
  const auto isValid = fileInfo.suffix() == "kmy" && !d->m_keysList.isEmpty();
  emit userBasedValidityChanged(isValid);
}

void SaveAsEncrypted::setFilePath(const QString &filePath)
{
  Q_D(SaveAsEncrypted);
  d->addRecoveryKeyAsConfigured();
  d->m_filePath = filePath;
  validateUserSelections();
}

void SaveAsEncrypted::setKeysList(const QStringList &keysList)
{
  Q_D(SaveAsEncrypted);
  d->m_keysList = keysList;
  validateUserSelections();
}

} // namespace Xml
} // namespace Storage
