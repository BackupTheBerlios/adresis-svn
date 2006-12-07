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
#include "adcancellation.h"

ADCancellation::ADCancellation()
{
}

ADCancellation::ADCancellation(QString idCancellation, QString idUser, QDateTime dateCancellation, QString razonCancellation)
	: ADObject(), m_idReserveCancellation(idCancellation), m_idUserCancellation(idUser), m_dateTimeCancellation(dateCancellation), m_razonCancellation(razonCancellation)
{
	
}


ADCancellation::~ADCancellation()
{
}

QString ADCancellation::idReserveCancellation()
{
	return m_idReserveCancellation;
}

QString ADCancellation::idUserCancellation()
{
	return m_idUserCancellation;
}

QDateTime ADCancellation::dateTimeCancellation()
{
	return m_dateTimeCancellation;
}

QString ADCancellation::razonCancellation()
{
	return m_razonCancellation;
}

QDomElement ADCancellation::toXml(QDomDocument &doc) const
{
	QDomElement root = doc.createElement("cancellation");
	
	root.setAttribute( "idcancellation", m_idReserveCancellation );
	root.setAttribute( "idusercancellation", m_idUserCancellation );
	root.setAttribute( "hourcancellation", m_dateTimeCancellation.time().toString("hh:mm") );
	root.setAttribute( "datecancellation", m_dateTimeCancellation.date().toString("dd/MM/yyyy") );
	root.setAttribute( "razoncancellation", m_razonCancellation );
	return root;
}

void ADCancellation::setValues(XMLResults values)
{
	m_idReserveCancellation = values["idcancellation"];
	m_idUserCancellation = values["idusercancellation"];
	m_dateTimeCancellation = QDateTime( QDate::fromString( values["datecancellation"]), QTime::fromString(values["hourcancellation"]));
	m_razonCancellation = values["razoncancellation"];
	
	m_valid = true;
}

bool ADCancellation::isValid() const
{
	return m_valid;
}

void ADCancellation::setRazonCancellation( QString r)
{
	m_razonCancellation = r;
}



