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
#include "adspaceform.h"

#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QGridLayout>
#include <ddebug.h>


ADSpaceForm::ADSpaceForm(QWidget *parent)
	: ADFormBase("<h1><b>Spaces</b><h1>" , parent)
{
	QWidget * base = new QWidget();
	QVBoxLayout *vBLayout = new QVBoxLayout(base);
	
	QGroupBox *container = new QGroupBox("Informacion");
	
	vBLayout->addWidget(container, Qt::AlignVCenter);
	
	
	QGridLayout *layout = new QGridLayout;
	
	container->setLayout(layout);
	
	QLabel *label;
	QLineEdit *edits;
	QStringList titles, titles2, ac, estados;
	
	acC = new QCheckBox(tr("Aire Acondicionado"),0);
	
	titles << tr("Codigo Espacio") << tr("Tipo Espacio") << tr("Aire Acondicionado") << tr("Capacidad") << tr("Nombre de Espacio");
	
	for(int i = 0; i < titles.count(); i++)
	{
		if(i==2)
		{
			layout->addWidget(new QLabel("Aire Acondicionado"),2,0);
			layout->addWidget(acC,2,1);
		}
		else
		{
			label = new QLabel(titles[i] );
			layout->addWidget(label, i, 0);
			edits = new QLineEdit();
			layout->addWidget(edits, i, 1);
			m_inputs.insert(titles[i].toLower() , edits);
		}
	}
	
	listSelect = new ADListSelect();
	vBLayout->addWidget(listSelect, Qt::AlignVCenter);
	
	setForm(base);
	connect(this, SIGNAL(requestDone()),this, SLOT(emitInsertSpace()));
}


ADSpaceForm::~ADSpaceForm()
{
}

void ADSpaceForm::emitInsertSpace()
{
	dDebug() << "Entre al emitInsertSpace";
	bool opcion;
	
	if(acC->checkState() == Qt::Checked)
	{
		opcion=true;
	}
	else
	{
		opcion=false;
	}
	
	
	
	dDebug() << static_cast<QLineEdit*>(m_inputs[tr("codigo espacio")])->text();
	dDebug() << static_cast<QLineEdit*>(m_inputs[tr("tipo espacio")])->text();
	dDebug() << opcion;
	dDebug() << static_cast<QLineEdit*>(m_inputs[tr("capacidad")])->text();
	dDebug() << static_cast<QLineEdit*>(m_inputs[tr("nombre de espacio")])->text();
	
	
	
	
	emit requestInsertSpace(
			static_cast<QLineEdit*>(m_inputs[tr("codigo espacio")])->text(),			static_cast<QLineEdit*>(m_inputs[tr("tipo espacio")])->text(),
			opcion,
			static_cast<QLineEdit*>(m_inputs[tr("capacidad")])->text(),
			static_cast<QLineEdit*>(m_inputs[tr("nombre de espacio")])->text()
	);
	dDebug() << "Sali del emitInsertSpace";
}




