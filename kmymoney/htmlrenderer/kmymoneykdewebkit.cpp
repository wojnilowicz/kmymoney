/***************************************************************************
                          kmymoneykdewebkit.cpp
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

#include "kmymoneykdewebkit.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QPrinter>

// ----------------------------------------------------------------------------
// KDE Includes

#include <KWebView>

// ----------------------------------------------------------------------------
// Project Includes

KMyMoneyKDEWebKit::KMyMoneyKDEWebKit(QWidget *parent) :
  m_view(new KWebView(parent))
{
}

void KMyMoneyKDEWebKit::setHtml(const QString &html, const QUrl &baseUrl)
{
  m_view->setHtml(html, baseUrl);
}

qreal KMyMoneyKDEWebKit::zoomFactor() const
{
  return m_view->zoomFactor();
}

void KMyMoneyKDEWebKit::setZoomFactor(qreal factor)
{
  m_view->setZoomFactor(factor);
}

void KMyMoneyKDEWebKit::print(QPrinter *printer)
{
  m_view->print(printer);
}

void KMyMoneyKDEWebKit::setScrollBarValue(Qt::Orientation orientation, int value)
{
  m_view->page()->mainFrame()->setScrollBarValue(orientation, value);
}

int KMyMoneyKDEWebKit::scrollBarValue(Qt::Orientation orientation) const
{
  return m_view->scrollBarValue(orientation);
}

void KMyMoneyKDEWebKit::load(const QUrl &url)
{
  m_view->load(url);
}

QWidget *KMyMoneyKDEWebKit::widget() const
{
  return m_view;
}

bool KMyMoneyKDEWebKit::acceptNavigationRequest(QWebFrame *frame, const QNetworkRequest &request, KWebPage::NavigationType type)
{
  Q_UNUSED(frame);
  if (type == KWebPage::NavigationTypeLinkClicked) {
    emit urlChanged(request.url());
    return false;
  }
  return true;
}
