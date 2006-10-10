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
	adSpace = new ADSpace(space.codeSpace(), space.typeSpace(), space.coolAirSpace(), space.capacitySpace(), space.nameSpace());
}


ADSpaceForm::~ADSpaceForm()
{
}

void  ADSpaceForm::fill()
{
	if(adSpace->isValid())
	{
		if(adSpace->coolAirSpace())
		{ 
			acC->setCheckState( Qt::Checked );
		}
		else
		{
			acC->setCheckState( Qt::Unchecked );
		}
		
		static_cast<QLineEdit*>(m_inputs[tr("codigo espacio")])->setText(adSpace->codeSpace());
		tiposC->setCurrentIndex(tiposC->findText(adSpace->typeSpace()));
		static_cast<QLineEdit*>(m_inputs[tr("capacidad")])->setText(adSpace->capacitySpace());
		static_cast<QLineEdit*>(m_inputs[tr("nombre de espacio")])->setText(adSpace->nameSpace());
	}

}




void ADSpaceForm::emitInsertSpace()
{
	
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
		tiposC->currentText(),
		opcion,
		static_cast<QLineEdit*>(m_inputs[tr("capacidad")])->text(),
		static_cast<QLineEdit*>(m_inputs[tr("nombre de espacio")])->text()		
		);

		checkListsToSave();
	}
	else
	{
		emit requestUpdateSpace(
		static_cast<QLineEdit*>(m_inputs[tr("codigo espacio")])->text(),
		tiposC->currentText(),
		opcion,
		static_cast<QLineEdit*>(m_inputs[tr("capacidad")])->text(),
		static_cast<QLineEdit*>(m_inputs[tr("nombre de espacio")])->text()
		);

		checkListsToSave();
	}
}



void ADSpaceForm::checkListsToSave()
{	
	for(int i=0; i < m_listAudiovisualE.count();i++)
	{
		if( ((m_listAudiovisualE.at(i)).last()) ==("null") )
		{	
			emit updateAudiovisuaList((m_listAudiovisualE.at(i)).at(0),	//Tipo	
						(m_listAudiovisualE.at(i)).at(2),	//marca
						(m_listAudiovisualE.at(i)).at(3), 	//estado
						(m_listAudiovisualE.at(i)).at(1),	//numero inventario
						static_cast<QLineEdit*>(m_inputs[tr("codigo espacio")])->text()	//espacio
			);
		}
	}
	
	for(int i=0; i < m_listAudiovisualL.count();i++)
	{
		if( ((m_listAudiovisualL.at(i)).last()) !=("null") )
		{
			emit updateAudiovisuaList((m_listAudiovisualL.at(i)).at(0),	//Tipo	
						(m_listAudiovisualL.at(i)).at(2),	//marca
						(m_listAudiovisualL.at(i)).at(3), 	//estado
						(m_listAudiovisualL.at(i)).at(1),	//numero inventario
						"null" 					//espacio
			);
		}
	}
}


void ADSpaceForm::listChangedSF(const QString & lista, int pos)
{
	if(lista ==("derecha"))
	{
		m_listAudiovisualE.append(m_listAudiovisualL.takeAt(pos));
	}
	else
	{
		m_listAudiovisualL.append(m_listAudiovisualE.takeAt(pos));
	}
}



void ADSpaceForm::insertListTypes(const QList<XMLResults>& results)
{
	QList<XMLResults>::const_iterator it= results.begin();
	
	while( it != results.end() )
	{
		tipos << ((*it)["type"]);
		++it;
	}

	tiposC->addItems(tipos);

	if(!m_inserter)
	{
		fill();
	}
}




void ADSpaceForm::insertListAudiovisual(const QList<XMLResults>& results)
{

	QList<XMLResults>::const_iterator it= results.begin();

	
	//Con la variable m_list quiero representar cual de las listas estoy obteniendo, si m_list es true es porque estoy hablando de la listas de ayudas libres, si es false es la lista de ayudas asignadas a este espacio

	QStringList listaDeListas;

	if(m_inserter)
	{
		
		while( it != results.end() )
		{
			int pos = 0;
			listaDeListas.insert(pos, (*it)["typespace"]);
			pos++;
			listaDeListas.insert(pos, (*it)["numberinventoryav"]);
			pos++;
			listaDeListas.insert(pos, (*it)["marksequipmentav"]);
			pos++;
			listaDeListas.insert(pos, (*it)["estateav"]);
			pos++;
			listaDeListas.insert(pos, (*it)["codespace"]);

			m_listAudiovisualL.append(listaDeListas);
			++it;
			
		}
			
		QStringList lista = takeListKeys("libres");
		listSelect->addListToLeft(lista);
	}

	else
	{
		if(m_list)
		{	
			while( it != results.end() )
			{
				int pos=0;
				listaDeListas.insert(pos, (*it)["typespace"]);
				pos++;
				listaDeListas.insert(pos, (*it)["numberinventoryav"]);
				pos++;
				listaDeListas.insert(pos, (*it)["marksequipmentav"]);
				pos++;
				listaDeListas.insert(pos, (*it)["estateav"]);
				pos++;
				listaDeListas.insert(pos, (*it)["codespace"]);

				m_listAudiovisualL.append(listaDeListas);
				++it;
				
			}
		
			QStringList lista = takeListKeys("libres");
			
			listSelect->addListToLeft(lista);
			m_list=false;

		}
		else
		{

			while( it != results.end() )
			{
				int pos=0;
				listaDeListas.insert(pos, (*it)["typespace"]);
				pos++;
				listaDeListas.insert(pos, (*it)["numberinventoryav"]);
				pos++;
				listaDeListas.insert(pos, (*it)["marksequipmentav"]);
				pos++;
				listaDeListas.insert(pos, (*it)["estateav"]);
				pos++;
				listaDeListas.insert(pos, (*it)["codespace"]);

				m_listAudiovisualE.append(listaDeListas);
				++it;
			}
			
			QStringList lista = takeListKeys("espacio");
			
			listSelect->addListToRight(lista);
			m_list=true;
		}
	}
}


QStringList ADSpaceForm::takeListKeys(const QString &list)
{
	QStringList lista;
	if(list ==("libres"))
	{
		for(int i=0; i < m_listAudiovisualL.count();i++)
		{
			lista << (m_listAudiovisualL.at(i)).first();
		}
	}
	else
	{
		for(int i=0; i < m_listAudiovisualE.count();i++)
		{
			lista << (m_listAudiovisualE.at(i)).first();
		}
	}

	return lista;
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
	
	tiposC = new QComboBox;	

	acC = new QCheckBox(tr("Aire Acondicionado"),0);
	
	titles << tr("Codigo Espacio") << tr("Tipo Espacio") << tr("Aire Acondicionado") << tr("Capacidad") << tr("Nombre de Espacio");
	
	for(int i = 0; i < titles.count(); i++)
	{

		if(i==1)
		{
			layout->addWidget(new QLabel("Tipo"),1,0);
			layout->addWidget(tiposC,1,1);
		}

		else if(i==2)
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
	connect(listSelect, SIGNAL(listChanged(const QString&, int)), this, SLOT(listChangedSF(const QString&, int)));

	vBLayout->addWidget(listSelect, Qt::AlignVCenter);
	
	setForm(base);
	connect(this, SIGNAL(requestDone()),this, SLOT(emitInsertSpace()));

}



