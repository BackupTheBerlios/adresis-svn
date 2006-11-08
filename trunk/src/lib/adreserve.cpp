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
/*
ADInsertPackage ADReserve::insertPackage()
{
	ADInsertPackage insert("",QStringList(),QStringList());
	return insert;
}

ADInsertPackage ADReserve::insertPackage(const QString& table)
{
	QString strIsActive;
	if(m_isactive)
	{
		strIsActive="true";
	}
	else
	{
		strIsActive="false";
	}
	
	ADInsertPackage insert(table, QStringList() << "typereserve" << "iduserreserve" << "iduserresponsable" << "idresource" << "day" << "beginhour" << "endhour" << "begindate" << "enddate" << "isactive" << "destinationreserve", QStringList() << SQLSTR(m_typereserve) << SQLSTR(m_iduserreserve) << SQLSTR(m_iduserresponsable) << SQLSTR(m_idresource) << SQLSTR(m_day)<< SQLSTR(m_beginhour) << SQLSTR(m_endhour) << SQLSTR(m_begindate) << SQLSTR(m_enddate) << strIsActive << SQLSTR(m_destinationreserve));
	
	dDebug() << insert.toString();
	return insert;
}


ADUpdatePackage ADReserve::updatePackage()
{
// 	ADUpdatePackage update("ad"+tipo+"reserve", QStringList() << "typeReserve" << "iduser" << "idresource" << "beginhour" << "endhour" << "begindate" << "enddate" << "destinationreserve", QStringList() << SQLSTR(m_typereserve) << SQLSTR(m_iduser) << SQLSTR(m_idresource) << SQLSTR(m_beginhour) << SQLSTR(m_endhour) << SQLSTR(m_begindate) << SQLSTR(m_enddate) << SQLSTR(m_destinationreserve));
	
	ADUpdatePackage update("", QStringList(), QStringList());
	dDebug() << update.toString();
	return update;
}
*/
QString ADReserve::toXml() const
{
	
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
