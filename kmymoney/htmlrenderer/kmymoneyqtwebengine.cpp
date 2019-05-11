/***************************************************************************
                          kmymoneyqtwebengine.cpp
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

#include "kmymoneyqtwebengine.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QPrinter>
#include <QWebEngineView>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

KMyMoneyQtWebEngine::KMyMoneyQtWebEngine(QWidget *parent) :
  m_view(new QWebEngineView(parent))
{
}

void KMyMoneyQtWebEngine::setHtml(const QString &html, const QUrl &baseUrl)
{
  m_view->setHtml(html, baseUrl);
}

qreal KMyMoneyQtWebEngine::zoomFactor() const
{
  return m_view->zoomFactor();
}

void KMyMoneyQtWebEngine::setZoomFactor(qreal factor)
{
  m_view->setZoomFactor(factor);
}

void KMyMoneyQtWebEngine::print(QPrinter *printer)
{
  m_view->page()->print(printer, [=](bool){});
}

void KMyMoneyQtWebEngine::setScrollBarValue(Qt::Orientation orientation, int value)
{
  Q_UNUSED(orientation)
  Q_UNUSED(value)
}

int KMyMoneyQtWebEngine::scrollBarValue(Qt::Orientation orientation) const
{
  Q_UNUSED(orientation)
  return 0;
}

void KMyMoneyQtWebEngine::load(const QUrl &url)
{
  m_view->load(url);
}

QWidget *KMyMoneyQtWebEngine::widget() const
{
  return m_view;
}

bool KMyMoneyQtWebEngine::acceptNavigationRequest(const QUrl &url, QWebEnginePage::NavigationType type, bool isMainFrame)
{
  if (type == QWebEnginePage::NavigationTypeLinkClicked) {
    emit urlChanged(url);
    return false;
  }
  return true;
}
