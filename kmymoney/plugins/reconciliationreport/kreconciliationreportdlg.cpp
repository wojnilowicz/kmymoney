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

#include "kreconciliationreportdlg.h"

// Qt includes
#include <QPushButton>
#include <QTabWidget>
#include <QPointer>

// KDE includes
#include <KStandardGuiItem>

// Project Includes

#include "kmymoneyhtmlrenderer.h"

#include "kmm_printer.h"

KReportDlg::KReportDlg(QWidget* parent, const QString& summaryReportHTML, const QString& detailsReportHTML) :
  QDialog(parent)
{
  setupUi(this);
  m_summaryHTMLPart = KMyMoneyHtmlRenderer::Create(m_summaryTab);
  m_detailsHTMLPart = KMyMoneyHtmlRenderer::Create(m_detailsTab);

  m_summaryLayout->addWidget(m_summaryHTMLPart->widget());
  m_detailsLayout->addWidget(m_detailsHTMLPart->widget());

  m_summaryHTMLPart->setHtml(summaryReportHTML, QUrl("file://"));
  m_detailsHTMLPart->setHtml(detailsReportHTML, QUrl("file://"));

  QPushButton* printButton = m_buttonBox->addButton(QString(), QDialogButtonBox::ActionRole);
  KGuiItem::assign(printButton, KStandardGuiItem::print());

  // signals and slots connections
  connect(printButton, SIGNAL(clicked()), this, SLOT(print()));
}

KReportDlg::~KReportDlg()
{
}

void KReportDlg::print()
{
  auto printer = KMyMoneyPrinter::startPrint();
  if (printer) {
    // do the actual painting job
    switch (m_tabWidget->currentIndex()) {
      case 0:
        m_summaryHTMLPart->print(printer);
        break;
      case 1:
        m_detailsHTMLPart->print(printer);
        break;
      default:
        qDebug("KReportDlg::print() current page index not handled correctly");
    }
  }
}
