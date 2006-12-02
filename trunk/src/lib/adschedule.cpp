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
#include "adschedule.h"
#include <QVBoxLayout>
#include <QDate>
#include <QStringList>
#include <dconfig.h>



ADSchedule::ADSchedule(QWidget *parent) 
	: QWidget(parent)
{
	QVBoxLayout *vBLayout = new QVBoxLayout(this);
	
	months << tr("ENERO") << tr("FEBRERO") << tr("MARZO") << tr("ABRIL") << tr("MAYO") << tr("JUNIO") << tr("JULIO") << tr("AGOSTO") << tr("SEPTIEMBRE") << tr("OCTUBRE") << tr("NOVIEMBRE") << tr("DICIEMBRE");

	month = new QComboBox;
	month->addItems(months);
	month->setHidden(true);
	vBLayout->addWidget( month );

	connect(month, SIGNAL(activated (int) ), this , SLOT( modifyMonthReserve( int )));

	m_table = new QTableWidget(this);

	vBLayout->addWidget( m_table );
	monthNo=0;
	previousColumn=0;
	previousRow=0;
	connect( m_table, SIGNAL( cellPressed( int, int ) ), this,  SLOT( valiteColumn( int,int )));
}


ADSchedule::~ADSchedule()
{
}

void ADSchedule::makeTable( bool semestral)
{
	rows = QStringList() << "07:00" << "07:30" <<"08:00" << "08:30" << "09:00" << "09:30" << "10:00" << "10:30" << "11:00" << "11:30" << "12:00" << "12:30" << "13:00" << "13:30" << "14:00" << "14:30" << "15:00" << "15:30" << "16:00" << "16:30" << "17:00" << "17:30" << "18:00" << "18:30" << "19:00" << "19:30" << "20:00" << "20:30" << "21:00" << "21:30";

	int numCols;
	year = (QDate::currentDate()).year();
	cols.clear();

	if(semestral)
	{
		month->setHidden(true);
		cols << "LUNES" << "MARTES" << "MIERCOLES" << "JUEVES" << "VIERNES" << "SABADO"<< "DOMINGO";
		m_table->setColumnCount(7);
	}
	else
	{
		month->setHidden(false);
		if(monthNo == 0 || monthNo == 2 || monthNo == 4 || monthNo== 6 || monthNo== 7 || monthNo == 9 || monthNo== 11)
		{
			numCols=31;
		}
		else if(monthNo == 3 || monthNo == 5 || monthNo == 8 || monthNo == 10)
		{
			numCols=30;
		}
		else if( monthNo == 1 && QDate::isLeapYear( year ) ) //Cond. de años bisiestos
		{
			numCols = 29;
		}
		else
		{
			numCols=28;
		}

		for(int n=1; n <= numCols; n++)
		{
			QString dia = QDate::longDayName( QDate( year, monthNo+1, n).dayOfWeek());
			cols << dia+"\n"+QString::number(n);
		}
		m_table->setColumnCount(numCols);
	}

	m_table->setRowCount(30);
	m_table->setHorizontalHeaderLabels(cols);
	m_table->setVerticalHeaderLabels(rows);

	for(int j=0; j < m_table->columnCount(); j++)
	{
		for(int i=0; i < m_table->rowCount(); i++)
		{
			m_item = new QTableWidgetItem("");
			m_item->setFlags( !Qt::ItemIsEditable );
			m_item->setFlags( !Qt::ItemIsSelectable );
			m_item->setFlags( !Qt::ItemIsEnabled );
			m_item->setTextAlignment(Qt::AlignHCenter);
			m_table->setItem(i,j, m_item);
		}
	}
}


void ADSchedule::modifyMonthReserve( int value)
{
	monthNo=value;
	makeTable( false );
	if(!reservasAnteriores.isEmpty())
	{
		m_reserve="temporal";
		fill();
	}
}

void ADSchedule::clear()
{
	dDebug() << "CLEAR CLEAR CLEAR CLEAR ";
	clearSchedule();
	reservasAnteriores.clear();
	m_points.clear();
	m_cellsReserved.clear();
}


void ADSchedule::clearSchedule()
{
	for(int i=0; i < m_table->rowCount(); i++)
	{
		for(int j=0; j < m_table->columnCount(); j++)
		{
			m_table->item( i , j )->setText("");
			m_table->item( i , j )->setBackgroundColor(Qt::white);
			m_table->item( i , j )->setToolTip( "" );
		}
	}
}


void ADSchedule::receiveReserves( const QList<ADReserve *>& results )
{
	reservasAnteriores = results;
}


