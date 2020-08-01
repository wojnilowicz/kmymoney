/***************************************************************************
                          kmymoneykdewebkit.h
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

#ifndef KMYMONEYKDEWEBKIT_H
#define KMYMONEYKDEWEBKIT_H

// ----------------------------------------------------------------------------
// QT Includes

// ----------------------------------------------------------------------------
// KDE Includes

#include <KWebPage>

// ----------------------------------------------------------------------------
// Project Includes

#include "kmymoneyhtmlrenderer.h"

class KWebView;

class KMyMoneyKDEWebKit : public KMyMoneyHtmlRenderer
{
  public:
    explicit KMyMoneyKDEWebKit(QWidget *parent = nullptr);

    void setHtml(const QString &html, const QUrl &baseUrl = QUrl()) override final;

    qreal zoomFactor() const override final;
    void setZoomFactor(qreal factor) override final;
    void print(QPrinter *printer) override final;
    void setScrollBarValue(Qt::Orientation orientation, int value) override final;
    int scrollBarValue(Qt::Orientation orientation) const override final;
    void load(const QUrl &url) override final;

    QWidget *widget() const override final;

  protected:
    bool acceptNavigationRequest(QWebFrame *frame, const QNetworkRequest &request, KWebPage::NavigationType type);

  private:
    KWebView *m_view;
};
#endif
