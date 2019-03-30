/***************************************************************************
                            homeview.cpp
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

#include "homeview.h"

// ----------------------------------------------------------------------------
// QT Includes

// ----------------------------------------------------------------------------
// KDE Includes

#include <KPluginFactory>
#include <KLocalizedString>

// ----------------------------------------------------------------------------
// Project Includes

#include "viewinterface.h"
#include "khomeview.h"

HomeView::HomeView(QObject *parent, const QVariantList &args) :
  KMyMoneyPlugin::Plugin(parent, "homeview"/*must be the same as X-KDE-PluginInfo-Name*/),
  m_view(nullptr)
{
  Q_UNUSED(args)
  setComponentName("homeview", i18n("Home view"));
  // For information, announce that we have been loaded.
  qDebug("Plugins: homeview loaded");
}

HomeView::~HomeView()
{
  qDebug("Plugins: homeview unloaded");
}

void HomeView::plug()
{
  m_view = new KHomeView;
  viewInterface()->addView(m_view, i18n("Home"), View::Budget);
}

void HomeView::unplug()
{
  viewInterface()->removeView(View::Budget);
}

K_PLUGIN_FACTORY_WITH_JSON(HomeViewFactory, "homeview.json", registerPlugin<HomeView>();)

#include "homeview.moc"
