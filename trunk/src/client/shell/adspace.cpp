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
#include "ddebug.h"

ADSpace::ADSpace() 
	: ADObject()
{
	m_valid = false;
}

ADSpace::ADSpace(const QString & codeSpace, const QString & typeSpace, const bool & coolAirSpace, const QString & capacitySpace, const QString & nameSpace, const QStringList & listAudiovisual) 
	: ADObject(), m_codeSpace(codeSpace), m_typeSpace(typeSpace), m_coolAirSpace(coolAirSpace), m_capacitySpace(capacitySpace), m_nameSpace(nameSpace), m_listAudioVisual(listAudiovisual)
{
	m_valid = true;
}

ADSpace::~ADSpace()
{
}

ADInsertPackage ADSpace::insertPackage()
{
// 	QString strAudioVisual;
// 	int i = 0;
// 	while(!listAudioVisual.isEmpty())
// 	{
// 		strAudioVisual = listAudioVisual.takeAt(i);
// 	}
	
	
	
	ADInsertPackage insert("adspace", QStringList() << "codespace" << "typespace" << "coolairspace" << "capacityspace" << "namespace", QStringList() << m_codeSpace << m_typeSpace << m_coolAirSpace << m_capacitySpace << m_nameSpace );
	
// 	foreach(QString tmp, listAudioVisual)
// 	{
// 		
// 	}

	
	dDebug() << insert.toString();
	return insert;
}

ADUpdatePackage ADSpace::updatePackage()
{
	ADUpdatePackage update("adspace", QStringList() << "codespace" << "typespace" << "coolairspace" << "capacityspace" << "namespace", QStringList() << m_codeSpace << m_typeSpace << m_coolAirSpace << m_capacitySpace << m_nameSpace );
	
	dDebug() << update.toString();
	return update;
}

void ADSpace::setValues(XMLResults values)
{
	dDebug() << "here adspace";
	m_codeSpace = values["codespace"];
	m_typeSpace = values["typespace"];
	m_coolAirSpace = values["coolairspace"];
	m_capacitySpace = values["capacityspace"];
	m_nameSpace = values["namespace"];
	m_valid = true;
	
}

bool ADSpace::isValid()
{
	return m_valid;
}

QStringList ADSpace::listAudioVisual()
{
	return m_listAudioVisual;
}

QString ADSpace::codeSpace()
{
	return m_codeSpace;
}

QString ADSpace::typeSpace()
{
	return m_typeSpace;
}

QString ADSpace::coolAirSpace()
{
	return m_coolAirSpace;
}

QString ADSpace::capacitySpace()
{
	return m_capacitySpace;
}

QString ADSpace::nameSpace()
{
	return m_nameSpace;
}

