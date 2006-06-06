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

#include "adsqlpackagebase.h"

ADSqlPackageBase::ADSqlPackageBase() : QDomDocument()
{
}


ADSqlPackageBase::~ADSqlPackageBase()
{
}

void ADSqlPackageBase::addTable(const QString &table, const QStringList &fields )
{
	QDomElement tableElement = createElement("table");
	tableElement.setAttribute( "id", table);
	
	foreach(QString field, fields)
	{
		QDomElement fieldElement = createElement("field");
		fieldElement.setAttribute("id", field);
		
		tableElement.appendChild( fieldElement );
	}
	
	documentElement().appendChild(tableElement);
}

void ADSqlPackageBase::addTable(const QString &table, const QStringList &fields, const QStringList &values ) // FIXME: eliminar esta funcion y ponerl la funcionalidad en la funcion de arriba
{
	QDomElement tableElement = createElement("table");
	tableElement.setAttribute( "id", table);
	
	QStringList::const_iterator fieldsIt = fields.begin();
	QStringList::const_iterator valuesIt = values.begin();
	
	while(fieldsIt != fields.end() && valuesIt != values.end() )
	{
		QDomElement fieldElement = createElement("field");
		fieldElement.setAttribute("id", *fieldsIt);
		fieldElement.setAttribute("value", *valuesIt );
		
		tableElement.appendChild( fieldElement );
		
		++fieldsIt;
		++valuesIt;
	}
	
	documentElement().appendChild(tableElement);
}



void ADSqlPackageBase::setWhere(const QString &where)
{
	QDomElement whereElement = createElement("where");
	
	QDomText text = createTextNode( where );
	
	whereElement.appendChild(text);
	
	documentElement().appendChild(whereElement);
}

void ADSqlPackageBase::setCondition(const QString &condition)
{
	QDomElement conditionElement = createElement("condition");
	
	QDomText text = createTextNode( condition );
	
	conditionElement.appendChild(text);
	
	documentElement().appendChild(conditionElement);
}

void ADSqlPackageBase::setOrderBy(const QString &field, Order o)
{
	QDomElement order = createElement("orderBy");
	order.setAttribute("order", QString::number(o));
	
	QDomText text = createTextNode( field );
	
	order.appendChild(text);
	
	documentElement().appendChild(order);
}



