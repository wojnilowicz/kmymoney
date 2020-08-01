/***************************************************************************
                          kmymoneywebpage.h
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

#ifndef KMYMONEYHTMLRENDERER_H
#define KMYMONEYHTMLRENDERER_H

#include "kmm_htmlrenderer_export.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QObject>
#include <QUrl>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

class QPrinter;

class KMM_HTMLRENDERER_EXPORT KMyMoneyHtmlRenderer : public QObject
{
  Q_OBJECT
  Q_DISABLE_COPY(KMyMoneyHtmlRenderer)

public:
  KMyMoneyHtmlRenderer() = default;
  static KMyMoneyHtmlRenderer *Create(QWidget *parent = nullptr);
  virtual void setHtml(const QString &html, const QUrl &baseUrl = QUrl()) = 0;

  virtual qreal zoomFactor() const = 0;
  virtual void setZoomFactor(qreal factor) = 0;
  virtual void print(QPrinter *printer) = 0;
  virtual void setScrollBarValue(Qt::Orientation orientation, int value) = 0;
  virtual int scrollBarValue(Qt::Orientation orientation) const = 0;
  virtual void load(const QUrl &url) = 0;

  virtual QWidget *widget() const = 0;

signals:
  void urlChanged(const QUrl &url);
};

#endif
