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
#include "adlistselect.h"
#include <QListWidgetItem>
#include <QGridLayout>
#include <QDrag>
#include <ddebug.h>
#include <global.h>
#include <QMessageBox>
#include <QLabel>

ADListSelect::ADListSelect(QWidget *parent): QWidget(parent)
{
	QGridLayout *hBLayout = new QGridLayout;
	QVBoxLayout *vBLayout = new QVBoxLayout;
	listWidgetLeft = new QListWidget(this);
	listWidgetLeft->setDragEnabled (true);
	listWidgetLeft->setAcceptDrops ( true );
	
	listWidgetRight = new QListWidget(this);
	listWidgetRight->setDragEnabled (true);
	listWidgetRight->setAcceptDrops (true );
	
	qTButtonLeft = new QToolButton(this);
	qTButtonLeft->setIcon( QIcon(THEME_DIR+"/icons/previous.png"));
	QObject::connect(qTButtonLeft, SIGNAL(clicked()), this, SLOT(addItemToLeft()));
	
	
	qTButtonRight = new QToolButton(this);
	qTButtonRight->setIcon( QIcon(THEME_DIR+"/icons/next.png"));
	QObject::connect(qTButtonRight,SIGNAL(clicked()),this, SLOT(addItemToRight()));
	
	hBLayout->addWidget(new QLabel("Ayudas Libres"),0,0);
	hBLayout->addWidget(listWidgetLeft,1,0);
	
	vBLayout->addWidget(qTButtonRight);
	vBLayout->addWidget(qTButtonLeft);
	hBLayout->addLayout(vBLayout,1,1);
	
	hBLayout->addWidget(new QLabel("Ayudas Asignadas"),0,2);
	hBLayout->addWidget(listWidgetRight,1,2);
		
	delete layout();
	setLayout(hBLayout);
}


ADListSelect::~ADListSelect()
{
}


void ADListSelect::addItemToLeft()
{
	int pos = listWidgetRight->currentRow();
	
	QListWidgetItem *itemTmp = listWidgetRight->takeItem(pos);
	
	if(itemTmp)
	{
		listWidgetLeft->addItem(itemTmp);
		listChanged("izquierda", pos);
	}

}

/// En el momento en que se ingresa una ayuda a la lista de las ayudas asignadas el va a revisar si el tipo de ayuda a asignar no esta en el espacio. En el caso que no se encuentre Tomara la ayuda de la izquierda a la derecha. En caso contrario Sacara un mensaje de error.
void ADListSelect::addItemToRight()
{
	int pos = listWidgetLeft->currentRow();
	QListWidgetItem *itemTmp = listWidgetLeft->takeItem(pos);
	
	if(itemTmp)
	{
		if(!check("derecha", itemTmp))
		{
			listWidgetRight->addItem(itemTmp);
			listChanged("derecha", pos);
		}
		else
		{
			listWidgetLeft->insertItem(pos, itemTmp);
			QMessageBox::information ( 0 , "Error", "Este espacio ya cuenta con una ayuda de ese tipo,\nsi desea agregar esta ayuda elimine la otra de la lista de ayudas de el espacio", 0);
		}
	}
}


/// Cuando se va a hacer una insercion a la lista de ayudas asignadas se necesita mirar que el tipo de la ayuda no este asignado ya, este metodo se encarga de verificar eso, que el tipo no se encuentre repetido en la lista de ayudas asignadas.
bool ADListSelect::check(const QString& lista, const QListWidgetItem *item)
{
	bool find=false;
	QStringList stringlist;

	stringlist = takeList("derecha");
	find = stringlist.contains(item->text());

	return find;
}



void ADListSelect::addListToLeft(const QStringList &list)
{	
	listWidgetLeft->clear();
	listWidgetLeft->addItems(list);
}


void ADListSelect::addListToRight(const QStringList &list)
{
	listWidgetRight->clear();
	listWidgetRight->addItems(list);
}


void ADListSelect::clearLists()
{
	listWidgetRight->clear();
}

/// TakeList devuelve una lista de strings de los tipos de ayudas que hay en una lista.
QStringList ADListSelect::takeList(const QString &listWidget)
{
	QStringList lista;
	if(listWidget ==("derecha"))
	{
		for(int i=0; i < listWidgetRight->count();i++)
		{
			lista << (listWidgetRight->item(i))->text();
		}
	}
	else
	{
		for(int i=0; i < listWidgetLeft->count();i++)
		{
			lista << (listWidgetLeft->item(i))->text();
		}
	}

	return lista;
}


