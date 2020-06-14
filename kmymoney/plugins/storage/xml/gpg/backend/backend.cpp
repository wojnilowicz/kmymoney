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

#include "backend.h"

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// QT Includes

#include <QDebug>
#include <QStandardPaths>
#include <QFileInfo>
#include <QDir>
#include <QDate>
#include <QRegularExpression>

// ----------------------------------------------------------------------------
// Project Includes

#include <../gpgme++/context.h>
#include <../gpgme++/key.h>
#include <../gpgme++/data.h>
#include <../gpgme++/decryptionresult.h>
#include <../gpgme++/encryptionresult.h>
#include <../gpgme++/importresult.h>
#include <../gpgme++/engineinfo.h>
#include <../gpgme++/interfaces/passphraseprovider.h>

#include "mymoney/mymoneyexception.h"

namespace Storage {
namespace Gpg {

class BackendPrivate
{
  Q_DISABLE_COPY(BackendPrivate)

public:
  BackendPrivate();
  ~BackendPrivate() = default;

  QString GPGHomeDirectory() const;
  std::vector<GpgME::Key> encryptionKeyList(const QStringList &pattern, bool secretOnly);

  QStringList keyIds(const std::vector<GpgME::Key> &keys) const;
  QStringList prettyKeyIds(const std::vector<GpgME::Key> &keys) const;

  std::unique_ptr<GpgME::Context> m_context;
  std::unique_ptr<GpgME::PassphraseProvider> m_passphraseProvider;

