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

ADReport::ADReport( const QString & creator, TypeConsult consult, TypeReport type, const QDate &beginDate, const QDate &endDate, const QDateTime & created ) : m_creator(creator), m_consult(consult), m_type(type), m_beginDate(beginDate), m_endDate(endDate), m_created(created)
{
	
	
}


ADReport::~ADReport()
{
}

QDomElement ADReport::toXml(QDomDocument &doc) const
{
	QDomElement root = doc.createElement("report");
	root.setAttribute( "creator", m_creator);
	root.setAttribute( "type", m_type );
	root.setAttribute( "consult", m_consult );
	root.setAttribute( "beginDate",m_beginDate.toString(Qt::ISODate)  );
	root.setAttribute( "endDate", m_endDate.toString(Qt::ISODate));
	root.setAttribute( "createdDate", m_created.toString(Qt::ISODate) );
	root.setAttribute( "content", m_content );
	return root;
}

void ADReport::setValues(XMLResults values)
{
}

bool ADReport::isValid()  const
{
	return true;
}


QDateTime ADReport::created() const
{
	return m_created;
}

QDate ADReport::beginDate() const
{
	return  m_beginDate;
}

QDate ADReport::endDate() const
{
	return m_endDate;
}

QString ADReport::creator() const
{
	return m_creator;
}


QString ADReport::content() const
{
	return m_content;
}

void ADReport::setContent(const QString & content)
{
	m_content = content;
}

int ADReport::type()
{
	return m_type;
}


QString ADReport::typeStr() const
{
	QString str;
	if(m_type == List)
	{
		str = "Listado";
	}
	else if(m_type == Histogram)
	{
		str = "Grafica";
	}
	return str;
}

int ADReport::consult()
{
	return m_consult;
}

QString ADReport::consultStr() const
{
	QString str;
	if(m_consult == TimeAudio)
	{
		str = "Tiempos de ayudas audiovisuales";
	}
	else if(m_consult == TimeProfesor)
	{
		str = "Tiempos de reservas por profesor";
	}
	else if(m_consult == Cancelations)
	{
		str = "Cancelaciones";
	}
	return str;
}

