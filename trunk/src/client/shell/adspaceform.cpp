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

#include <QGridLayout>
#include <QCheckBox>
#include <ddebug.h>




ADSpaceForm::ADSpaceForm(QWidget *parent)
	: ADFormBase("<h1><b>Spaces</b><h1>" , parent)
{
	m_inserter = true;
	setup();
}

ADSpaceForm::ADSpaceForm(const ADSpace& space, QWidget *parent) 
	: ADFormBase("<h1><b>Spaces</b><h1>" , parent)
{
	D_FUNCINFO;
	setup();
	m_inserter = false;
	m_list = true;
	if(space.isValid())
	{
	
		if(space.coolAirSpace())
		{ 
			acC->setCheckState( Qt::Checked );
		}
		else
		{
			acC->setCheckState( Qt::Unchecked );
		}
		
		
		static_cast<QLineEdit*>(m_inputs[tr("codigo espacio")])->setText(space.codeSpace());
		static_cast<QLineEdit*>(m_inputs[tr("tipo espacio")])->setText(space.typeSpace());
		static_cast<QLineEdit*>(m_inputs[tr("capacidad")])->setText(space.capacitySpace());
		static_cast<QLineEdit*>(m_inputs[tr("nombre de espacio")])->setText(space.nameSpace());
	}
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
	
		


	if(m_inserter)
	{
		emit requestInsertSpace(
		static_cast<QLineEdit*>(m_inputs[tr("codigo espacio")])->text(),
		static_cast<QLineEdit*>(m_inputs[tr("tipo espacio")])->text(),
		opcion,
		static_cast<QLineEdit*>(m_inputs[tr("capacidad")])->text(),
		static_cast<QLineEdit*>(m_inputs[tr("nombre de espacio")])->text()		
	);
	}
	else
	{
		emit requestUpdateSpace(
		static_cast<QLineEdit*>(m_inputs[tr("codigo espacio")])->text().trimmed(),
		static_cast<QLineEdit*>(m_inputs[tr("tipo espacio")])->text().trimmed(),
		opcion,
		static_cast<QLineEdit*>(m_inputs[tr("capacidad")])->text().trimmed(),
		static_cast<QLineEdit*>(m_inputs[tr("nombre de espacio")])->text().trimmed()
	);
	}

}

void ADSpaceForm::insertListAudiovisual(const QList<XMLResults>& results)
{

	QList<XMLResults>::const_iterator it= results.begin();

	
	//Con la variable m_list quiero representar cual de las listas estoy obteniendo, si m_list es true es porque estoy hablando de la listas de ayudas libres, si es false es la lista de ayudas asignadas a este espacio

	
	if(m_inserter)
	{

		while( it != results.end() )
		{
			m_listAudiovisualL.insert((*it)["typeav"], (*it)["numberinventoryav"]);
			++it;
		}
			
		QStringList lista = QStringList(m_listAudiovisualL.keys());
	
		dDebug() << "";
		foreach(QString elem, lista)
		{
			dDebug() << "elemento de la lista libres " << elem;
		}

		listSelect->addListToLeft(lista);
	}
	else
	{
		if(m_list)
		{	
			while( it != results.end() )
			{
				m_listAudiovisualL.insert((*it)["typeav"], (*it)["numberinventoryav"]);
				++it;
			}
		
			QStringList lista = QStringList(m_listAudiovisualL.keys());
			
			dDebug() << "";
			foreach(QString elem, lista)
			{
				dDebug() << "elemento de la lista libres " << elem;
			}

 			dDebug() << "Estoy en el otro if m_list=" << m_list;
			listSelect->addListToLeft(lista);
			m_list=false;

		}
		else
		{

			while( it != results.end() )
			{
				m_listAudiovisualE.insert((*it)["typeav"], (*it)["numberinventoryav"]);
				++it;
			}
			
			QStringList lista = QStringList(m_listAudiovisualE.keys());
			
			dDebug() << "";
			foreach(QString elem, lista)
			{
				dDebug() << "elemento de la lista del espacio " << elem;
			}
			
			dDebug() << "Estoy en el else de el otro if m_list=" << m_list;
			listSelect->addListToRight(lista);
			m_list=true;
		}
	}
}




void ADSpaceForm::setup()
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



