/***************************************************************************
		*   Copyright (C) 2006 by Jorge Cuadrado   *
		*   kuadrosx@zi0n   *
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
#ifndef ADSPACE_H
#define ADSPACE_H

#include "adobject.h"
#include <QStringList>
#include <QMetaType>
#include <QDomElement>
#include <QXmlDefaultHandler>

 
/**
 * @author Charly Aguirre Manzano,0330911
 */
		class ADSpace : public ADObject
{
	public:
		ADSpace();
		ADSpace(const QString & codeSpace, const QString & typeSpace, const bool & coolAirSpace, const QString & capacitySpace, const QString & nameSpace);
		~ADSpace();
	
		QDomElement toXml(QDomDocument &doc) const;
		
		void setValues(const XMLResults values);
		void setValues(const QXmlAttributes& values);
		bool isValid() const;
		QString codeSpace() const;
		QString typeSpace() const;
		bool coolAirSpace() const;
		QString capacitySpace() const;
		QString nameSpace() const;
		QStringList listAudioVisual() const;
		
	private:
		QString m_codeSpace;
		QString m_typeSpace;
		bool m_coolAirSpace;
		QString m_capacitySpace;
		QString m_nameSpace;
		bool m_valid;
		QStringList m_listAudioVisual;
		

};

Q_DECLARE_METATYPE(ADSpace *);
#endif
