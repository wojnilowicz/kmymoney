/***************************************************************************
                          kmymoneykhtml.cpp
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

#include "kmymoneykhtml.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QPrinter>
#include <QtMath>

// ----------------------------------------------------------------------------
// KDE Includes

#include <KHTMLView>
#include <KHTMLPart>

// ----------------------------------------------------------------------------
// Project Includes

KMyMoneyKHTML::KMyMoneyKHTML(QWidget *parent) :
  m_part(new KHTMLPart(parent))
{
  connect(m_part->browserExtension(), &KParts::BrowserExtension::openUrlRequest, this, &KMyMoneyKHTML::slotUrlChanged);
}

void KMyMoneyKHTML::setHtml(const QString &html, const QUrl &baseUrl)
{
  Q_UNUSED(baseUrl)
  m_part->begin();
  m_part->write(html);
  m_part->end();
}

qreal KMyMoneyKHTML::zoomFactor() const
{
  return (m_part->view()->zoomLevel() / 100);
}

void KMyMoneyKHTML::setZoomFactor(qreal factor)
{
// QWebEnginePage zoom factor is in range from 0.25 to 5.0
// KHtml zoom factor is in range from 20 to 300
  m_part->setZoomFactor(qCeil(factor * 100));
}

void KMyMoneyKHTML::print(QPrinter *printer)
{
  Q_UNUSED(printer)
  m_part->view()->print();
}

void KMyMoneyKHTML::setScrollBarValue(Qt::Orientation orientation, int value)
{
  Q_UNUSED(orientation)
  Q_UNUSED(value)
}

int KMyMoneyKHTML::scrollBarValue(Qt::Orientation orientation) const
{
  Q_UNUSED(orientation)
  return 0;
}

void KMyMoneyKHTML::load(const QUrl &url)
{
  m_part->openUrl(url);
}

QWidget *KMyMoneyKHTML::widget() const
{
  return m_part->view();
}

void KMyMoneyKHTML::slotUrlChanged(const QUrl &url, const KParts::OpenUrlArguments&, const KParts::BrowserArguments&)
{
  emit urlChanged(url);
}
