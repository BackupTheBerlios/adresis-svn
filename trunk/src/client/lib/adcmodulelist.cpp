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
#include "adcmodulelist.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QHeaderView>
#include <QTimer>


#include <dtreewidgetsearchline.h>

#include "global.h"
#include <ddebug.h>

ADCModuleList::ADCModuleList(const QString& moduleName, const QStringList& titles, QWidget *parent )
	:QWidget(parent)
{
	QVBoxLayout *layout = new QVBoxLayout(this);
	setLayout(layout);
	QHBoxLayout *search = new QHBoxLayout;
	
	QToolButton *button = new QToolButton;
	button->setIcon( QIcon(THEME_DIR+"/icons/clear_right.png"));
	
	
	search->addWidget(button);
	m_pTree = new  QTreeWidget;
// 	m_pTree = new DTreeListWidget;
	
	m_pTree->setHeaderLabels ( titles );
	m_pTree->header()->show();
	m_pSearch = new DTreeWidgetSearchLine(this, m_pTree);
	
	
	search->addWidget( m_pSearch );
	
	m_pSearch->setSearchColumns(QList<int>() << 0 );
	
	layout->addLayout(search);
	
	layout->addWidget(m_pTree);
	
	
	connect(button, SIGNAL(clicked()), m_pSearch, SLOT(clear()));
// 	connect(button, SIGNAL(clicked()), this, SLOT(fill()));
	
// 	m_pTree->setEditable(false);
	
	setup(titles);
}


ADCModuleList::~ADCModuleList()
{
	
}

ADModuleButtonBar *ADCModuleList::addButtonBar(int flags)
{
	ADModuleButtonBar *bar = new ADModuleButtonBar(flags);
	
	boxLayout()->addWidget( bar );
	return bar;
}

QBoxLayout *ADCModuleList::boxLayout()
{
	return qobject_cast<QVBoxLayout *>(layout());
}

void ADCModuleList::addItem(const QStringList &cols)
{
	QTreeWidgetItem *item = new QTreeWidgetItem(m_pTree);
	int count = 0;
	foreach(QString str, cols)
	{
		item->setText(count, str);
		count++;
	}
}

void ADCModuleList::setup(const QStringList& headers )
{
	D_FUNCINFO;
	
	// TODO: guardar los campos de la BD
	
// 	QStringList headers;
// 	QStringList tables, attributes;
// 	QString where;
// 	QTreeWidgetItem *data = new QTreeWidgetItem();
	
// 	int count = 0;
// 	foreach(QString info,headers)
// 	{
// 		data->setText(count, info);
// // 		data->setData(count, Field, info);
// 		
// // 		headers << info.first;
// 		++count;
// 	}
// 	
// 	m_pTree->setHeaderLabels(headers);
// 	m_pTree->setHeaderItem(data);
// 	
// 	m_pTree->header()->show();
// 	
// 	m_pModuleInfo = module;
// 	
// 	QTimer::singleShot(500, this, SLOT(fill()));
}