void ADSchedule::valiteColumn( int currentRow, int currentColumn)
{
	bool descolorearItems=false;
	bool colorearItem=false;
	QList< QPair<int,int> >::const_iterator point = m_points.begin();
	
	///Este IF es por si se quiere desseleccionar un item ya seleccionado
	if( (m_table->item( currentRow , currentColumn)->text()) ==("RESERVAR") )
	{
		(m_table->item( currentRow , currentColumn))->setText("");
		(m_table->item( currentRow , currentColumn))->setBackgroundColor(Qt::white);
		(m_table->item( currentRow , currentColumn))->setToolTip( "" );

		m_points.removeAll(qMakePair(currentRow, currentColumn));
	}



	/// En este else if se maneja si se cambia de una columna a otra, cuando se cambia de columna se deben deshabilitar los anteriores.
	else if( currentColumn != previousColumn )
	{
		descolorearItems=true;

		if(!m_cellsReserved.contains(qMakePair(currentRow, currentColumn)))
		{
			colorearItem = true;
		}
	}

	/// En este else if se maneja el caso que se seleccione una celda de una misma columna lo que se hace es modificar el item.
	else if(currentColumn == previousColumn && !m_cellsReserved.contains(qMakePair(currentRow, currentColumn)))
	{
		colorearItem = true;
	}


	if(descolorearItems)
	{
		while( point != m_points.end() )
		{
			m_table->item( (*point).first , (*point).second )->setText("");
			m_table->item( (*point).first , (*point).second )->setBackgroundColor(Qt::white);
			m_table->item( (*point).first , (*point).second )->setToolTip( "" );
			point++;
		}
		m_points.clear();
		
	}
	
	if(colorearItem)
	{
		(m_table->item( currentRow , currentColumn))->setText("RESERVAR");
 		(m_table->item( currentRow , currentColumn))->setBackgroundColor( QColor(Qt::yellow) );
 		(m_table->item( currentRow , currentColumn))->setToolTip( "Horario para mi reserva" );
 		m_points.append(qMakePair(currentRow, currentColumn));
	}

	previousColumn=currentColumn;
	previousRow=currentRow;
}


void ADSchedule::assignTypeReserve(const QString typeReserve, bool inserter)
{
	dDebug() << "assignTypeReserve => " << typeReserve;
	m_reserve = typeReserve;
	m_inserter = inserter;
}


void ADSchedule::fill()
{
	clearSchedule();
	QList<ADReserve *>::const_iterator it = reservasAnteriores.begin();
	QList<QTableWidgetItem *> selectedItems;
	
	int row1=0, row2=0;
	QList<int> column;
	dDebug() << "EN FILL M_RESERVE ES >> " << m_reserve;

	while( it != reservasAnteriores.end() )
	{
		dDebug() << "(*it)->typeReserve().toLower() >> " << (*it)->typeReserve().toLower() <<"   m_reserve =>"<< m_reserve;
		dDebug() << "((( (*it)->beginDateTime().date()).month())-1) >> " << ((( (*it)->beginDateTime().date()).month())-1) << "   month => " << monthNo;
		dDebug() << "((( (*it)->endDateTime().date()).month())-1) >> " << ((( (*it)->endDateTime().date()).month())-1) << "   month => " << monthNo;
		
		
		
		if( ((*it)->typeReserve().toLower()) == "semestral" && (m_reserve.toLower()) == "semestral" )
		{
			column << cols.indexOf( ( (*it)->day() ).toUpper() );
			row1 = rows.indexOf( ((*it)->beginDateTime()).toString("hh:mm") );
			row2 = rows.indexOf( ((*it)->endDateTime()).toString("hh:mm") );
			
			dDebug() << "((*it)->beginDateTime()).toString( hh:mm )" << ((*it)->beginDateTime()).toString("hh:mm") << " row1 " << row1;
			dDebug() << "((*it)->endDateTime()).toString( hh:mm )" << ((*it)->endDateTime()).toString("hh:mm")<< "  row2 " << row2;
			dDebug() << "( (*it)->day() ).toUpper()" << ( (*it)->day() ).toUpper() << "   columna " << cols.indexOf( ( (*it)->day() ).toUpper() );
		}
		
		
		else if( ((*it)->typeReserve().toLower()) == "temporal" && (m_reserve.toLower()) == "temporal" && monthNo == ((( (*it)->beginDateTime().date()).month())-1))
		{
			///En la siguiente linea, tomo el valor de el dia de la fecha de inicio ya que en una reserva temporal la fecha de inicio y la de finalizacion es la misma
			column << (( (*it)->beginDateTime().date()).day())-1;
			row1 = rows.indexOf( ((*it)->beginDateTime()).toString("hh:mm") );
			row2 = rows.indexOf( ((*it)->endDateTime()).toString("hh:mm") );
		}
		
		
		else if( ((*it)->typeReserve().toLower()) == "semestral" && (m_reserve.toLower()) ==("temporal") && monthNo >= ((( (*it)->beginDateTime().date()).month())-1)	&&	monthNo <= ((( (*it)->endDateTime().date()).month())-1))
		{
			int diaIniRes = (( (*it)->beginDateTime().date()).day())-1;
			int diaFinRes = (( (*it)->endDateTime().date()).day())-1;
		
			for(int c = 0; c < m_table->columnCount(); c++)
			{
				
				
				dDebug() << "(((*it)->day()).toUpper()" << (((*it)->day()).toUpper()) << "  (((m_table->horizontalHeaderItem(c))->text()).section('\n',0,0).toUpper()) => " << (((m_table->horizontalHeaderItem(c))->text()).section('\n',0,0).toUpper());
				
				
				
			///Este if maneja si el mes elegido y el mes inicial de la reserva son iguales, en tal caso descartara los dias que sean menores a el dia inicial de la reserva.
				if( (((m_table->horizontalHeaderItem(c))->text()).section('\n',0,0).toUpper()) == (((*it)->day()).toUpper())	&&	monthNo == ((( (*it)->beginDateTime().date()).month())-1)	&&	c >= diaIniRes )
				{
					column << c;
				}
				
			///Este else if maneja si el mes elegido y el mes final de la reserva son iguales, en tal caso descartara los dias que sean mayores a el dia final de la reserva.	
				else if( (((m_table->horizontalHeaderItem(c))->text()).section('\n',0,0).toUpper()) == (((*it)->day()).toUpper()) &&	monthNo == ((( (*it)->endDateTime().date()).month())-1)	&&	c <= diaFinRes )
				{
					column << c;
				}
				
			///Este else if maneja si el mes elegido y el mes de la reserva son diferentes, en ese caso no se descarta ningun dia.
				else if( (((m_table->horizontalHeaderItem(c))->text()).section('\n',0,0).toUpper()) == (((*it)->day()).toUpper()) &&	monthNo != ((( (*it)->beginDateTime().date()).month())-1)	&&	monthNo != ((( (*it)->endDateTime().date()).month())-1) )
				{
					column << c;
				}
			}
			
			row1 = rows.indexOf( ((*it)->beginDateTime()).toString("hh:mm") );
			row2 = rows.indexOf( ((*it)->endDateTime()).toString("hh:mm") );
		}
		
		
		
		for(int pos=0; pos < column.count(); pos++)
		{
			dDebug() << "col " << column.at(pos);
			
			for( int i =row1; i < row2; i++ )
			{
				dDebug() << "fila " << i;
				(m_table->item( i , column.at(pos)))->setText((*it)->iduserresponsable().toUpper() );
				(m_table->item( i , column.at(pos)))->setBackgroundColor( QColor(Qt::blue) );
				(m_table->item( i , column.at(pos)))->setToolTip( "Espacio ocupado en este horario" );
				
				if(m_inserter)
				{
					(m_table->item( i , column.at(pos)))->setFlags( !Qt::ItemIsEditable );
					(m_table->item( i , column.at(pos)))->setFlags( !Qt::ItemIsSelectable );
					(m_table->item( i , column.at(pos)))->setFlags( !Qt::ItemIsEnabled );
					m_cellsReserved.append( qMakePair(i,column.at(pos)) );
				}
				else
				{
					m_points.append(qMakePair(i,column.at(pos)));
				}
				
				(m_table->item( i , column.at(pos)))->setTextAlignment(Qt::AlignHCenter);
				
				
				
			}
		}
		
		column.clear();
		++it;
	}
}


