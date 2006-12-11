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
#include <QMessageBox>
#include <QGridLayout>
#include <QCheckBox>
#include <ddebug.h>
#include "adspace.h"


ADAudiovisualForm::ADAudiovisualForm(const QList<QVariant> & spaces, const QStringList& types, QWidget *parent)
	: ADFormBase("<h1><b>Audiovisuals</b><h1>" , parent), m_types(types)
{
	m_inserter = true;
	m_codesSpaces <<tr("Ninguno");
	foreach(QVariant s, spaces)
	{
		QString code = qvariant_cast<ADSpace *>(s)->codeSpace();
		if(code != "null")
		{
			m_codesSpaces << code;
		}
	}
	setup();
}


ADAudiovisualForm::~ADAudiovisualForm()
{
}

ADAudiovisualForm::ADAudiovisualForm(const ADAudioVisual * audiovisual, const QList<QVariant>& spaces, const QStringList& types, QWidget * parent): ADFormBase("<h1><b>Audiovisuals</b><h1>" , parent), m_types(types)
{
	D_FUNCINFO;
	m_codesSpaces << tr("Ninguno");
	foreach(QVariant s, spaces)
	{
		QString code = qvariant_cast<ADSpace *>(s)->codeSpace();
		if( code != "null")
		{
			m_codesSpaces << code;
		}
	}
	setup();
	m_inserter = false;
	if(audiovisual)
	{
		m_typesC->setCurrentIndex(m_typesC->findText(audiovisual->type()));
		m_marks->setText(audiovisual->marksEquipment());
		m_state->setCurrentIndex(m_state->findText(audiovisual->state()));
		m_numberInventory->setText(audiovisual->numberInventory());
		if(audiovisual->codeSpace() == "null")
		{
			m_codeSpace->setCurrentIndex(0);
		}
		else
		{
			m_codeSpace->setCurrentIndex(m_codeSpace->findText(audiovisual->codeSpace()));
		}
	}
}


void ADAudiovisualForm::setup()
{
	QWidget * base = new QWidget();
	QVBoxLayout *vBLayout = new QVBoxLayout(base);
	QGroupBox *container = new QGroupBox("Informacion");
	vBLayout->addWidget(container, Qt::AlignVCenter);
	QStringList titles, titles2, estados;
	QGridLayout *layout = new QGridLayout;
	
	container->setLayout(layout);
	m_typesC = new QComboBox;
	m_typesC->addItems(m_types);
	
	
	estados << tr("bueno") << tr("malo");
	m_state = new QComboBox;
	m_state->addItems(estados);
	
	QValidator *validator;
	QRegExp rx("\\d{10}");
	v = new QRegExpValidator(rx, this);
	validator = v;
	
	m_numberInventory = new QLineEdit();
	m_numberInventory->setValidator(validator);
	layout->addWidget(new QLabel(tr("Numero de inventario"),0,0));
	layout->addWidget(m_numberInventory,0,1);
	

	layout->addWidget(new QLabel(tr("Tipo")),1,0);
	m_typesC->setToolTip(tr("Escoja el tipo de ayuda audiovisual"));
	layout->addWidget(m_typesC,1,1);
	
	layout->addWidget(new QLabel(tr("Estado")), 2, 0);
	m_state->setToolTip(tr("Escoja el estado de la ayuda audiovisual"));
	layout->addWidget(m_state, 2, 1);
	
	m_marks = new QLineEdit();
	m_marks->setMaxLength(20);
	layout->addWidget(new QLabel(tr("Marca")), 3, 0);
	layout->addWidget(m_marks, 3,1);
	m_marks->setToolTip(tr("Ingrese la marca de la ayuda audiovisual"));
	
	m_codeSpace = new QComboBox();
	layout->addWidget(new QLabel(tr("Asignar Espacio")),4,0);
	m_codeSpace->addItems(m_codesSpaces);
	layout->addWidget(m_codeSpace,4, 1);
	
	setForm(base);
	connect(this, SIGNAL(requestDone()),this, SLOT(emitEvent()));
}

void ADAudiovisualForm::emitEvent()
{
	if(valite())
	{
		QString code = "null";
		if(!(m_codeSpace->currentText() == tr("Ninguno")))
		{
			code = m_codeSpace->currentText();
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
		emit requestClose();
	}
}


bool ADAudiovisualForm::valite()
{
	int pos = 0;
	bool isValid = true;
	QString nInventory = m_numberInventory->text();
	QString marca = m_marks->text();
	const QValidator * inventoryv = m_numberInventory->validator();
	const QValidator * marcav = m_marks->validator();;
	
	
	if(nInventory.isEmpty() || marca.isEmpty()|| (inventoryv->validate(nInventory, pos) != QValidator::Acceptable))
	{
		QMessageBox::information ( 0 , "ERROR", "Revise que alguno de los campos no sea incorrecto, por ejemplo:\nNo se encuentra en blanco algun campo\nEl numero de inventario es de 10 numeros", 0);
		isValid=false;
	}
	
	return isValid;
}

void ADAudiovisualForm::clearFields()
{
	m_numberInventory->clear();
	m_marks->clear();
	m_state->setCurrentIndex(0);
	m_typesC->setCurrentIndex( 0 );
	m_codeSpace->setCurrentIndex( 0 );
}


