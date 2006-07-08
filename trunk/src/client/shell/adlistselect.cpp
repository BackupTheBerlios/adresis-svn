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
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QListWidgetItem>
#include <QGridLayout>
#include <QCheckBox>
#include <QDebug>
#include <QMimeData>
#include <QDrag>
#include <ddebug.h>
#include <global.h>

ADListSelect::ADListSelect(QWidget *parent)
 : QWidget(parent)
{
	QHBoxLayout *hBLayout = new QHBoxLayout(this);
	QVBoxLayout *vBLayout = new QVBoxLayout(this);
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
	
	hBLayout->addWidget(listWidgetLeft);
	
	vBLayout->addWidget(qTButtonRight);
	vBLayout->addWidget(qTButtonLeft);
	hBLayout->addLayout(vBLayout);
	
	hBLayout->addWidget(listWidgetRight);
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
		listWidgetLeft->sortItems(Qt::AscendingOrder);
		listWidgetRight->sortItems(Qt::AscendingOrder);
	}
}

void ADListSelect::addItemToRight()
{
	int pos = listWidgetLeft->currentRow();
	QListWidgetItem *itemTmp = listWidgetLeft->takeItem(pos);
	if(itemTmp)
	{
		listWidgetRight->addItem(itemTmp);
		listWidgetRight->sortItems(Qt::AscendingOrder);
		listWidgetLeft->sortItems(Qt::AscendingOrder);
	}
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


QStringList ADListSelect::takeList()
{
	QStringList lista;
	for(int i=0; i < listWidgetRight->count();i++)
	{
		lista << (listWidgetRight->item(i))->text();
	}
	dDebug() << "tamaño "<< lista.count();
	for(int i=0; i< lista.count();i++)
	{
		dDebug() << lista.at(i);
	}
	return lista;
}


