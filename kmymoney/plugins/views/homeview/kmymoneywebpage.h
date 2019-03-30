/***************************************************************************
                          kmymoneywebpage.h
                             -------------------
        copyright            : (C) 2017 by Łukasz Wojniłowicz <lukasz.wojnilowicz@gmail.com>
                               
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KMYMONEYWEBPAGE_H
#define KMYMONEYWEBPAGE_H

#include <config-kmymoney.h>

// ----------------------------------------------------------------------------
// QT Includes

// ----------------------------------------------------------------------------
// KDE Includes

#ifdef ENABLE_WEBENGINE
#include <QWebEnginePage>
#else
#include <KWebPage>
#endif

// ----------------------------------------------------------------------------
// Project Includes


#ifdef ENABLE_WEBENGINE
class MyQWebEnginePage : public QWebEnginePage
#else
class MyQWebEnginePage : public KWebPage
#endif
{
  Q_OBJECT

public:
#ifdef ENABLE_WEBENGINE
  explicit MyQWebEnginePage(QObject* parent = nullptr) : QWebEnginePage(parent){}
#else
  explicit MyQWebEnginePage(QObject* parent = nullptr) : KWebPage(parent){}
#endif

protected:
#ifdef ENABLE_WEBENGINE
  bool acceptNavigationRequest(const QUrl &url, NavigationType type, bool) final override;
#else
  bool acceptNavigationRequest(QWebFrame *frame, const QNetworkRequest &request, NavigationType type) final override;
#endif

};
#endif
