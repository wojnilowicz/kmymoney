/***************************************************************************
                             homeview.h
                             -------------------
    copyright            : (C) 2018 by Łukasz Wojniłowicz
    email                : lukasz.wojnilowicz@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef HOMEVIEW_H
#define HOMEVIEW_H

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// QT Includes

// Project Includes

#include "kmymoneyplugin.h"

class KHomeView;

class HomeView : public KMyMoneyPlugin::Plugin
{
  Q_OBJECT

public:
  explicit HomeView(QObject *parent, const QVariantList &args);
  ~HomeView() override;

  void plug() override;
  void unplug() override;

private:
  KHomeView* m_view;
};

#endif
