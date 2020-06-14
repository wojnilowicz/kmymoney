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

#ifndef STORAGE_GPG_BACKEND_H
#define STORAGE_GPG_BACKEND_H

#include <memory>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// QT Includes

#include <QtGlobal>

// ----------------------------------------------------------------------------
// Project Includes

class QString;
class QDate;
class QStringList;
class QByteArray;
class QFile;
class QUrl;

namespace GpgME {
class PassphraseProvider;
}

namespace Storage {
namespace Gpg {

class BackendPrivate;
class Backend
{
  Q_DISABLE_COPY(Backend)
  Q_DECLARE_PRIVATE(Backend)

public:
  explicit Backend();
  ~Backend();

  void setPassphraseProvider(std::unique_ptr<GpgME::PassphraseProvider> passphraseProvider);

  QStringList systemEncryptionKeys(bool pretty = false);
  QStringList systemDecryptionKeys(bool pretty = false);

  QStringList prettyRepresentation(const QStringList &keyIds);
  QStringList mainKeysFromSubKeys(const QStringList &subKeyIds);

  QString keyBySubKey(const QString &subKeyId, bool pretty = false);
  QDate keyExpiryDate(const QString &keyId);

  QString importGPGkey(const QFile &keyBlock);
  bool removeGPGKey(const QString &keyId);

  static QString recoveryKeyUrl();
  static QString recoveryKeyFingerprint();

  QStringList fileEncryptionKeys(const QString &filePath);
  std::unique_ptr<QByteArray> loadDataFromEncryptedFile(const QString &filePath);
  void saveDataToEncryptedFile(const QString &filePath, const QByteArray &data, const QStringList &keyIDs);

  bool isOpeartional() const;

private:
  const std::unique_ptr<BackendPrivate> d_ptr;
};

} // namespace Gpg
} // namespace Storage

#endif
