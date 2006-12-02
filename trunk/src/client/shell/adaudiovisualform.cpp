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
#include "adspace.h"


ADAudiovisualForm::ADAudiovisualForm(const QList<QVariant> & spaces, const QStringList& types, QWidget *parent)
	: ADFormBase("<h1><b>Audiovisuals</b><h1>" , parent), m_types(types)
{
	m_inserter = true;
	m_codesSpaces <<tr("Ninguno");
	foreach(QVariant s, spaces)
	{
		QString code = qvariant_cast<ADSpace *>(s)->codeSpace();
		m_codesSpaces << code;
	}
	setup();
}


ADAudiovisualForm::~ADAudiovisualForm()
{
}

ADAudiovisualForm::ADAudiovisualForm(const ADAudioVisual * audiovisual, const QList<QVariant>& spaces, const QStringList& types, QWidget * parent): ADFormBase("<h1><b>Audiovisuals</b><h1>" , parent), m_types(types)
{
	D_FUNCINFO;
	m_codesSpaces <<tr("Ninguno");
	foreach(QVariant s, spaces)
	{
		QString code = qvariant_cast<ADSpace *>(s)->codeSpace();
		m_codesSpaces << code;
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


/**
 * elimiar esta funcion
 */
void ADAudiovisualForm::fill()
{
	if(adAudiovisual->isValid())
	{
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
	
	m_codeSpace = new QComboBox();
	layout->addWidget(new QLabel(tr("Assing space")),4,0);
	m_codeSpace->addItems(m_codesSpaces);
	layout->addWidget(m_codeSpace,4, 1);
	
	setForm(base);
	connect(this, SIGNAL(requestDone()),this, SLOT(emitEvent()));
}

void ADAudiovisualForm::emitEvent()
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
}

