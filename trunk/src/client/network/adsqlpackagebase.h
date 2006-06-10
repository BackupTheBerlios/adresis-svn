/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@gmail.com                                                      *
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

#ifndef CSQLPACKAGEBASE_H
#define CSQLPACKAGEBASE_H

#include <QDomDocument>
#include <QStringList>

/**
 * Esta clase solo puede ser usada mediante el mecanismo de herencia
 * @author David Cuadrado <krawek@gmail.com>
*/

class ADSqlPackageBase : public QDomDocument
{
	public:
		virtual ~ADSqlPackageBase();
	protected:
		enum Order
		{
			NoOrder = 0,
			Asc = 1,
			Desc = 0
		};
		ADSqlPackageBase();
		
		void addTable(const QString &table, const QStringList &fields = QStringList() );
		void addTable(const QString &table, const QStringList &fields, const QStringList &values );
		
	public:
		void setWhere(const QString &where);
		void setCondition(const QString &condition);
		void setOrderBy(const QString &field, Order o);

};

#endif


