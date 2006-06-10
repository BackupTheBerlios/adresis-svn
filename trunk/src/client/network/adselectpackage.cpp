/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@gmail.com                                                       *
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

#include "adselectpackage.h"
#include "ddebug.h"

ADSelectPackage::ADSelectPackage(const QStringList &tables, const QStringList &fields, bool distinct) : ADSqlPackageBase()
{
	QDomElement root = createElement("Select");
	
	if (distinct)
	{
		root.setAttribute( "distinct", QString::number(1) );
	}
	else
	{
		root.setAttribute( "distinct", QString::number(0) );
	}
	
	appendChild( root );
	
	QString table;
	
	QStringList::const_iterator it = tables.begin();
	
	while(it != tables.end())
	{
		if ( it+1 == tables.end() )
		{
			table += *it;
		}
		else
		{
			table += *it + ",";
		}
		
		++it;
	}
	
	addTable( table, fields );
}


ADSelectPackage::~ADSelectPackage()
{
}


