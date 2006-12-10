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

#include <ddebug.h>
#include <dtreewidgetsearchline.h>

#include "aduser.h"
#include "adaudiovisual.h"
#include "adspace.h"
#include "adreserve.h"
#include "global.h"
#include "adevent.h"
#include "adcancellation.h"
#include "adreport.h"


ADCModuleList::ADCModuleList(Logic::Module module, QWidget *parent )
	:QWidget(parent), m_pModule(module)
{
	QVBoxLayout *layout = new QVBoxLayout(this);
	setLayout(layout);
	QHBoxLayout *search = new QHBoxLayout;
	
	QToolButton *button = new QToolButton;
	button->setIcon( QIcon(THEME_DIR+"/icons/clear_right.png"));
	
	
	search->addWidget(button);
	m_pTree = new  QTreeWidget;
// 	m_pTree = new DTreeListWidget;
	QStringList titles;
	switch(m_pModule)
	{
		case Logic::Users:
		{
			titles << tr("Codigo")  <<  tr("Nombre") << tr("Login");
			setWindowTitle ( "Usuarios");
		}
		break;
		case Logic::Audiovisuals:
		{
			titles << tr("No Inventario") << tr("Estado") << tr("Tipo") << tr("Marca")   << tr("Espacio");
			setWindowTitle ( "Audiovisuales");
		}
		break;
		case Logic::ReservesF:
		{
// typereserve | iduserreserve | iduserresponsable | idaudiovisual | idspace | day | beginhour | endhour | begindate | enddate | isactive | destinationreserve
			
			titles << tr("Id") << tr("Tipo Reserva") << tr("Usuario Reserva") << tr("Usuario Responsable ") << tr("Id Recurso") << tr("dia");
			setWindowTitle ( "Reservas Semestrales");
		}
		break;
		case Logic::ReservesT:
		{
// typereserve | iduserreserve | iduserresponsable | idaudiovisual | idspace | day | beginhour | endhour | begindate | enddate | isactive | destinationreserve
			
			titles << tr("Id") << tr("Tipo Reserva") << tr("Usuario Reserva") << tr("Usuario Responsable ") << tr("Id Recurso") << tr("dia");
			setWindowTitle ( "Reservas Temporales");
		}
		break;
		case Logic::Spaces:
		{
			titles << tr("Codigo") << tr("Tipo") << tr("Aire Acondicionado") << tr("Capacidad") << tr("Nombre");
			setWindowTitle ( "Espacios");
		}
		break;
		case Logic::Reports:
		{
			setWindowTitle ( "Reportes" );
			titles << tr("Creador") << tr("Creado") <<tr("Tipo") << tr("Consulta");
		}
		break;
	}
	
	
	m_pTree->setHeaderLabels ( titles );	
	for(int i = 0; i < m_pTree->columnCount (); i++)
	{
		m_pTree->resizeColumnToContents ( i );
	}
	m_pTree->header()->show();
	m_pSearch = new DTreeWidgetSearchLine(this, m_pTree);
	search->addWidget( m_pSearch );
	layout->addLayout(search);
	layout->addWidget(m_pTree);
	
	connect(button, SIGNAL(clicked()), m_pSearch, SLOT(clear()));
	ADModuleButtonBar *buttonBar = addButtonBar( ADModuleButtonBar::Add | ADModuleButtonBar::Del | ADModuleButtonBar::Modify );
	connect(buttonBar, SIGNAL(buttonClicked( int )), this, SLOT(requestAction(int)));
}

ADCModuleList::~ADCModuleList()
{
	
}