void ADSchedule::organizePairs()
{
	int j;
	for(int i=1; i < m_points.size(); i++)
	{
		QPair<int, int> temp = m_points.at(i);
		j= i-1;
		 
		while( j >= 0 && (m_points.at(j)).first > temp.first)
		{
			m_points.replace( j+1, (m_points.at(j)) );
         		j--;
		}
		m_points.replace( j+1, temp );
	}
	
	

}

QList< QMap<QString, QString> > ADSchedule::returnSchedule()
{
	QString column;
	QList<QMap<QString, QString> > listSchedule;
	organizePairs();
	
	QMap<QString, QString > map;

	
	for( int point=0; point < m_points.count(); point++ )
	{
		if( (point) == 0 || (m_points.at(point).first)-1 != (m_points.at(point-1).first) )
		{
			column = ((m_table->horizontalHeaderItem(previousColumn))->text()).toLower();
			
			if((m_reserve.toLower()) ==("semestral"))
			{
				map.insert("day", column );
				map.insert("typereserve", m_reserve);
				map.insert("beginhour", ((m_table->verticalHeaderItem(m_points.at(point).first))->text()) );
				
			}
			else if ((m_reserve.toLower()) ==("temporal"))
			{
				map.insert("day", column.section('\n',0,0) );
				map.insert("typereserve", m_reserve);
				
				column = column.section('\n',1,1);
				map.insert("begindate", ""+column+"/"+QString::number(monthNo+1)+"/"+QString::number(year));
				map.insert("enddate",   ""+column+"/"+QString::number(monthNo+1)+"/"+QString::number(year));
				map.insert("beginhour", ((m_table->verticalHeaderItem(m_points.at(point).first))->text()) );
			}
		}
		
		if( (point) == (m_points.count()-1) || (m_points.at(point).first)+1 != (m_points.at(point+1).first) )
		{
			if(!(m_table->verticalHeaderItem(m_points.at(point).first+1)))
			{
				map.insert("endhour", "22:00" );
			}
			else
			{
				map.insert("endhour", ((m_table->verticalHeaderItem(m_points.at(point).first+1))->text()) );
			}
			listSchedule.append(map);
		}
	
	}
	
	return listSchedule;
}
