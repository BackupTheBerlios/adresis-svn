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
#include "adinsertresource.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>

ADInsertResource::ADInsertResource(QWidget *parent)
 : QDialog(parent)
{
	QVBoxLayout *vBLayout = new QVBoxLayout;
	typeResource = new QComboBox();
	typeResource->addItems(QStringList() << "Espacios" << "Audiovisual");
	connect(typeResource, SIGNAL( activated(int) ), this, SLOT( changeTypeResource(int) ));
	resource = new QComboBox();
	connect( resource, SIGNAL( activated(const QString&)), this, SLOT( changeNameResource(const QString&)) );
	edit = new QLineEdit();
	QHBoxLayout *hBLayout;
	
	hBLayout = new QHBoxLayout;
	hBLayout->addWidget(new QLabel("Tipo de Recurso"));
	hBLayout->addWidget(typeResource);
	vBLayout->addLayout(hBLayout);
	hBLayout = new QHBoxLayout;
	hBLayout->addWidget(new QLabel("Recurso"));
	hBLayout->addWidget(resource);
	vBLayout->addLayout(hBLayout);
	vBLayout->addWidget(edit);
	
	hBLayout = new QHBoxLayout;
	addButton = new QPushButton("Agregar Recurso");
	connect (addButton, SIGNAL( clicked() ), this, SLOT( insertResource() ));
	delButton = new QPushButton("Borrar Recurso");
	connect (delButton, SIGNAL( clicked() ), this, SLOT( deleteResource() ));
	hBLayout->addWidget(addButton);
	hBLayout->addWidget(delButton);
	vBLayout->addLayout(hBLayout);
	
	hBLayout = new QHBoxLayout;
	okButton = new QPushButton("OK");
	connect( okButton, SIGNAL(clicked() ), this, SLOT( ok() ));
	cancelButton = new QPushButton("CANCEL");
	connect( cancelButton, SIGNAL( clicked() ), this, SLOT( cancel() ) );
	hBLayout->addWidget(okButton);
	hBLayout->addWidget(cancelButton);
	
	vBLayout->addLayout(hBLayout);
	
	setLayout(vBLayout);
	list=1;
}


ADInsertResource::~ADInsertResource()
{
}

void ADInsertResource::insertListTypes(const QList<XMLResults>& results)
{
	QList<XMLResults>::const_iterator it= results.begin();
	QStringList recursos;
	recursos.insert(0, "ESCOJA UN RECURSO");
	while( it != results.end() )
	{
		recursos << ((*it)["type"]);
		++it;
	}
	
	if(list == 1)
	{
		recursosEsp = recursos;
	}

	else if(list == 2)
	{
		recursosAud = recursos;
		changeTypeResource(0);
	}
	
	list++;
}


void ADInsertResource::changeTypeResource(int opcion)
{
	resource->clear();
//	opcion = 0 -> Espacios
//	opcion = 1 -> audiovisual

	if(opcion == 0)
	{
		resource->addItems(recursosEsp);
	}

	else if(opcion == 1)
	{
		resource->addItems(recursosAud);
	}
}

void ADInsertResource::insertResource()
{
	if( valite() )
	{
		QMessageBox::information ( 0 , "ERROR", "El nombre de el nuevo recurso debe\nser diferente a los ya especifcados", 0);
	}
	else
	{
		QString opcion = "add";
		QString texto = edit->text();
		QString type = typeResource->currentText();
		QString table;
		resource->addItem(texto);
		resource->clear();
		if( type.operator==("Espacios"))
		{
			table = "adspacetype";
		}
		else if( type.operator==("Audiovisual") )
		{
			table = "adaudiovisualtype";
		}
		emit addDelResource(opcion, table, texto);
	}
}

void ADInsertResource::deleteResource()
{
	if(valite())
	{
		QMessageBox::information ( 0 , "ERROR", "El nombre de el recurso a eliminar no esta\ndentro de los recursos existentes", 0);
	}
	else
	{
		QString opcion = "del";
		QString texto = edit->text();
		QString type = typeResource->currentText();
		resource->clear();
		resource->removeItem( resource->currentIndex() );
		QString table;
		if( type.operator==("Espacios"))
		{
			table = "adspacetype";
		}
		else if( type.operator==("Audiovisual") )
		{
			table = "adaudiovisualtype";
		}
		emit addDelResource(opcion, table, texto);
	}
}

bool ADInsertResource::valite()
{
	int pos = resource->findText( edit->text() );
	if(pos == -1)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void ADInsertResource::ok()
{
	accept();
}

void ADInsertResource::cancel()
{
	reject();
}

void ADInsertResource::changeNameResource( const QString & resource)
{
	if( resource.operator!=("ESCOJA UN RECURSO") )
	{
		edit->setText(resource);
	}
}