void ADCModuleList::fill( const QList<QVariant> list)
{
	
	foreach(QVariant data, list)
	{
		addData( m_pModule, data);
	}
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

void ADCModuleList::requestAction(int action)
{
	switch(ADModuleButtonBar::Button(action))
	{
		case ADModuleButtonBar::Add:
		{
			emit requestShowForm(m_pModule);
		}
		break;
		case ADModuleButtonBar::Del:
		{
			//Confirmar si desea borrar
			QTreeWidgetItem *current = m_pTree->currentItem();
			if(current)
			{
				QString key = current-> text ( 0 );
				ADEvent event(ADEvent::Client, m_pModule, Logic::Del, key );
				emit sendEvent(&event);
			}
		}
		break;
		case ADModuleButtonBar::Modify:
		{
			QTreeWidgetItem *current = m_pTree->currentItem();
			if(current)
			{
				QString key = current-> text ( 0 );
				emit requestShowForm(m_pModule, key);
			}
		}
		break;
	};
}

void ADCModuleList::clean()
{
	
}

void ADCModuleList::addData(Logic::Module module, const QVariant & data )
{
	D_FUNCINFO << module;
	if(module == m_pModule)
	{
		switch(module)
		{
			case Logic::Users:
			{
				ADUser *user = qVariantValue<ADUser *>(data);
				QList<QString> strs;
				strs << user->code();
				strs << user->name();
				strs << user->login();
				QTreeWidgetItem *item = new QTreeWidgetItem(m_pTree);
				int count = 0;
				foreach(QString str, strs)
				{
					item->setText(count, str);
					count++;
				}
			}
			break;
			case Logic::Audiovisuals:
			{
				ADAudioVisual *audiovisual = qVariantValue<ADAudioVisual *>(data);
				QList<QString> strs;
				
				QString code =  audiovisual->codeSpace();
				if(code == "null")
				{
					code = tr("No signado");
				}
				
				strs << audiovisual->numberInventory()  << audiovisual->state() <<  audiovisual->type() << audiovisual->marksEquipment() << code;
				
				QTreeWidgetItem *item = new QTreeWidgetItem(m_pTree);
				int count = 0;
				foreach(QString str, strs)
				{
					item->setText(count, str);
					count++;
				}
			}
			break;
			case Logic::ReservesF:
			{
				ADReserve *reserve = qVariantValue<ADReserve *>(data);
				QList<QString> strs;
				strs << reserve->idReserve(); //Identficador de la reserve
				strs << reserve->typeReserve();
				strs << reserve->iduserreserve();
				strs << reserve->iduserresponsable();
				if( reserve->idspace() != "" )
				{
					strs << reserve->idspace();
				}
				else
				{
					strs << reserve->idaudiovisual();
				}
				strs << reserve->day();
	
				QTreeWidgetItem *item = new QTreeWidgetItem(m_pTree);
				int count = 0;
				foreach(QString str, strs)
				{
					item->setText(count, str);
					count++;
				}
			}
			break;
			case Logic::ReservesT:
			{
				ADReserve *reserve = qVariantValue<ADReserve *>(data);
				QList<QString> strs;
				strs << reserve->idReserve();
				strs << reserve->typeReserve();
				strs << reserve->iduserreserve();
				strs << reserve->iduserresponsable();
				if( reserve->idspace() != "" )
				{
					strs << reserve->idspace();
				}
				else
				{
					strs << reserve->idaudiovisual();
				}
				strs << reserve->day();
	
				QTreeWidgetItem *item = new QTreeWidgetItem(m_pTree);
				int count = 0;
				foreach(QString str, strs)
				{
					item->setText(count, str);
					count++;
				}
			}
			break;
			case Logic::Spaces:
			{
				ADSpace *space = qVariantValue<ADSpace *>(data);
				if( (space->codeSpace()) != "null" )
				{
					QList<QString> strs;	
					strs << space->codeSpace(); 
					strs << space->typeSpace();
					if(space->coolAirSpace())
					{
						strs << tr("Yes");
					}
					else
					{
						strs << tr("No");
					}
					strs << space->capacitySpace();
					strs << space->nameSpace();
					
					QTreeWidgetItem *item = new QTreeWidgetItem(m_pTree);
					int count = 0;
					foreach(QString str, strs)
					{
						item->setText(count, str);
						count++;
					}
				}
			}
			break;
			case Logic::Reports:
			{
				ADReport *report = qVariantValue<ADReport *>(data);
				QList<QString> strs;
				
				
				strs << report->creator() << report->created().toString(Qt::ISODate) << report->typeStr() << report->consultStr();
				
				QTreeWidgetItem *item = new QTreeWidgetItem(m_pTree);
				int count = 0;
				foreach(QString str, strs)
				{
					item->setText(count, str);
					count++;
				}
				
			}
			break;
			case Logic::Cancellation:
			{
// 				idReserveCancellation(); idUserCancellation(); dateTimeCancellation(); razonCancellation();
				ADCancellation *cancel = qVariantValue<ADCancellation *>(data);
				QList<QString> strs;
				strs << cancel->idReserveCancellation();
				strs << cancel->idUserCancellation();
				strs << cancel->dateTimeCancellation().date().toString("dd/MM/yyyy");
				strs << cancel->dateTimeCancellation().time().toString("hh:mm");
				QTreeWidgetItem *item = new QTreeWidgetItem(m_pTree);
				int count = 0;
				foreach(QString str, strs)
				{
					item->setText(count, str);
					count++;
				}
				
			}
			break;
		}
	}
	for(int i = 0; i < m_pTree->columnCount (); i++)
	{
		m_pTree->resizeColumnToContents ( i );
	}
}

void ADCModuleList::removeData(Logic::Module module, const QString & key )
{
	if(module == m_pModule)
	{
		QTreeWidgetItemIterator it(m_pTree);
		while( (*it) )
		{
			if( key == (*it)->text(0) )
			{
				delete (*it);
				break;
			}
			++it;
		}
		for(int i = 0; i < m_pTree->columnCount (); i++)
		{
			m_pTree->resizeColumnToContents ( i );
		}
	}
}

void ADCModuleList::updateData(Logic::Module module, const QVariant & data )
{
	D_FUNCINFO;
	switch(module)
	{
		case Logic::Users:
		{
			ADUser * u = qvariant_cast<ADUser *>(data);
			if (u)
			{
				removeData(module, u->code() );
			}
		}
		break;
		case Logic::Audiovisuals:
		{
			ADAudioVisual * a = qvariant_cast<ADAudioVisual *>(data);
			if (a)
			{
				removeData(module, a->numberInventory() );
			}
		}
		break;
		case Logic::Spaces:
		{
			ADSpace * s = qvariant_cast<ADSpace *>(data);
			if (s)
			{
				removeData(module, s->codeSpace() );
			}
		}
		break;
		case Logic::ReservesF:
		{
			ADReserve * r = qvariant_cast<ADReserve *>(data);
			if (r)
			{
				removeData(module, r->idReserve() );
			}
		}
		break;
		case Logic::ReservesT:
		{
			ADReserve * r = qvariant_cast<ADReserve *>(data);
			if (r)
			{
				removeData(module, r->idReserve() );
			}
		}
		break;
	}
	addData(module, data);
}
