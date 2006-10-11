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
#ifndef ADRESERVE_H
#define ADRESERVE_H

#include "adobject.h"

/**
	@author Hector Fabio Cruz Mosquera,0329876 <hectorcaz@gmail.com>
*/
class ADReserve : public ADObject
{
	public:
		ADReserve();
		ADReserve(const QString& typeReserve, const QString& idUserReserve, const QString& idUserResponsable, const QString& idResource, const QString& day, const QString& beginHour, const QString& endHour, const QString& beginDate, const QString& endDate, const bool isActive, const QString& destinationreserve);
		~ADReserve();
// 		ADInsertPackage insertPackage();
// 		ADInsertPackage insertPackage(const QString&);
// 		ADUpdatePackage updatePackage();
		
		QString toXml() const;
		void fromXml(const QString & xml );
		
		void setValues(XMLResults values);
		bool isValid() const;
		QString typeReserve() const;
		QString iduserreserve() const;
		QString iduserresponsable() const;
		QString idresource() const;
		QString day() const;
		QString beginhour() const;
		QString endhour() const;
		QString begindate() const;
		QString enddate() const;
		bool isActive() const;
		QString destinationreserve() const;
		
	private:
		QString m_typereserve;
		QString m_iduserreserve;
		QString m_iduserresponsable;
		QString m_idresource;
		QString m_day;
		QString m_beginhour;
		QString m_endhour;
		QString m_begindate;
		QString m_enddate;
		bool m_isactive;
		QString m_destinationreserve;
		bool m_valid;
		QString tipo;

};

#endif
