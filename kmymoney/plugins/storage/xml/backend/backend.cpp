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

#include "cmakedefine.h"
#include "backend.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QVector>
#include <QUrl>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>
#include <QBuffer>
#include <QDate>

// ----------------------------------------------------------------------------
// KDE Includes

#include <KLocalizedString>
#include <KCompressionDevice>

// ----------------------------------------------------------------------------
// Project Includes

#include "mymoney/mymoneyfile.h"
#include "mymoney/mymoneyexception.h"
#include "mymoney/mymoneyfile.h"

#include "storage/interface/enums.h"
#include "storage/interface/ivalidator.h"

#include "readerwriter.h"
#include "upgrader.h"
#include "converter.h"
#include "anonymizer.h"
#include "url.h"


#ifdef ENABLE_GPG
#include "configuration.h"
#include "gpg/backend/backend.h"
#endif

static constexpr auto compressionType = KCompressionDevice::GZip;
static constexpr auto bytesToInferType = 5;
static constexpr auto bytesToInferVersion = 230;
static constexpr auto bytesToInferDocType = 70;

namespace Storage {
namespace Xml {

enum class eFileType {
  Unknown,
  GZIP,
  GPG,
  XML
};

const QVector<eType> kmmnTypes {
      eType::kmmn_XmlGzip,
      eType::kmmn_XmlGpg,
      eType::kmmn_XmlPlain,
      eType::kmmn_XmlAnonymous
};

const QVector<eType> kmmTypes {
      eType::kmm_XmlGzip,
      eType::kmm_XmlGpg,
      eType::kmm_XmlPlain,
      eType::kmm_XmlAnonymous
};

const QMap<eType, eType> kmmToKmmnTypeMap {
  {eType::kmm_XmlAnonymous, eType::kmmn_XmlAnonymous},
  {eType::kmm_XmlPlain,     eType::kmmn_XmlPlain},
  {eType::kmm_XmlGzip,      eType::kmmn_XmlGzip},
  {eType::kmm_XmlGpg,       eType::kmmn_XmlGpg}
};

class BackendPrivate
{  
public:
  BackendPrivate();
  ~BackendPrivate();

  eProducer docType(const QUrl &url);
  eFileType fileTypeByMagicHeader(const QUrl &url);

  QVector<eType> typesFiltered(const QVector<eType> &excludedTypes = QVector<eType>{}) const;

  const QByteArray &readAll(const QUrl &url);

  uint storageVersionKMM(const QUrl &url);
  uint storageVersionKMMN(const QUrl &url);
  uint storageVersion(const QUrl &url);
  eVersionStatus versionStatus(const QUrl &url);

  void resetCache();

#ifdef ENABLE_GPG
  auto checkOnlyEncryptionKeys(const QUrl &url);
  auto checkExpiringKeys(const QUrl &url);
  auto checkMissingKeys(const QUrl &url);
  auto checkMissingDecryptionKeys(const QUrl &url);
  auto checkMissingEncryptionKeys(const QUrl &url);
  auto checkMissingRecoveryKey(const QUrl &url);
  const std::unique_ptr<Gpg::Backend> m_gpg;
#endif

