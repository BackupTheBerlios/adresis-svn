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

#include<QGridLayout>
#include "adreportfrom.h"
#include "adreport.h"
#include <ddebug.h>

ADReportForm::ADReportForm(const QString & login, QWidget *parent): 
	ADFormBase("Reportes", parent), m_user(login)
{
	m_inserter = true;
	m_consultList << "horas utilizadas de las ayudas audiovisuales" << "horas reservadas por un profesor" << "reporte de cancelaciones";
	
	QWidget *container= new QWidget(this);
	QGridLayout *grid = new QGridLayout(container);
	
	grid->addWidget(new QLabel( tr("Tipo de cosulta") ), 0, 0);
	m_type = new QComboBox();
	m_type->addItem("Listado");
	m_type->addItem("Histogramas");
	grid->addWidget(m_type, 0, 1);
	
	m_consult = new QComboBox();
	m_consult->addItems(m_consultList);
	grid->addWidget(new QLabel( tr("Consulta") ), 1, 0);
	grid->addWidget(m_consult, 1,1);
	
	grid->addWidget(new QLabel( tr("Fecha inicial")+":" ), 2, 0);
	grid->addWidget(new QLabel( tr("Fecha final")+":" ), 2, 1);
	
	m_beginDate = new DDatePicker(container);
	m_beginDate->setToolTip("Seleccione la fecha minima");
	m_endDate = new DDatePicker(container);
	m_beginDate->setToolTip("Seleccione la fecha maxima");
	grid->addWidget(m_beginDate, 3,0);
	grid->addWidget(m_endDate, 3,1);
	
	setForm(container);
	connect(this, SIGNAL(requestDone()), this, SLOT(emitEvent()));
	
}

ADReportForm::~ ADReportForm()
{
}

void ADReportForm::emitEvent()
{
	D_FUNCINFO;
	ADReport report(m_user, ADReport::TypeConsult(m_consult->currentIndex()), ADReport::TypeReport(m_type->currentIndex()),m_beginDate->date(), m_endDate->date());
	Logic::Action action;
	action = Logic::Add;
	ADEvent event( ADEvent::Client, Logic::Reports, action, QVariant::fromValue(&report ));
	emit sendEvent(&event);
	
	
}
