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

#include "opensave.h"
#include "opensave_p.h"

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

#include "storage/interface/enums.h"
#include "backend/url.h"
#include "backend/backend.h"

namespace Storage {
namespace Sql {

OpenSave::OpenSave(OpenSavePrivate &d, Backend *backend, eType storageType, eAction action) :
  d_ptr(&d)
{
  d.m_backend = backend;
  d.m_storageType = storageType;
  d.m_action = action;
}

OpenSave::~OpenSave() = default;

QUrl OpenSave::storageUrl() const
{
  Q_D(const OpenSave);

  QUrl url;
  Url storageUrl(url);
  storageUrl.setPluginScheme();
  storageUrl.encodeStorageType(d->m_storageType);
  storageUrl.setPath(d->m_filePath);

  if (d->m_backend->isKMyMoneyType(d->m_storageType))
    storageUrl.setNoSwitch();

  return std::move(storageUrl);
}

void OpenSave::validateUserSelections()
{
  Q_D(OpenSave);
  const auto fileInfo = QFileInfo(d->m_filePath);

  bool isValid = true;
  if (d->m_action == eAction::Open)
    isValid &= fileInfo.isFile();
  isValid &= fileInfo.suffix() == "db";

  emit userBasedValidityChanged(isValid);
}

void OpenSave::setFilePath(const QString &filePath)
{
  Q_D(OpenSave);
  d->m_filePath = filePath;
  validateUserSelections();
}

} // namespace Sql
} // namespace Storage