  QByteArray  m_cachedRawStorage;
  QUrl        m_cachedUrl;
  eProducer   m_cachedProducer = eProducer::Unknown;
  eType       m_cachedType = eType::Unknown;
  ProgressCallback m_callback = nullptr;
};

BackendPrivate::BackendPrivate()
#ifdef ENABLE_GPG
:  m_gpg (std::make_unique<Gpg::Backend>())
#endif
{
}

BackendPrivate::~BackendPrivate() = default;

eProducer BackendPrivate::docType(const QUrl &url)
{
  if (url == m_cachedUrl && m_cachedProducer != eProducer::Unknown)
    return m_cachedProducer;

  const auto &fileContent = readAll(url);
  const auto fileHead = fileContent.left(bytesToInferDocType);

  const QMap<const char *, eProducer> docTypes {
    {"<!DOCTYPE KMYMONEY-FILE>",     eProducer::KMyMoney},
    {"<!DOCTYPE KMYMONEYNEXT-FILE>", eProducer::KMyMoneyNEXT}
  };

  m_cachedProducer = eProducer::Unknown;
  for (auto it = docTypes.cbegin(); it != docTypes.cend(); ++it) {
    if (fileHead.contains(it.key())) {
      m_cachedProducer = it.value();
      break;
    }
  }

  return m_cachedProducer;
}

eFileType BackendPrivate::fileTypeByMagicHeader(const QUrl &url)
{
  const auto localFileName = url.path();
  QFile fileHandle(localFileName);
  fileHandle.open(QIODevice::ReadOnly);

  const auto fileHead = fileHandle.read(bytesToInferType);

  static const QMap<const char *, eFileType> fileTypes {
    {"\037\213",  eFileType::GZIP},// gzipped?
    {"--",        eFileType::GPG}, // PGP ASCII armored?
    {"\205\001",  eFileType::GPG}, // PGP binary?
    {"\205\002",  eFileType::GPG}, // PGP binary?
    {"<?xml",     eFileType::XML}
  };

  for (auto it = fileTypes.cbegin(); it != fileTypes.cend(); ++it)
    if (fileHead.startsWith(it.key()))
      return it.value();
  return eFileType::Unknown;
}

QVector<eType> BackendPrivate::typesFiltered(const QVector<eType> &excludedTypes) const
{
  QVector<eType> types;
  types.append(kmmTypes);
  types.append(kmmnTypes);

  for (auto i = types.count() - 1; i > 0; --i) {
    const auto storageType = types.at(i);
    if (excludedTypes.contains(storageType))
      types.removeAt(i);
  }

  return types;
}

const QByteArray &BackendPrivate::readAll(const QUrl &url)
{
  if (url == m_cachedUrl && !m_cachedRawStorage.isEmpty())
    return m_cachedRawStorage;

  resetCache();

  const auto fileType = fileTypeByMagicHeader(url);
  const auto localFileName = url.path();

  switch (fileType) {
#ifdef ENABLE_GPG
    case eFileType::GPG: {
      auto rawStorage = m_gpg->loadDataFromEncryptedFile(localFileName).release();
      m_cachedRawStorage = std::move(*rawStorage);
    } break;
#endif
    case eFileType::GZIP:
    case eFileType::XML: {
      std::unique_ptr<QIODevice> fileHandle = nullptr;
      switch (fileType) {
        case eFileType::GZIP:
          fileHandle = std::make_unique<KCompressionDevice>(localFileName, compressionType);
          break;
        case eFileType::XML:
          fileHandle = std::make_unique<QFile>(localFileName);
          break;
        default:
          break;
      }
      if (!fileHandle->open(QIODevice::ReadOnly))
        throw MYMONEYEXCEPTION_CSTRING("Failed to open storage.");
      m_cachedRawStorage = fileHandle->readAll();
    } break;
    default:
      throw MYMONEYEXCEPTION_CSTRING("Storage is neither gziped, encrypted nor plain.");
  }

  m_cachedUrl = url;

  return m_cachedRawStorage;
}

uint BackendPrivate::storageVersionKMM(const QUrl &url)
{
  const auto fileHead = readAll(url).left(bytesToInferVersion);

  QString fileVersion;
  QString fileFixVersion;

  QRegularExpression re;
  QRegularExpressionMatch match;

  re.setPattern(QStringLiteral("<VERSION id=\"(?<version>\\d+)\""));
  match = re.match(fileHead);
  if (match.hasMatch())
    fileVersion = match.captured(1);

  re.setPattern(QStringLiteral("<FIXVERSION id=\"(?<version>\\d+)\""));
  match = re.match(fileHead);
  if (match.hasMatch())
    fileFixVersion = match.captured(1);

  if (fileVersion.isEmpty() or fileFixVersion.isEmpty())
    throw MYMONEYEXCEPTION_CSTRING("Failed to parse KMyMoney storage version.");

  return fileVersion.toUInt() * 1000 + fileFixVersion.toUInt();
}

uint BackendPrivate::storageVersionKMMN(const QUrl &url)
{
  const auto fileHead = readAll(url).left(bytesToInferVersion);

  QString fileVersion;

  QRegularExpression re;
  QRegularExpressionMatch match;

  re.setPattern(QStringLiteral("<VERSION id=\"(?<version>\\d+)\""));
  match = re.match(fileHead);
  if (match.hasMatch())
    fileVersion = match.captured(1);

  if (fileVersion.isEmpty())
    throw MYMONEYEXCEPTION_CSTRING("Failed to parse KMyMoneyNEXT storage version.");
  return fileVersion.toUInt();
}

uint BackendPrivate::storageVersion(const QUrl &url)
{
  switch (docType(url)) {
    case eProducer::KMyMoney:
      return storageVersionKMM(url);
    case eProducer::KMyMoneyNEXT:
      return storageVersionKMMN(url);
    default:
      throw MYMONEYEXCEPTION_CSTRING("Failed to parse storage version.");
  }
}

eVersionStatus BackendPrivate::versionStatus(const QUrl &url)
{
  const auto storageProducer = docType(url);
  const auto supportedVersions = Upgrader::supportedVersions(storageProducer);

  if (supportedVersions.isEmpty())
    return eVersionStatus::Unknown;

  const auto highestSupportedVersion = *std::max_element(supportedVersions.cbegin(), supportedVersions.cend());
  const auto storageVersion = BackendPrivate::storageVersion(url);

  if (storageVersion == highestSupportedVersion)
    return eVersionStatus::Current;
  else if (storageVersion > highestSupportedVersion)
    return eVersionStatus::Newer;
  else
    return eVersionStatus::Older;
}

void BackendPrivate::resetCache()
{
  m_cachedUrl = QUrl();
  m_cachedRawStorage = QByteArray();
  m_cachedType = eType::Unknown;
}


#ifdef ENABLE_GPG
auto BackendPrivate::checkOnlyEncryptionKeys(const QUrl &url)
{
  QVector<Issue> issues;

  Gpg::Backend gpgBackend;
  Url enhancedUrl(url);
  const auto chosenKeys = enhancedUrl.encryptionKeys();
  const auto decryptionKeys = gpgBackend.systemDecryptionKeys();
  for (const auto &chosenKey : chosenKeys)
    if (decryptionKeys.contains(chosenKey))
      return issues;

  issues.append({eIssue::OnlyEncryptionKeys, QStringList()});

  return issues;
}

auto BackendPrivate::checkExpiringKeys(const QUrl &url)
{
  QVector<Issue> issues;

  Gpg::Backend gpgBackend;
  Url enhancedUrl(url);
  auto keys = enhancedUrl.encryptionKeys();
  QStringList expiringKeys;
  for (const auto &key : keys) {
    const auto expiryDate = m_gpg->keyExpiryDate(key);
    if (expiryDate.isValid()) {
      const auto daysToExpire = QDate::currentDate().daysTo(expiryDate);
      if (daysToExpire >= 0 && daysToExpire <= Configuration::warnSoManyDaysInAdvance())
        expiringKeys.append(i18n("%1 in %2 days").arg(key).arg(daysToExpire));
    }
  }

  if (!expiringKeys.isEmpty())
    issues.append({eIssue::ExpiringEncryptionKeys, expiringKeys});

  return issues;
}

auto BackendPrivate::checkMissingKeys(const QUrl &url)
{
  QVector<Issue> issues;

  Url enhancedUrl(url);
  const auto chosenKeys = enhancedUrl.encryptionKeys();
  if (chosenKeys.isEmpty())
    issues.append({eIssue::MissingKeys, QStringList()});

  return issues;
}

auto BackendPrivate::checkMissingDecryptionKeys(const QUrl &url)
{
  QVector<Issue> issues;

  Url enhancedUrl(url);
  const auto chosenKeys = enhancedUrl.encryptionKeys();
  const auto systemKeys = m_gpg->systemDecryptionKeys();
  QStringList missingKeys;
  for (const auto &chosenKey : chosenKeys)
    if (!systemKeys.contains(chosenKey))
      missingKeys.append(chosenKey);

  const auto missingKeysPretty = m_gpg->prettyRepresentation(missingKeys);
  if (chosenKeys.count() == missingKeys.count())
    issues.append({eIssue::MissingDecryptionKeys, missingKeysPretty});

  return issues;
}

auto BackendPrivate::checkMissingEncryptionKeys(const QUrl &url)
{
  QVector<Issue> issues;

  Url enhancedUrl(url);
  const auto chosenKeys = enhancedUrl.encryptionKeys();
  const auto systemKeys = m_gpg->systemEncryptionKeys();
  QStringList missingKeys;
  for (const auto &chosenKey : chosenKeys)
    if (!systemKeys.contains(chosenKey))
      missingKeys.append(chosenKey);

  const auto missingKeysPretty = m_gpg->prettyRepresentation(missingKeys);
  if (chosenKeys.count() == missingKeys.count())
    issues.append({eIssue::MissingEncryptionKeys, missingKeysPretty});
  else if (!missingKeys.isEmpty())
    issues.append({eIssue::SomeMissingEncryptionKeys, missingKeysPretty});

  return issues;
}

auto BackendPrivate::checkMissingRecoveryKey(const QUrl &url)
{
  Q_UNUSED(url)
  QVector<Issue> issues;

  Gpg::Backend gpgBackend;
  const auto systemEncryptionKeys = gpgBackend.systemEncryptionKeys();
  if (!systemEncryptionKeys.contains(Gpg::Backend::recoveryKeyFingerprint().right(16)))
    issues.append({eIssue::MissingRecoveryKey, QStringList{Gpg::Backend::recoveryKeyUrl()}});

  return issues;
}
#endif


Backend::Backend() :
  d_ptr(std::make_unique<BackendPrivate>())
{
}

Backend::~Backend() = default;

void Backend::setProgressCallback(ProgressCallback callback)
{
  Q_D(Backend);
  d->m_callback = callback;
}

void Backend::openStorage(MyMoneyFile &storage, const QUrl &url)
{    
  Q_D(Backend);

  const auto &storageContent = d->readAll(url);
  if (storageContent.isEmpty())
    return;

  ReaderWriter xmlReader;
  xmlReader.setProgressCallback(d->m_callback);
  xmlReader.readFile(storageContent, storage);
  d->resetCache();

  return;
}

void Backend::saveStorage(const MyMoneyFile &storage, const QUrl &url)
{
  Q_D(Backend);

  QByteArray rawStorage;

  const auto localFilename = url.path();
  Url enhancedUrl(url);
  const auto storageType = enhancedUrl.decodeStorageType();

  // anonymize storage if requested by the user
  switch (storageType) {
    case eType::kmm_XmlAnonymous:
    case eType::kmmn_XmlAnonymous: {
      Anonymizer xmlAnonymizer;
      xmlAnonymizer.setProgressCallback(d->m_callback);
      rawStorage = xmlAnonymizer.writeFile(storage);
      enhancedUrl.setNoSwitch();
    } break;
    default: {
      ReaderWriter xmlWriter;
      xmlWriter.setProgressCallback(d->m_callback);
      rawStorage = xmlWriter.writeFile(storage);
    } break;
  }

  // convert to KMyMoney before saving to its file format
  switch (storageType) {
    case eType::kmm_XmlPlain:
    case eType::kmm_XmlGzip:
    case eType::kmm_XmlGpg:
    case eType::kmm_XmlAnonymous:{
      Converter xmlConverter(d->m_callback);
      rawStorage = xmlConverter.convertStorage(rawStorage, eProducer::KMyMoney);
    } break;
    default:
      break;
  }

  // choose encrypted, compressed or plain file writer
#ifdef ENABLE_GPG
  switch (storageType) {
    case eType::kmm_XmlGpg:
    case eType::kmmn_XmlGpg:
      d->m_gpg->saveDataToEncryptedFile(localFilename, rawStorage, enhancedUrl.encryptionKeys());
      return;
    default:
      break;
  }
#endif

  std::unique_ptr<QIODevice> fileHandle = nullptr;

  switch (storageType) {
    case eType::kmm_XmlPlain:
    case eType::kmmn_XmlPlain:
    case eType::kmm_XmlAnonymous:
    case eType::kmmn_XmlAnonymous:
      fileHandle = std::make_unique<QFile>(localFilename);
      break;
    case eType::kmm_XmlGzip:
    case eType::kmmn_XmlGzip:
      fileHandle = std::make_unique<KCompressionDevice>(localFilename, compressionType);
      break;
    default:
      throw MYMONEYEXCEPTION_CSTRING("Cannot save given type of storage.");
  }
  fileHandle->open(QIODevice::WriteOnly);
  fileHandle->write(rawStorage);
}

void Backend::upgradeStorage(const QUrl &url)
{
  Q_D(Backend);

  const auto &originalContent = d->readAll(url);
  const auto storageProducer = d->docType(url);
  Upgrader xmlUpgrader(d->m_callback);
  const auto storageVersion = d->storageVersion(url);

  auto upgradedContent = xmlUpgrader.upgradeStorage(originalContent, storageProducer, storageVersion);

  if (storageProducer == eProducer::KMyMoney) {
    Converter xmlConverter(d->m_callback);
    upgradedContent = xmlConverter.convertStorage(upgradedContent, eProducer::KMyMoneyNEXT);
    d->m_cachedProducer = eProducer::KMyMoneyNEXT;
    auto type = storageType(url);
    d->m_cachedType = kmmToKmmnTypeMap.value(type, type);
  }

  d->m_cachedRawStorage = upgradedContent;
}

eType Backend::storageType(const QUrl &url)
{
  Q_D(Backend);
  if (url == d->m_cachedUrl && d->m_cachedType != eType::Unknown)
    return d->m_cachedType;

  auto fileType = d->fileTypeByMagicHeader(url);
  if (fileType == eFileType::Unknown)
    return eType::Unknown;

  static const QMap<eFileType, eType> storageTypesKMM {
    {eFileType::GZIP, eType::kmm_XmlGzip},
    {eFileType::GPG,  eType::kmm_XmlGpg},
    {eFileType::XML,  eType::kmm_XmlPlain}
  };

  static const QMap<eFileType, eType> storageTypesKMMN {
    {eFileType::GZIP, eType::kmmn_XmlGzip},
    {eFileType::GPG,  eType::kmmn_XmlGpg},
    {eFileType::XML,  eType::kmmn_XmlPlain}
  };

  switch (d->docType(url)) {
    case eProducer::KMyMoney:
      d->m_cachedType = storageTypesKMM.value(fileType);
      break;
    case eProducer::KMyMoneyNEXT:
      d->m_cachedType = storageTypesKMMN.value(fileType);
      break;
    default:
      return eType::Unknown;
  }

  return d->m_cachedType;
}

QVector<eType> Backend::types() const
{
  Q_D(const Backend);
  return d->typesFiltered();
}

QVector<eType> Backend::canOpenTypes() const
{
  Q_D(const Backend);
  QVector<eType> cannotOpenTypes {
    eType::kmm_XmlAnonymous,
    eType::kmmn_XmlAnonymous,
#ifndef ENABLE_GPG
    eType::kmm_XmlGpg,
    eType::kmmn_XmlGpg,
#endif
  };

#ifdef ENABLE_GPG
  if (!gpg()->isOpeartional())
    cannotOpenTypes.append(QVector<eType>{eType::kmm_XmlGpg, eType::kmmn_XmlGpg});
#endif

  return d->typesFiltered(cannotOpenTypes);
}

QVector<eType> Backend::canSaveTypes() const
{
  Q_D(const Backend);
  QVector<eType> cannotSaveTypes {
#ifndef ENABLE_GPG
    eType::kmm_XmlGpg,
    eType::kmmn_XmlGpg,
#endif
  };

#ifdef ENABLE_GPG
  if (!gpg()->isOpeartional())
    cannotSaveTypes.append(QVector<eType>{eType::kmm_XmlGpg, eType::kmmn_XmlGpg});
#endif

  return d->typesFiltered(cannotSaveTypes);
}

bool Backend::isKMyMoneyType(eType type) const
{
  if (kmmTypes.contains(type))
    return true;
  return false;
}

QString Backend::typeDisplayedName(eType storageType) const
{
  const QMap <eType, QString> typeDisplayedName {
    {eType::kmm_XmlGzip,       i18n("KMyMoney compressed XML")},
    {eType::kmm_XmlGpg,        i18n("KMyMoney encrypted XML")},
    {eType::kmm_XmlPlain,      i18n("KMyMoney uncompressed XML")},
    {eType::kmm_XmlAnonymous,  i18n("KMyMoney anonymous XML")},
    {eType::kmmn_XmlGzip,      i18n("KMyMoneyNEXT compressed XML")},
    {eType::kmmn_XmlGpg,       i18n("KMyMoneyNEXT encrypted XML")},
    {eType::kmmn_XmlPlain,     i18n("KMyMoneyNEXT uncompressed XML")},
    {eType::kmmn_XmlAnonymous, i18n("KMyMoneyNEXT anonymous XML")},
  };
  return typeDisplayedName.value(storageType);
}

QString Backend::typeInternalName(eType storageType) const
{
  return Url::typeInternalName(storageType);
}

QVector<Issue> Backend::validateForOpen(const QUrl &url)
{
  Q_D(Backend);
  QVector<Issue> issues;

  Url enhancedUrl(url);

  const auto currentStorageType = storageType(url);
  if (currentStorageType == eType::Unknown) {
    issues.append({eIssue::InvalidStorage, QStringList()});
    return issues;
  }

  const auto declaredStorageType = enhancedUrl.decodeStorageType();
  if (currentStorageType != declaredStorageType) {
    QStringList storageTypes {typeDisplayedName(declaredStorageType),
                             typeDisplayedName(currentStorageType)};
    issues.append({eIssue::WrongTypeSelected, storageTypes});
    return issues;
  }

#ifdef ENABLE_GPG
  if (currentStorageType == eType::kmm_XmlGpg ||
      currentStorageType == eType::kmmn_XmlGpg) {
    issues.append(d->checkMissingKeys(url));
    if (!issues.isEmpty())
      return issues;

    issues.append(d->checkMissingDecryptionKeys(url));
    if (!issues.isEmpty())
      return issues;
  }
#endif

  eProducer storageProducer;
  QString storageProducerString;
  if (kmmTypes.contains(currentStorageType)) {
    storageProducer = eProducer::KMyMoney;
    storageProducerString = QStringLiteral("KMyMoney");
  } else if (kmmnTypes.contains(currentStorageType)) {
    storageProducer = eProducer::KMyMoneyNEXT;
    storageProducerString = QStringLiteral("KMyMoneyNEXT");
  } else {
    issues.append({eIssue::InvalidStorage, QStringList()});
    return issues;
  }

  const auto currentVersionStatus = d->versionStatus(url);
  switch (currentVersionStatus) {
    case eVersionStatus::Current:
      break;
    case eVersionStatus::Newer:
      issues.clear();
      issues.append({eIssue::NeedsNewerSoftware, QStringList{storageProducerString}});
      return issues;
    case eVersionStatus::Older:
      issues.append({eIssue::NeedsUpgrade, QStringList{storageProducerString}});
      break;
    default:
      issues.clear();
      issues.append({eIssue::InvalidStorage, QStringList()});
      return issues;
  }

  if (storageProducer == eProducer::KMyMoney)
    issues.append({eIssue::NeedsConversion, QStringList{storageProducerString}});

#ifdef ENABLE_GPG
  if (currentStorageType == eType::kmm_XmlGpg ||
      currentStorageType == eType::kmmn_XmlGpg) {
    issues.append(d->checkExpiringKeys(url));
  }
#endif

  return issues;
}

QVector<Issue> Backend::validateForSave(const QUrl &url)
{
  QVector<Issue> issues;

  Url storageUrl(url);
  const auto declaredStorageType = storageUrl.decodeStorageType();
  if (!canSaveTypes().contains(declaredStorageType)) {
    issues.append({eIssue::InvalidStorage, QStringList()});
    return issues;
  }

  if (isKMyMoneyType(declaredStorageType))
    issues.append({eIssue::UnrecommendedType, QStringList{"KMyMoney"}});

#ifdef ENABLE_GPG
  Q_D(Backend);
  if (declaredStorageType == eType::kmm_XmlGpg ||
      declaredStorageType == eType::kmmn_XmlGpg) {
    issues.append(d->checkMissingEncryptionKeys(url));
    if (!issues.isEmpty() && issues.last().code == eIssue::MissingEncryptionKeys)
      return issues;
    issues.append(d->checkOnlyEncryptionKeys(url));
// KHtml crashes trying to download on macOS
#if not defined(Q_OS_MACOS)
    if (Configuration::encryptWithRecoveryKeyByDefault())
      issues.append(d->checkMissingRecoveryKey(url));
#endif

  }
#endif

  return issues;
}

Gpg::Backend *Backend::gpg() const
{
#ifdef ENABLE_GPG
  Q_D(const Backend);
  return d->m_gpg.get();
#else
  return nullptr;
#endif
}

} // namespace Xml
} // namespace Storage
