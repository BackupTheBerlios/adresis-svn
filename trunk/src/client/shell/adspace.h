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


/**
@author Charly Aguirre Manzano,0330911
*/
class ADSpace : public ADObject
{
	public:
		ADSpace();
// 		codeSpace varchar(10) PRIMARY KEY,
// 		typeSpace varchar(20),
// 		coolAirSpace boolean,
// 		capacitySpace integer,
// 		nameSpace varchar(20)
		ADSpace(const QString & codeSpace, const QString & typeSpace, const bool & coolAirSpace, const int & capacitySpace, const QString & nameSpace, const QStringList listAudioVisual );
		~ADSpace();
		ADInsertPackage insertPackage();
		ADUpdatePackage updatePackage();
		void setValues(XMLResults values);
		bool isValid();
		QString codeSpace(); 
		QString typeSpace(); 
		QString coolAirSpace(); 
		QString capacitySpace(); 
		QString nameSpace(); 
		//QList <ADAudioVisual> listAudioVisual();
		QStringList listAudioVisual();
		
	private:
		QString m_codeSpace;
		QString m_typeSpace;
		QString m_coolAirSpace;
		QString m_capacitySpace;
		QString m_nameSpace;
		bool m_valid;
		QList <ADAudioVisual> m_listAudioVisual;
		QStringList m_listAudioVisual;
		

};

#endif
