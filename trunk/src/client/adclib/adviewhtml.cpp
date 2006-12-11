/***************************************************************************
 *   Copyright (C) 2006 by Jorge Cuadrado                                  *
 *   kuadrosxx@gmail.com                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "adviewhtml.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPrinter>
#include <QFileDialog>
#include <QPrintDialog>

ADViewHtml::ADViewHtml(QWidget *parent)
 : QFrame(parent)
{
	QVBoxLayout *layout = new QVBoxLayout(this);
	
	m_view = new QTextBrowser;
	layout->addWidget(m_view);
	
	m_panel = new QGroupBox(this);
	QHBoxLayout *panelLayout = new QHBoxLayout(m_panel);
	QPushButton *printButton = new QPushButton(tr("Imprimir"));
	connect(printButton,SIGNAL(clicked()), this, SLOT(print()));
	panelLayout->addWidget(printButton);
	
	QPushButton *pdfButton = new QPushButton(tr("Exportar a pdf"));
	connect(pdfButton,SIGNAL(clicked()), this, SLOT(exportToPdf()));
	panelLayout->addWidget(pdfButton);
	
	QPushButton *closeButton = new QPushButton(tr("Cerrar"));
	connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
	m_panel->show();
	panelLayout->addWidget(closeButton);
	layout->addWidget(m_panel);
	
}


ADViewHtml::~ADViewHtml()
{
}

void ADViewHtml::setHtml(const QString& html )
{
	m_view->insertHtml(html);
}


void ADViewHtml::print()
{
	#ifndef QT_NO_PRINTER
    QPrinter printer(QPrinter::HighResolution);
    printer.setFullPage(true);
	 QPrintDialog *dlg = new QPrintDialog(&printer, this);
	 if (dlg->exec() == QDialog::Accepted) {
		 m_view->document()->print(&printer);
	 }
	 delete dlg;
	#endif
}

void ADViewHtml::exportToPdf()
{
#ifndef QT_NO_PRINTER
    QString fileName = QFileDialog::getSaveFileName(this, tr("Exportar a pdf"), QString(), "*.pdf");
    if (fileName.isEmpty())
		 return;
	 
	 QFileInfo info(fileName);
	 if(info.suffix () != "pdf")
	 {
		 fileName += ".pdf";
	 }
	 
	 QPrinter printer(QPrinter::HighResolution);
	 printer.setOutputFormat(QPrinter::PdfFormat);
	 printer.setOutputFileName(fileName);
	 m_view->document()->print(&printer);
#endif
}
