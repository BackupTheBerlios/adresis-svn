/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado             krawek@gmail.com     *
 *                         Jorge Cuadrado             kuadrosxx@gmail.com  *
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

#include "adupdatepackage.h"

ADUpdatePackage::ADUpdatePackage(const QString &table, const QStringList &fields, const QStringList &values) : ADSqlPackageBase()
{
	QDomElement root = createElement("Update");
	
	appendChild( root );
	addTable( table, fields, values);
}


ADUpdatePackage::ADUpdatePackage(const QStringList &tables, const QList<QStringList> &fields, const QList<QStringList> &values) : ADSqlPackageBase()
{
	QDomElement root = createElement("Update");
	
	appendChild( root );
	
	int count = 0;
	foreach(QString table, tables )
	{
		addTable(table, fields[count], values[count]);
		
		++count;
	}
}


ADUpdatePackage::~ADUpdatePackage()
{
}


