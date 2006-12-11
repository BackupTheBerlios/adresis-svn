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
#include "adconsultschedule.h"
#include <QVBoxLayout>
#include <QFrame>

ADConsultSchedule::ADConsultSchedule(const QList<QVariant> &audiovisuals, const QList<QVariant> &spaces, QWidget *parent): DTabDialog(parent, true )
{
	QFrame *container = new QFrame;
	QVBoxLayout *layout = new QVBoxLayout(container);
	
	m_type = new QComboBox(container);
	m_type->insertItem(0, "Ayuda audiovisual");
	m_type->insertItem(1, "Espacio");
	connect( m_type, SIGNAL(currentIndexChanged ( int )), this,SLOT(changeType( int )));
	layout->addWidget(m_type);
	
	
	m_listAudiovisual = new QComboBox(container);
	QStringList list;
	foreach(QVariant a, audiovisuals)
	{
		ADAudioVisual * audio = qvariant_cast<ADAudioVisual *>(a);
		if(audio)
		{
			list << (audio->numberInventory() + "-" + audio->type());
		}
	}
	m_listAudiovisual->addItems(list);
	layout->addWidget(m_listAudiovisual);
	
	list.clear();
	foreach(QVariant s, spaces)
	{
		ADSpace *space = qvariant_cast<ADSpace *>(s);
		if(space)
		{
			list << space->codeSpace() + "-" + space->nameSpace();
		}
	}
	
	m_listSpaces = new QComboBox(container);
	m_listSpaces->addItems(list);
	m_listSpaces->setVisible(false);
	
	layout->addWidget(m_listSpaces);
	m_listSpaces->hide();
	addTab( container, "Recurso" );
}


ADConsultSchedule::~ADConsultSchedule()
{
	
}


QString ADConsultSchedule::type() const
{
	return m_type->currentText();
}

QString ADConsultSchedule::currentElemet() const
{
	QString key;
	if(m_type->currentIndex() == 0)
	{
		key = m_listAudiovisual->currentText().split("-")[0];
	}
	else
	{
		key = m_listSpaces->currentText().split("-")[0];
	}
	return key;
}

void ADConsultSchedule::changeType(int type)
{
	if(type == 0)
	{
		m_listAudiovisual->setVisible(true);
		m_listSpaces->setVisible(false);
	}
	else
	{
		m_listAudiovisual->setVisible(false);
		m_listSpaces->setVisible(true);
	}
}