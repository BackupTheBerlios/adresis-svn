/**************************************************************************
*   Copyright (C) 2006 by Hector Cruz                                     *
*   hectorcaz@gmail.com                                                   *
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
#include <QRegExp>




ADSpaceForm::ADSpaceForm(const QStringList listSpaces, QList<QVariant>  listAudiovisual ,QWidget *parent)
	: ADFormBase("<h1><b>Spaces</b><h1>" , parent)
{
	m_inserter = true;
	setup();
	foreach(QVariant a, listAudiovisual)
	{
		ADAudioVisual ayuda = *(qvariant_cast<ADAudioVisual *>(a));
		m_listAudiovisualL << ayuda;
	}
	insertListTypes(listSpaces);
	insertListAudiovisual();
}



ADSpaceForm::ADSpaceForm(const ADSpace& space, QList<QVariant>  listAudiovisualL, QList<QVariant>  listAudiovisualE, QWidget *parent) 
	: ADFormBase("<h1><b>Spaces</b><h1>" , parent)
{
	D_FUNCINFO;
	m_inserter = false;
	m_list = true;
	setup();
	
	foreach(QVariant a, listAudiovisualL)
	{
		ADAudioVisual ayuda = *(qvariant_cast<ADAudioVisual *>(a));
		m_listAudiovisualL << ayuda;
	}
	foreach(QVariant a, listAudiovisualE)
	{
		ADAudioVisual ayuda = *(qvariant_cast<ADAudioVisual *>(a));
		m_listAudiovisualE << ayuda;
	}
	
	m_space = new ADSpace(space.codeSpace() ,space.typeSpace(), space.coolAirSpace(), space.capacitySpace(), space.nameSpace());
	insertListAudiovisual();
	fill();
}



ADSpaceForm::~ADSpaceForm()
{
}


void  ADSpaceForm::fill()
{
	if(m_space->coolAirSpace())
	{ 
		acC->setCheckState( Qt::Checked );
	}
	else
	{
		acC->setCheckState( Qt::Unchecked );
	}
	static_cast<QLineEdit*>(m_inputs[tr("codigo espacio")])->setText(m_space->codeSpace());
	static_cast<QLineEdit*>(m_inputs[tr("codigo espacio")])->setReadOnly(true);
	tiposC->addItem( m_space->typeSpace() );
	capacity->setValue( (m_space->capacitySpace()).toInt() );
	static_cast<QLineEdit*>(m_inputs[tr("nombre de espacio")])->setText(m_space->nameSpace());
	

}



void ADSpaceForm::checkListsToSave()
{	
	foreach(ADAudioVisual a, m_listAudiovisualE)
	{
		if( a.codeSpace() == ("null") )
		{	
			QString code = static_cast<QLineEdit*>(m_inputs[tr("codigo espacio")])->text();
			ADAudioVisual audiovisual( a.type(), a.marksEquipment(), a.state(), a.numberInventory(), code);
			ADEvent event( ADEvent::Client, Logic::Audiovisuals, Logic::Update, QVariant::fromValue(&audiovisual));
			emit sendEvent( &event);
		}
	}
	
	foreach(ADAudioVisual a, m_listAudiovisualL)
	{
		if( a.codeSpace() != ("null") )
		{	
			QString code = "null";
			ADAudioVisual audiovisual( a.type(), a.marksEquipment(), a.state(), a.numberInventory(), code);
			ADEvent event( ADEvent::Client, Logic::Audiovisuals, Logic::Update, QVariant::fromValue(&audiovisual));
			emit sendEvent( &event);
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


void ADSpaceForm::insertListTypes(const QStringList typesSpaces)
{
	tiposC->addItems(typesSpaces);

}


void ADSpaceForm::insertListAudiovisual()
{

	//Con la variable m_list quiero representar cual de las listas estoy obteniendo, si m_list es true es porque estoy hablando de la listas de ayudas libres, si es false es la lista de ayudas asignadas a este espacio

	if(m_inserter)
	{
		QStringList lista = takeListKeys("libres");
		listSelect->addListToLeft(lista);
	}
	

	else
	{
		QStringList lista = takeListKeys("libres");
		listSelect->addListToLeft(lista);
		
		lista = takeListKeys("espacio");
		listSelect->addListToRight(lista);
		
		
	}
}


QStringList ADSpaceForm::takeListKeys(const QString &list)
{
	QStringList lista;
	if(list ==("libres"))
	{
		foreach(ADAudioVisual a, m_listAudiovisualL)
		{
			lista << a.type();
		}
	}
	else
	{
		foreach(ADAudioVisual a, m_listAudiovisualE)
		{
			lista << a.type();
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
	
	QVBoxLayout *containerLayout = new QVBoxLayout;
	container->setLayout(containerLayout);
	QHBoxLayout *layout;
	
	QLineEdit *edits;
	QStringList titles;
	
	tiposC = new QComboBox;
	
	capacity = new QSpinBox();
	capacity->setRange(1,300);
	
	acC = new QCheckBox();
	
	titles << tr("Codigo Espacio") << tr("Tipo Espacio") << tr("Aire Acondicionado") << tr("Nombre de Espacio");
	
	for(int i = 0; i < titles.count(); i++)
	{

		if(i==0)
		{
			QRegExp rx("\\d{3}-\\d{4}");
			v = new QRegExpValidator(rx, this);
			QValidator *validator = v;

			layout = new QHBoxLayout;
			layout->addWidget(new QLabel(titles[i]));
			edits = new QLineEdit();
			edits->setValidator(validator);
			layout->addWidget(edits);
			m_inputs.insert(titles[i].toLower() , edits);
			containerLayout->addLayout(layout);
			
		
		}
		else if(i==1)
		{
			layout = new QHBoxLayout;
			layout->addWidget(new QLabel(titles[i]));
			layout->addWidget(tiposC);
			containerLayout->addLayout(layout);
		}

		else if(i==2)
		{
			layout = new QHBoxLayout;
			layout->addWidget(new QLabel(titles[i]));
			layout->addWidget(acC);
			layout->addWidget(new QLabel("Capacidad"));
			layout->addWidget(capacity);
			containerLayout->addLayout(layout);
		}
		else
		{
			layout = new QHBoxLayout;
			layout->addWidget(new QLabel(titles[i]));
			edits = new QLineEdit();
			layout->addWidget(edits);
			m_inputs.insert(titles[i].toLower() , edits);
			containerLayout->addLayout(layout);
		}
	}
	
	listSelect = new ADListSelect();
	connect(listSelect, SIGNAL(listChanged(const QString&, int)), this, SLOT(listChangedSF(const QString&, int)));

	vBLayout->addWidget(listSelect, Qt::AlignVCenter);
	
	setForm(base);
	connect(this, SIGNAL(requestDone()),this, SLOT(emitEvent()));

}


void ADSpaceForm::emitEvent()
{
	bool opcion;
	if(valite())
	{
		if(acC->checkState() == Qt::Checked)
		{
			opcion=true;
		}
		else
		{
			opcion=false;
		}
		
		
		ADSpace space( static_cast<QLineEdit*>(m_inputs[tr("codigo espacio")])->text(),
			       tiposC->currentText(),
			       opcion, 
			       QString::number(capacity->value()), 
			       static_cast<QLineEdit*>(m_inputs[tr("nombre de espacio")])->text());
		
		
		Logic::Action action;
		if(m_inserter)
		{
			action = Logic::Add;
		}
		else
		{
			action = Logic::Update;
		}
		ADEvent event( ADEvent::Client, Logic::Spaces, action, QVariant::fromValue(&space));
		emit sendEvent(&event);
		checkListsToSave();	/// Se hace la actualizacion de las ayudas, tanto libres como asignadas.
		if(m_inserter)
		{
			clearFields();
		}
		else
		{
			emit requestClose();
		}
	}
}


bool ADSpaceForm::valite()
{
	bool isValid = true;
	QString s = (static_cast<QLineEdit*>(m_inputs[tr("codigo espacio")])->text());
	int pos=0;
	if( s.isEmpty() || (v->validate(s, pos) != QValidator::Acceptable))
	{
		QMessageBox::information ( 0 , "ERROR", "El codigo de espacio es incorrecto\nEste debe ser no_Edificio(guion)no_Espacio,  donde\nno_Edificio = 3 digitos\nno_Espacio = 4 digitos", 0);
		isValid=false;
	}
	
	if( (static_cast<QLineEdit*>(m_inputs[tr("nombre de espacio")])->text()).isEmpty() )
	{
		QMessageBox::information ( 0 , "ERROR", "El nombre del espacio\nes incorrecto", 0);
		isValid=false;
	}
	
	return isValid;
}


void ADSpaceForm::clearFields()
{
	static_cast<QLineEdit*>(m_inputs[tr("codigo espacio")])->clear();
	static_cast<QLineEdit*>(m_inputs[tr("nombre de espacio")])->clear();
	acC->setCheckState ( Qt::Unchecked );
	tiposC->setCurrentIndex(0);
	capacity->setValue ( 0 );
	listSelect->clearLists();
}

void ADSpaceForm::modifyContend( Logic::Module, const QList< QVariant >&)
{
	
}
