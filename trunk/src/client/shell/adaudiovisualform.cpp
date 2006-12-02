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
#include "adaudiovisualform.h"

#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>

#include <QGridLayout>
#include <QCheckBox>
#include <ddebug.h>


ADAudiovisualForm::ADAudiovisualForm(const QStringList& types, QWidget *parent)
	: ADFormBase("<h1><b>Audiovisuals</b><h1>" , parent), m_types(types)
{
	m_inserter = true;
	setup();
}


ADAudiovisualForm::~ADAudiovisualForm()
{
}


// Este metodo  lo cambie debido a que los combobox no estaban tomando los datos reales, en especial el combobox tiposC que es el que me determina que tipo de ayuda es. Entonces de ahora en adelante lo que hago es en admainwindow creo un adspaceform cun una ayudaaudiovisual esa ayuda que me entra como parametro la guardo aqui en adAudiovisual y luego, cuando ya tenga los tipos de ayuda lo que hago es llenar los campos. con la ayuda que tengo actualmente guardada. 
// Esto me toco hacerlo debido a que cuando yo hacia la peticion de la lista de tipos de ayudas siempre me creaba primero la forma y despues me llegaban los tipos, entonces dado esto ahora mas bien lo que hago es cuando ya me hallan llegado la lista de tipos de ayudas ahi si mando a rellenar los campos.

ADAudiovisualForm::ADAudiovisualForm(const ADAudioVisual * audiovisual, QWidget * parent): ADFormBase("<h1><b>Audiovisuals</b><h1>" , parent)
{
	D_FUNCINFO;
	setup();
	m_inserter = false;
	
	m_typesC->setCurrentIndex(m_typesC->findText(adAudiovisual->type()));
	m_marks->setText(audiovisual->marksEquipment());
	m_state->setCurrentIndex(m_typesC->findText(adAudiovisual->type()));
	m_numberInventory->setText(audiovisual->numberInventory());
	m_codeSpace->setText(audiovisual->codeSpace());
}



void ADAudiovisualForm::insertListTypes(const QList<XMLResults>& results)
{
	QList<XMLResults>::const_iterator it= results.begin();
	
	while( it != results.end() )
	{
		tipos << ((*it)["type"]);
		++it;
	}

	m_typesC->addItems(tipos);

	if(!m_inserter)
	{
		fill();
	}
}


void ADAudiovisualForm::fill()
{
	if(adAudiovisual->isValid())
	{
// 		m_typesC->setCurrentIndex(m_typesC->findText(adAudiovisual->type())); 
// 		m_typesC->currentText(), 

		
// 		m_typesC->setCurrentIndex(m_typesC->findText(adAudiovisual->type())); 
// 		static_cast<QLineEdit*>(m_inputs[tr("mark")])->setText(adAudiovisual->marksEquipment());
// 		estadoC->setCurrentIndex(estadoC->findText(adAudiovisual->state()));
// 		static_cast<QLineEdit*>(m_inputs[tr("number of inventory")])->setText(adAudiovisual->numberInventory());
// 		static_cast<QLineEdit*>(m_inputs[tr("assing space")])->setText(adAudiovisual->codeSpace());
	}
}


void ADAudiovisualForm::setup()
{
	QWidget * base = new QWidget();
	QVBoxLayout *vBLayout = new QVBoxLayout(base);
	QGroupBox *container = new QGroupBox("Informacion");
	vBLayout->addWidget(container, Qt::AlignVCenter);
	QLabel *label;
	QStringList titles, titles2, estados;
	QGridLayout *layout = new QGridLayout;
	
	container->setLayout(layout);
	
	label = new QLabel("NOTA: SI ESTA AYUDA NO ESTA ASIGNADO A NINGUN ESPACIO, DEJE EN BLANCO EL CAMPO \"ASIGNADO AL ESPACIO\"");

	vBLayout->addWidget(label);

	m_typesC = new QComboBox;
	m_typesC->addItems(m_types);
	
	estados << tr("bueno") << tr("malo");
	m_state = new QComboBox;
	m_state->addItems(estados);
	
	m_numberInventory = new QLineEdit();
	layout->addWidget(new QLabel(tr("Number of inventory"),0,0));
	layout->addWidget(m_numberInventory,0,1);
	
	layout->addWidget(new QLabel(tr("Type")),1,0);
	m_typesC->setToolTip(tr("Choose type of audiovisual"));
	layout->addWidget(m_typesC,1,1);
	
	layout->addWidget(new QLabel(tr("State")), 2, 0);
	m_state->setToolTip(tr("Choose state of audiovisual"));
	layout->addWidget(m_state, 2, 1);
	
	m_marks = new QLineEdit();
	layout->addWidget(new QLabel(tr("Mark")), 3, 0);
	layout->addWidget(m_marks, 3,1);
	m_marks->setToolTip(tr("Enter mark of audiovisual"));
	
	m_codeSpace = new QLineEdit();
	layout->addWidget(new QLabel(tr("Assing space")),4,0);
	layout->addWidget(m_codeSpace,4, 1);
	
	setForm(base);
	connect(this, SIGNAL(requestDone()),this, SLOT(emitEvent()));
}

void ADAudiovisualForm::emitEvent()
{
	QString code = "null";
	if(!m_codeSpace->text().isEmpty())
	{
		code = m_codeSpace->text();
	}
	
	
	ADAudioVisual audiovisual( m_typesC->currentText(), m_marks->text(),
										 m_state->currentText(),
										 m_numberInventory->text(),
										 code);
	Logic::Action action;
	if(m_inserter)
	{
		action = Logic::Add;
	}
	else
	{
		action = Logic::Update;
	}
	ADEvent event( ADEvent::Client, Logic::Audiovisuals, action, QVariant::fromValue(&audiovisual));
	emit sendEvent(&event);
}

