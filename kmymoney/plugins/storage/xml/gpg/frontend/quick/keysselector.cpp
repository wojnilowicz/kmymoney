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

#include "keysselector.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "storage/xml/gpg/frontend/keysselector_p.h"

#include "gpg/backend/keysmodel.h"

inline void initMyResource() { Q_INIT_RESOURCE(gpgresources); }

namespace Storage {
namespace Gpg {
namespace Quick {

class KeysSelectorPrivate : public Gpg::KeysSelectorPrivate
{
public:
  KeysSelectorPrivate();
  ~KeysSelectorPrivate() = default;

  QScopedPointer<QObject, QScopedPointerDeleteLater> m_uiPart;
  const QScopedPointer<QQmlApplicationEngine, QScopedPointerDeleteLater> m_engine;
};

KeysSelectorPrivate::KeysSelectorPrivate() :
  m_engine(new QQmlApplicationEngine)
{
}

KeysSelector::KeysSelector() :
  Gpg::KeysSelector(*new KeysSelectorPrivate)
{
  initMyResource();
  Q_D(KeysSelector);
  QQmlComponent component(d->m_engine.get(), QUrl(QLatin1String("qrc:/storage/gpg/keysselector.qml")));
  d->m_engine->rootContext()->setContextProperty("keyselectorbackend", this);
  d->m_engine->rootContext()->setContextProperty("keysModel", d->m_keysModel.get());
  d->m_uiPart.reset(component.create());
}

KeysSelector::~KeysSelector() = default;

QObject *KeysSelector::uiPart()
{
  Q_D(KeysSelector);
  return d->m_uiPart.get();
}

} // namespace Quick$
} // namespace Gpg
} // namespace Storage
