/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@gmail.com                                           	   *
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

#include "sresultset.h"
#include <ddebug.h>

SResultSet::SResultSet() : QDomDocument()
{
	QDomProcessingInstruction cabecera = this->createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
	this->appendChild(cabecera);
	
	QDomElement raiz = this->createElement( "Results" );
	this->appendChild( raiz );
}


SResultSet::~SResultSet()
{
}

void SResultSet::setRecord(const QStringList &fields, const QStringList &results)
{
	QDomElement recordElement = this->createElement("Record");
	
	int counter = 0;
	foreach(QString field, fields)
	{
		QDomElement fieldElement = this->createElement("field"); //field.remove('(').remove(')'));
		fieldElement.setAttribute("name", field);
		fieldElement.setAttribute("value", results[counter]);
		
		recordElement.appendChild(fieldElement);
		
		if ( !m_map.contains( field ) )
		{
			m_map.insert(field, QStringList() << results[counter]);
		}
		else
		{
			m_map[field] << results[counter];
		}
		
		if ( !m_map.contains( field ) )
		{
			m_map.insert(field, QStringList() << results[counter]);
		}
		else
		{
			m_map[field] << results[counter];
		}
	}
	
	documentElement().appendChild(recordElement);
}

void SResultSet::setRecord(const QStringList &fields, const QList<QVariant> &results)
{
	QDomElement recordElement = this->createElement("Record");
	
	int counter = 0;
	foreach(QString field, fields)
	{
		QDomElement fieldElement = this->createElement("field"); //field.remove('(').remove(')'));
		fieldElement.setAttribute("name", field);
		fieldElement.setAttribute("value", results[counter].toString());
		
		recordElement.appendChild(fieldElement);
		
		if ( !m_map.contains( field ) )
		{
			m_map.insert(field, QStringList() << results[counter].toString());
		}
		else
		{
			m_map[field] << results[counter].toString();
		}
		
		counter++;
	}
	
	documentElement().appendChild(recordElement);
}

QMap<QString, QStringList> SResultSet::map() const
{
	return m_map;
}

bool SResultSet::isValid() const
{
	return !m_map.isEmpty();
}
