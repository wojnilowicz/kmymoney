/***************************************************************************
 *   Copyright 2009  Cristian Onet onet.cristian@gmail.com                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License as        *
 *   published by the Free Software Foundation; either version 2 of        *
 *   the License or (at your option) version 3 or any later version        *
 *   accepted by the membership of KDE e.V. (or its successor approved     *
 *   by the membership of KDE e.V.), which shall act as a proxy            *
 *   defined in Section 14 of version 3 of the license.                    *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>  *
 ***************************************************************************/
#ifndef KCM_CHECKPRINTING_H
#define KCM_CHECKPRINTING_H

#include <config-kmymoney.h>

#include <KCModule>
#include <QWidget>
#include "ui_pluginsettingsdecl.h"

class KMyMoneyHtmlRenderer;

class PluginSettingsWidget : public QWidget, public Ui::PluginSettingsDecl
{
  Q_OBJECT

public:
  explicit PluginSettingsWidget(QWidget* parent = 0);

public Q_SLOTS:
  void urlSelected(const QUrl &url);
  void returnPressed(const QString& url);

private:
  KMyMoneyHtmlRenderer *m_checkTemplatePreviewHTMLPart;
};

class KCMCheckPrinting : public KCModule
{
public:
  KCMCheckPrinting(QWidget* parent, const QVariantList& args);
  ~KCMCheckPrinting();
};

#endif

