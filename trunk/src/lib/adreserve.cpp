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
#include "adreserve.h"
#include <ddebug.h>

ADReserve::ADReserve()
	:ADObject()
{
	m_valid = false;
}


ADReserve::ADReserve(const QString& typeReserve, const QString& idUserReserve, const QString& idUserResponsable, const QString& idAudiovisual, const QString& idSpace, const QString& day, const QString& beginHour, const QString& endHour, const QString& beginDate, const QString& endDate, const bool isActive, const QString& destinationreserve)
	: ADObject(), m_typereserve(typeReserve), m_iduserreserve(idUserReserve), m_iduserresponsable(idUserResponsable), m_idaudiovisual (idAudiovisual ), m_idspace( idSpace ) , m_day(day), m_beginhour(beginHour), m_endhour(endHour), m_begindate(beginDate), m_enddate(endDate), m_isactive(isActive), m_destinationreserve(destinationreserve)
{
	m_valid = true;
}


ADReserve::~ADReserve()
{
}

QDomElement ADReserve::toXml(QDomDocument &doc)
{
	QDomElement root = doc.createElement("space");
	/*
	idreserve | typereserve | iduserreserve | iduserresponsable | idaudiovisual | idspace | day | beginhour | endhour | begindate | enddate | isactive | destinationreserve

	*/
	root.setAttribute( "typereserve", m_typereserve );
	root.setAttribute( "iduserreserve", m_iduserreserve );
	root.setAttribute( "iduserresponsable", m_iduserresponsable );
	root.setAttribute( "idaudiovisual", m_idaudiovisual );
	root.setAttribute( "idspace", m_idspace );
	root.setAttribute( "day", m_day );
	root.setAttribute( "beginhour", m_beginhour );
	root.setAttribute( "endhour", m_endhour );
	root.setAttribute( "begindate", m_begindate );
	root.setAttribute( "enddate", m_enddate );
	
	if(m_isactive)
	{
		root.setAttribute( "isactive", "true");
	}
	else
	{
		root.setAttribute( "isactive", "false");
	}
	root.setAttribute( "destinationreserve", m_destinationreserve );
	
	return root;
}

QString ADReserve::toXml() const
{
	return "";
}

void ADReserve::fromXml(const QString & xml )
{
	
}

void ADReserve::setValues(XMLResults values)
{
	dDebug() << "here adspace";
	m_typereserve = values["typeReserve"];
	m_iduserreserve = values["iduserreserve"];
	m_iduserresponsable = values["iduserresponsable"];
	m_idaudiovisual = values["idaudiovisual"];
	m_idspace = values["idspace"];
	m_day = values["day"];
	m_beginhour = values["beginhour"];
	m_endhour = values["endhour"];
	m_begindate = values["begindate"];
	m_enddate = values["enddate"];
	
	if(values["isactive"] == "true")
	{
		m_isactive = true;
	}
	else
	{
		m_isactive = false;
	}
	m_destinationreserve = values["destinationreserve"];
	
	m_valid = true;
}

bool ADReserve::isValid()const
{
	return m_valid;
}

QString ADReserve::typeReserve()const
{
	return m_typereserve;
}

QString ADReserve::iduserreserve()const
{
	return m_iduserreserve;
}

QString ADReserve::iduserresponsable()const
{
	return m_iduserresponsable;
}

QString ADReserve::idaudiovisual()const
{
	return m_idaudiovisual;
}

QString ADReserve::idspace()const
{
	return m_idspace;
}

QString ADReserve::day()const
{
	return m_day;
}

QString ADReserve::beginhour()const
{
	return m_beginhour;
}

QString ADReserve::endhour()const
{
	return m_endhour;
}

QString ADReserve::begindate()const
{
	return m_begindate;
}

QString ADReserve::enddate()const
{
	return m_enddate;
}

bool ADReserve::isActive()const
{
	return m_isactive;
}

QString ADReserve::destinationreserve()const
{
	return m_destinationreserve;
}
