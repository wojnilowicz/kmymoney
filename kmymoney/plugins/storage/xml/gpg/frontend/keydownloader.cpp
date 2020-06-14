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

#include "keydownloader.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QUrl>
#include <QDebug>
#include <QRegularExpression>
#include <QTemporaryFile>

// ----------------------------------------------------------------------------
// KDE Includes

#include <KHTMLPart>

// ----------------------------------------------------------------------------
// Project Includes

#include "storage/interface/enums.h"
#include "storage/interface/ivalidator.h"
#include "storage/interface/issuesprocessor.h"

#include "gpg/backend/backend.h"
#include "gpg/frontend/questioner.h"

namespace Storage {
namespace Gpg {

class KeyDownloaderPrivate
{
  Q_DISABLE_COPY(KeyDownloaderPrivate)
  Q_DECLARE_PUBLIC(KeyDownloader)

public:
  KeyDownloaderPrivate(KeyDownloader *qq, std::unique_ptr<IQuestioner> questioner);

  void processIssues(const QVector<Issue> &issues);
  void slotProcessIssues(eIssue issue, eAnswer answer, ProcessorCb success);
  QString extractRecoveryKey(const QString &htmlPage);
  void installRecoveryKey(const QString &key);
  void downloadByKHtml();

  KeyDownloader * const q_ptr;
  QString m_badFingerPrint;
  const std::unique_ptr<IQuestioner> m_questioner;
  const std::unique_ptr<Backend> m_backend;
};

KeyDownloaderPrivate::KeyDownloaderPrivate(KeyDownloader *q, std::unique_ptr<IQuestioner> questioner) :
  q_ptr(q),
  m_questioner(std::move(questioner)),
  m_backend(std::make_unique<Backend>())
{
}

void KeyDownloaderPrivate::processIssues(const QVector<Issue> &issues)
{
  Q_Q(KeyDownloader);
  auto frontendHandler = [=] () {
    return m_questioner.get();
  };

  auto backendHandler = [=] (eIssue issue, eAnswer answer, ProcessorCb success) {
    slotProcessIssues(issue, answer, success);
  };

  auto onSuccess = [=](bool success) {
    emit q->result(success);
  };

  IssuesProcessor::createIssuesProcessor(issues, frontendHandler, backendHandler, onSuccess);
}

void KeyDownloaderPrivate::slotProcessIssues(eIssue issue, eAnswer answer, ProcessorCb success)
{
  Q_Q(KeyDownloader);
  switch (issue) {
    case eIssue::RecoveryKeyCorrupt:
      switch (answer) {
        case eAnswer::Yes: {
          m_backend->removeGPGKey(m_badFingerPrint);
          success(true);
        } return;
        default:
          break;
      }; break;

    case eIssue::RecoveryKeyInstalled:
      success(true);
      return;

    case eIssue::RecoveryKeyDownloadFailed:
      switch (answer) {
        case eAnswer::Yes:
          q->downloadRecoveryKey();
          return;
        default:
          break;
      };

    default:
      break;
  }

  success(false);
}

QString KeyDownloaderPrivate::extractRecoveryKey(const QString &htmlPage)
{
  QString extractedKey;
  const QRegularExpression re(QStringLiteral("-----BEGIN PGP PUBLIC KEY BLOCK-----.*-----END PGP PUBLIC KEY BLOCK-----"),
                        QRegularExpression::DotMatchesEverythingOption);
  const auto match = re.match(htmlPage);
  if (match.hasMatch())
    extractedKey = match.captured(0);
  return extractedKey;
}

void KeyDownloaderPrivate::installRecoveryKey(const QString &key)
{
  QTemporaryFile file;
  file.open();
  file.write(key.toUtf8());
  file.seek(0);
  const auto fingerprint = m_backend->importGPGkey(file);
  QVector<Issue> issues;
  if (fingerprint != m_backend->recoveryKeyFingerprint()) {
    issues.append({eIssue::RecoveryKeyCorrupt, QStringList{fingerprint, m_backend->recoveryKeyFingerprint()}});
    m_badFingerPrint = fingerprint;
  } else {
    issues.append({eIssue::RecoveryKeyInstalled, QStringList()});
  }

  processIssues(issues);
}

void KeyDownloaderPrivate::downloadByKHtml()
{
  Q_Q(KeyDownloader);
  auto m_khtmlPart = std::make_shared<KHTMLPart>();

  auto connection = std::make_shared<QMetaObject::Connection>();
  auto slotReadOnlyPartCompleted = [=]() mutable {
    if (m_khtmlPart->inProgress())
      return;

    q->disconnect(*connection);

    const auto htmlSource = m_khtmlPart->documentSource();
    m_khtmlPart.reset();
    const auto &extractedKey = extractRecoveryKey(htmlSource);
    if (!extractedKey.isEmpty()) {
      installRecoveryKey(extractedKey);
    } else {
      QVector<Issue> issues;
      issues.append({eIssue::RecoveryKeyDownloadFailed, QStringList()});
      processIssues(issues);
    }
  };

  *connection = q->connect(m_khtmlPart.get(), qOverload<>(&KParts::ReadOnlyPart::completed),
          slotReadOnlyPartCompleted);
  m_khtmlPart->openUrl(QUrl(m_backend->recoveryKeyUrl()));
}


KeyDownloader::KeyDownloader(std::unique_ptr<IQuestioner> questioner) :
  d_ptr(std::make_unique<KeyDownloaderPrivate>(this, std::move(questioner)))
{
}

KeyDownloader::~KeyDownloader()  = default;

void KeyDownloader::downloadRecoveryKey()
{
  Q_D(KeyDownloader);
  d->downloadByKHtml();
}

} // namespace Gpg
} // namespace Storage
