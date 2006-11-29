/***************************************************************************
 *   Copyright (C) 2006 by Jorge Cuadrado   *
 *   kuadrosx@zi0n   *
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
#ifndef ADSCHEDULE_H
#define ADSCHEDULE_H

#include <QWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTableWidgetSelectionRange>
#include <QGroupBox>
#include <QColor>
#include <QMimeData>
#include <QDrag>
#include <ddebug.h>
#include <global.h>
#include <QMessageBox>
#include <QComboBox>
#include <QList>
#include <QPair>
#include "adreserve.h"

		
/**
	@author Hector Fabio Cruz Mosquera,0329876 <hectorcaz@gmail.com>
*/

class ADSchedule : public QWidget
{
	Q_OBJECT
	public:
		ADSchedule(QWidget *parent=0);
		~ADSchedule();
		QList< QMap<QString, QString> > returnSchedule();
	
	
	public slots:
		void makeTable(bool semestral);
		void modifyMonthReserve(int);
		void receiveReserves( const QList<ADReserve *>& results );
		void assignTypeReserve(const QString typeReserve);
		void fill();
		void clear();
		void valiteColumn( int currentRow, int currentColumn);

	private:
		QTableWidget *m_table;
		QComboBox *month;
		QTableWidgetItem *m_item;
		QStringList cols;
		QStringList rows;
		QStringList months;
		QList<ADReserve *> reservasAnteriores;
		QList< QPair<int,int> > m_points; // Aqui se guardan las posiciones de las celdas que seleccione el usuario.
		QList< QPair<int,int> > m_cellsReserved; // Aqui se guardan las posiciones de las celdas con reservas ya hechas
		QPair<int, int> pair;
		int monthNo;
		int previousColumn;
		int previousRow;
		int year;
		QString m_reserve;
		
		void clearSchedule();
		void organizePairs();
		
		

};

#endif
