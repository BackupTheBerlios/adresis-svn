/***************************************************************************
 *   Copyright (C) 2006 by Jorge Cuadrado   *
 *   kuadrosx@gmail.com                                                    *
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
#ifndef ADCMODULELIST_H
#define ADCMODULELIST_H


#include <qwidget.h>

#include <QTreeWidget>
#include <dtreelistwidget.h>

#include "admodulebuttonbar.h"
#include "global.h"


class DTreeWidgetSearchLine;
class QBoxLayout;
/**
 * @author Jorge Cuadrado <kuadrosxx@gmail.com>
 */

class ADCModuleList: public QWidget
{
	Q_OBJECT
	public:
		ADCModuleList(const QString& moduleName, const QStringList& list, QWidget *parent );
		virtual ~ADCModuleList();
		virtual void fill( const QList<XMLResults>&results) = 0;
		virtual void clean() = 0;
		ADModuleButtonBar *addButtonBar(int flags);
		QBoxLayout *boxLayout();
		void addItem(const QStringList &cols);
		
		
	protected slots:
		virtual void requestAction(int action) = 0;
		
	protected:
		QTreeWidget *m_pTree;
		DTreeWidgetSearchLine *m_pSearch;
		
};

#endif
