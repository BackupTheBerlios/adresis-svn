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
#include "adreport.h"

ADReport::ADReport(TypeConsult consult, TypeReport type, const QDate &beginDate, const QDate &endDate  ) : m_consult(consult), m_type(type), m_beginDate(beginDate), m_endDate(endDate)
{
	m_created = QDateTime::currentDateTime();
	
}


ADReport::~ADReport()
{
}

QDomElement ADReport::toXml(QDomDocument &doc) const
{
	QDomElement root = doc.createElement("report");
	root.setAttribute( "type", m_type );
	root.setAttribute( "consult", m_consult );
	root.setAttribute( "beginDate",m_beginDate.toString(Qt::ISODate)  );
	root.setAttribute( "endDate", m_endDate.toString(Qt::ISODate));
	root.setAttribute( "createdDate", m_created.toString(Qt::ISODate) );
	return root;
}

void ADReport::setValues(XMLResults values)
{
}

bool ADReport::isValid()  const
{
	return true;
}
