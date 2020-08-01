/***************************************************************************
                          kmymoneyqtwebengine.h
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

#ifndef KMYMONEYQTWEBENGINE_H
#define KMYMONEYQTWEBENGINE_H

// ----------------------------------------------------------------------------
// QT Includes

#include <QWebEnginePage>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "kmymoneyhtmlrenderer.h"

class QWebEngineView;

class KMyMoneyQtWebEngine : public KMyMoneyHtmlRenderer
{
  public:
    explicit KMyMoneyQtWebEngine(QWidget *parent = nullptr);

    void setHtml(const QString &html, const QUrl &baseUrl = QUrl()) override final;

    qreal zoomFactor() const override final;
    void setZoomFactor(qreal factor) override final;
    void print(QPrinter *printer) override final;
    void setScrollBarValue(Qt::Orientation orientation, int value) override final;
    int scrollBarValue(Qt::Orientation orientation) const override final;
    void load(const QUrl &url) override final;

    QWidget *widget() const override final;

  protected:
    bool acceptNavigationRequest(const QUrl &url, QWebEnginePage::NavigationType type, bool isMainFrame);

  private:
    QWebEngineView *m_view;
};
#endif
