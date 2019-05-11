/***************************************************************************
                          kmymoneyhtmlrenderer.cpp
                             -------------------
        copyright            : (C) 2019 by Łukasz Wojniłowicz <lukasz.wojnilowicz@gmail.com>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "config-htmlrenderer.h"
#include "kmymoneyhtmlrenderer.h"

// ----------------------------------------------------------------------------
// QT Includes

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#ifdef ENABLE_QTWEBENGINE
#include "kmymoneyqtwebengine.h"
#endif

#ifdef ENABLE_KDEWEBKIT
#include "kmymoneykdewebkit.h"
#endif

#ifdef ENABLE_KHTML
#include "kmymoneykhtml.h"
#endif

KMyMoneyHtmlRenderer *KMyMoneyHtmlRenderer::Create(QWidget *parent)
{
#if defined(ENABLE_QTWEBENGINE)
  return new KMyMoneyQtWebEngine(parent);
#elif defined(ENABLE_KDEWEBKIT)
  return new KMyMoneyKDEWebKit(parent);
#elif defined(ENABLE_KHTML)
  return new KMyMoneyKHTML(parent);
#endif
}
