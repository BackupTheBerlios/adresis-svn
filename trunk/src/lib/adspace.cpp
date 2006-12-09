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
#include "adspace.h"
#include <ddebug.h>

		ADSpace::ADSpace() 
	: ADObject()
{
	m_valid = false;
}

ADSpace::ADSpace(const QString & codeSpace, const QString & typeSpace, const bool & coolAirSpace, const QString & capacitySpace, const QString & nameSpace)
	: ADObject(), m_codeSpace(codeSpace), m_typeSpace(typeSpace), m_coolAirSpace(coolAirSpace), m_capacitySpace(capacitySpace), m_nameSpace(nameSpace)
{
	m_valid = true;
}

ADSpace::~ADSpace()
{
}

QDomElement ADSpace::toXml(QDomDocument &doc) const
{
	QDomElement root = doc.createElement("space");
	
	QString cool;
	dDebug() << "MY COOL_AIR_SPACE ==>> " << m_coolAirSpace;
	if(m_coolAirSpace)
	{
		cool = "true";
	}
	else
	{
		cool = "false";
	}
	dDebug() << "COOL QUEDO  ==>> " << m_coolAirSpace;
	root.setAttribute( "codespace", m_codeSpace );
	root.setAttribute( "typespace", m_typeSpace );
	root.setAttribute( "coolairspace", cool);
	root.setAttribute( "capacityspace", m_capacitySpace );
	root.setAttribute( "namespace", m_nameSpace );
	
	return root;
}


void ADSpace::setValues(XMLResults)
{
}


void ADSpace::setValues(const QXmlAttributes& values)
{
	dDebug() << "here adspace";
	m_codeSpace = values.value("codespace");
	m_typeSpace = values.value("typespace");
	if(values.value("coolairspace") == "true")
	{
		m_coolAirSpace = true;
	}
	else
	{
		m_coolAirSpace = false;
	}
	dDebug() << "*******************///////////////////";
	dDebug() << "ADSpace::setValues(XMLResults values)";
	dDebug() << "*******************///////////////////";
	m_capacitySpace = values.value("capacityspace");
	m_nameSpace = values.value("namespace");
	m_valid = true;
	
}

bool ADSpace::isValid()const
{
	return m_valid;
}

QStringList ADSpace::listAudioVisual() const
{
	return m_listAudioVisual;
}

QString ADSpace::codeSpace() const
{
	return m_codeSpace;
}

QString ADSpace::typeSpace() const
{
	return m_typeSpace;
}

bool ADSpace::coolAirSpace() const
{
	return m_coolAirSpace;
}

QString ADSpace::capacitySpace() const
{
	return m_capacitySpace;
}

QString ADSpace::nameSpace() const
{
	return m_nameSpace;
}

