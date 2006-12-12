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
#include "adformbase.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

#include "ddebug.h"
ADFormBase::ADFormBase(const QString & title,QWidget* parent)
	: QFrame(parent)
{
	QVBoxLayout *containerLayout = new QVBoxLayout();
	setLayout(containerLayout);
	m_title = new QLabel(title);
	containerLayout->addWidget(m_title);
	m_title->setAlignment ( Qt::AlignHCenter );
	m_title->setTextFormat (Qt::RichText);
}


ADFormBase::~ADFormBase()
{
}

void ADFormBase::setForm(QWidget* form)
{
	static_cast<QVBoxLayout*>(layout())->addWidget(form);
	QHBoxLayout *layout1 = new QHBoxLayout();
	
	m_buttons = new QGroupBox();
	m_buttons->setSizePolicy ( QSizePolicy::Expanding ,QSizePolicy::Fixed);
	m_buttons->setLayout(layout1);
	
	QPushButton *ok = new QPushButton("aceptar");
	QPushButton *cancel = new QPushButton("cancelar");
	connect(cancel, SIGNAL(clicked()), this, SIGNAL(requestClose()));
	connect(ok, SIGNAL(clicked()), this, SIGNAL(requestDone()));
	layout1->addWidget(ok);
	layout1->addWidget(cancel);
	
	static_cast<QVBoxLayout*>(layout())->addWidget(m_buttons, -1);
	
}

void ADFormBase::setTitle( const QString & title)
{
	m_title->setText("<h1><b>"+title+"</h1></b>");
}

void ADFormBase::receiveEvent( ADEvent *)
{
}

void modifyContend( ADEvent *event, Logic::Module, const QList< QVariant >&)
{
	
}