  bool m_isOperational = true;
};


BackendPrivate::BackendPrivate()
{
  const auto GPGHomeDirectory = BackendPrivate::GPGHomeDirectory();
  if (GPGHomeDirectory.isEmpty()) {
    qDebug() << "Failed to find home directory";
    m_isOperational = false;
    return;
  }

  GpgME::initializeLibrary();
  m_context = GpgME::Context::create(GpgME::OpenPGP);
  if (!m_context) {
    qDebug() << "Failed to create context";
    m_isOperational = false;
    return;
  }
  
  auto gpgError = m_context->setEngineHomeDirectory(GPGHomeDirectory.toUtf8().constData());
  if (gpgError.code() != GPG_ERR_NO_ERROR) {
    m_isOperational = false;
    qDebug() << QString::fromLatin1("Cannot set home directory: %1").arg(gpgError.asString());
  }
}

QString BackendPrivate::GPGHomeDirectory() const
{
  const QStringList GPGHomeDirectoryNames {
    QStringLiteral(".gnupg"),
    QStringLiteral("gnupg")
  };

  const QStringList GPGKeyringNames {
    QStringLiteral("secring.gpg"),
    QStringLiteral("pubring.gpg"),
    QStringLiteral("pubring.kbx")
  };

  const QVector<QStandardPaths::StandardLocation> standardLocations {
     QStandardPaths::HomeLocation,
     QStandardPaths::GenericDataLocation,
     QStandardPaths::AppDataLocation
   };
   
  for (const auto &standardLocation : standardLocations) {
    auto standardPaths = QStandardPaths::standardLocations(standardLocation);
    for (const auto &standardPath : standardPaths) {
      QDir standarPathAsDir(standardPath);
      if (standarPathAsDir.dirName() == "kmymoney")
        standarPathAsDir.cdUp();
      for (const auto &GPGHomeDirectoryName : GPGHomeDirectoryNames) {
        QDir standardGpgPath(QString::fromLatin1("%1/%2").arg(standarPathAsDir.path(),
                                                              GPGHomeDirectoryName));        
        if (!standardGpgPath.exists())
          continue;
        
        for (const auto &GPGKeyringName : GPGKeyringNames) {
          if (!standardGpgPath.exists(GPGKeyringName))
            continue;
          return standardGpgPath.toNativeSeparators(standardGpgPath.path());
          }
        }
      }
    }
  
  return QString();
}

std::vector<GpgME::Key> BackendPrivate::encryptionKeyList(const QStringList &patterns, bool secretOnly)
{
  std::vector<GpgME::Key> keyList;
  for (const auto &pattern : patterns) {
    auto gpgError = m_context->startKeyListing(pattern.toUtf8().constData(), secretOnly);
    while (gpgError.code() == GPG_ERR_NO_ERROR) {
      const auto key = m_context->nextKey(gpgError);
      if (gpgError.code() != GPG_ERR_NO_ERROR)
        break;
      if (!key.isBad() && key.canEncrypt())
        keyList.push_back(key);
    }

    if (gpgError.code() != GPG_ERR_EOF)
      qDebug() << QString::fromLatin1("cannot list keys: %1").arg(gpgError.asString());
  }
  return keyList;
}

QStringList BackendPrivate::keyIds(const std::vector<GpgME::Key> &keys) const
{
  QStringList keyIDs;
  for (const auto &key : keys)
    keyIDs.append(key.keyID());
  return keyIDs;
}

QStringList BackendPrivate::prettyKeyIds(const std::vector<GpgME::Key> &keys) const
{
  QStringList prettyIDs;
  for (const auto &key : keys)
    prettyIDs.append(QString::fromLatin1("%1: %2").arg(key.keyID(), key.userID(0).id()));
  return prettyIDs;
}

Backend::Backend() :
  d_ptr(new BackendPrivate)
{
}

Backend::~Backend() = default;

void Backend::setPassphraseProvider(std::unique_ptr<GpgME::PassphraseProvider> passphraseProvider)
{
  Q_D(Backend);
  if (passphraseProvider) {
    d->m_passphraseProvider = std::move(passphraseProvider);
    d->m_context->setPassphraseProvider(d->m_passphraseProvider.get());
    d->m_context->setPinentryMode(GpgME::Context::PinentryLoopback);
  } else {
    d->m_passphraseProvider.reset();
    d->m_context->setPinentryMode(GpgME::Context::PinentryDefault);
  }
}

QStringList Backend::systemEncryptionKeys(bool pretty)
{
  Q_D(Backend);
  const auto publicKeys = d->encryptionKeyList(QStringList {QString()}, false);

  if (pretty)
    return d->prettyKeyIds(publicKeys);
  return d->keyIds(publicKeys);
}

QStringList Backend::systemDecryptionKeys(bool pretty)
{
  Q_D(Backend);
  const auto privateKeys = d->encryptionKeyList(QStringList {QString()}, true);

  if (pretty)
    return d->prettyKeyIds(privateKeys);
  return d->keyIds(privateKeys);
}

QStringList Backend::prettyRepresentation(const QStringList &keyIds)
{
  Q_D(Backend);
  const auto keys = d->encryptionKeyList(QStringList {QString()}, false);

  auto findPrettyRepresentation = [&](const QString &keyId) {
    for (const auto &key : keys)
      if (key.keyID() == keyId)
        return QString::fromLatin1("%1: %2").arg(key.keyID(), key.userID(0).id());
    return keyId;
  };

  QStringList prettyList;
  for (const auto &keyId : keyIds)
    prettyList.append(findPrettyRepresentation(keyId));

  return prettyList;
}

QStringList Backend::mainKeysFromSubKeys(const QStringList &subKeyIds)
{
  Q_D(Backend);
  const auto keys = d->encryptionKeyList(QStringList {QString()}, false);

  auto findMainKey = [&](const QString &subKeyId) {
    auto searchedKeys = d->encryptionKeyList(QStringList {subKeyId}, false);
    if (!searchedKeys.size())
      return subKeyId;
    return QString::fromLatin1(searchedKeys.at(0).keyID());
  };

  QStringList mainKeysList;
  for (const auto &subKeyId : subKeyIds)
    mainKeysList.append(findMainKey(subKeyId));

  return mainKeysList;
}

QString Backend::keyBySubKey(const QString &subKeyId, bool pretty)
{
  Q_D(Backend);
  auto searchedKeys = d->encryptionKeyList(QStringList {subKeyId}, false);
  if (!searchedKeys.size())
    return QString();

  if (pretty)
    return d->prettyKeyIds(searchedKeys).first();
  return d->keyIds(searchedKeys).first();
}

QDate Backend::keyExpiryDate(const QString &keyId)
{
  Q_D(Backend);
  auto searchedKeys = d->encryptionKeyList(QStringList {keyId}, false);
  if (searchedKeys.size() && searchedKeys.front().subkeys().size())
    return QDateTime::fromTime_t(searchedKeys.front().subkey(0).expirationTime()).date();

  return QDate();
}

QString Backend::importGPGkey(const QFile &keyFile)
{
  Q_D(Backend);

  GpgME::Data keyDataBuffer(keyFile.handle());
  const auto importResult = d->m_context->importKeys(keyDataBuffer);
  if (!importResult.numImported())
    return QString();

  const auto import = importResult.import(0);
  return import.fingerprint();
}

bool Backend::removeGPGKey(const QString &keyId)
{
  Q_D(Backend);
  auto searchedKeys = d->encryptionKeyList(QStringList {keyId}, false);
  if (!searchedKeys.size())
    return false;
  d->m_context->deleteKey(searchedKeys.front(), true);
  return true;
}

QString Backend::recoveryKeyUrl()
{
  return QStringLiteral("https://kmymoney.org/recovery.html");
}

QString Backend::recoveryKeyFingerprint()
{
  return QStringLiteral("2E1B19BBDE4A9AD3B51BC41859B0F826D2B08440");
}

QStringList Backend::fileEncryptionKeys(const QString &filePath)
{
  Q_D(Backend);

  QStringList encryptionKeys;

  QFile encryptedFile(filePath);
  if (!encryptedFile.open(QIODevice::ReadOnly))
    return encryptionKeys;

  GpgME::Data decryptedData;
  const GpgME::Data encryptedData(encryptedFile.handle());
  const auto stashedPinentryMode = d->m_context->pinentryMode();
  d->m_context->setPinentryMode(GpgME::Context::PinentryCancel);
  const auto decryptionResults = d->m_context->decrypt(encryptedData, decryptedData);
  d->m_context->setPinentryMode(stashedPinentryMode);
  for (const auto &recipient : decryptionResults.recipients())
    encryptionKeys.append(recipient.keyID());

  return encryptionKeys;
}

std::unique_ptr<QByteArray> Backend::loadDataFromEncryptedFile(const QString &filePath)
{
  Q_D(Backend);

  auto loadedFileData = std::make_unique<QByteArray>();
  if (!QFile::exists(filePath))
    return loadedFileData;

  QFile encryptedFile(filePath);
  if (!encryptedFile.open(QIODevice::ReadOnly))
    return loadedFileData;

  const GpgME::Data encryptedData(encryptedFile.handle());
  while (true) {
    GpgME::Data decryptedData; // in the loop, because otherwise data gets corrupted after failed decryption try
    encryptedFile.seek(0);
    const auto decryptionResults = d->m_context->decrypt(encryptedData, decryptedData);
    const auto errorCode = decryptionResults.error().code();
    if (errorCode == GPG_ERR_BAD_PASSPHRASE)
      continue;

    const auto dataLength = decryptedData.seek(0, SEEK_CUR);
    loadedFileData->resize(dataLength);
    decryptedData.seek(0, SEEK_SET);
    decryptedData.read(loadedFileData->data(), dataLength);

    break;
  }

  return loadedFileData;
}

void Backend::saveDataToEncryptedFile(const QString &filePath, const QByteArray &data, const QStringList &keyIDs)
{
  Q_D(Backend);

  if (QFile::exists(filePath) && !(QFile::permissions(filePath) & QFileDevice::WriteUser))
    throw MYMONEYEXCEPTION_CSTRING("The file is not writeable.");

  if (keyIDs.isEmpty())
    throw MYMONEYEXCEPTION_CSTRING("No keys to encrypt the file.");

  auto encryptionKeys = d->encryptionKeyList(keyIDs, false);
  QFile encryptedFile(filePath);
  if (!encryptedFile.open(QIODevice::WriteOnly))
    return;

  GpgME::Data decryptedData(data.constData(), data.size(), false);
  GpgME::Data encryptedData(encryptedFile.handle());
  auto encryptionResults = d->m_context->encrypt(encryptionKeys, decryptedData, encryptedData, GpgME::Context::AlwaysTrust);
  if (encryptionResults.error().code() != GPG_ERR_NO_ERROR)
    throw MYMONEYEXCEPTION(QString::fromLatin1("encryption error: %1").arg(encryptionResults.error().asString()));
}

bool Backend::isOpeartional() const
{
  Q_D(const Backend);
  return d->m_isOperational;
}

} // namespace Gpg
} // namespace Storage
