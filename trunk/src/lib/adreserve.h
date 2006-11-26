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
#include <QMetaType>
#include <QDomElement>
#include <QDateTime>

/**
	@author Hector Fabio Cruz Mosquera,0329876 <hectorcaz@gmail.com>
*/
class ADReserve : public ADObject
{
	public:
		ADReserve();
// idreserve | typereserve | iduserreserve | iduserresponsable | idaudiovisual | idspace | day | beginhour | endhour | begindate | enddate | isactive | destinationreserve 

/** PENDIENTE PARA  BORRAR **/
// 		ADReserve(const QString& typeReserve, const QString& idUserReserve, const QString& idUserResponsable, const QString& idAudiovisual, const QString& idSpace, const QString& day, const QString& beginHour, const QString& endHour, const QString& beginDate, const QString& endDate, const bool isActive, const QString& destinationreserve);

		ADReserve(const QString idReserve, const QString& typeReserve, const QString& idUserReserve, const QString& idUserResponsable, const QString& idAudiovisual, const QString& idSpace, const QString& day, const QDateTime& beginDateTime, const QDateTime& endDateTime, const bool isActive, const QString& destinationreserve);

		~ADReserve();
		
		QDomElement toXml(QDomDocument &doc) const;
		void fromXml(const QString & xml );
		void setValues(XMLResults values);
		bool isValid() const;
		QString idReserve() const;
		QString typeReserve() const;
		QString iduserreserve() const;
		QString iduserresponsable() const;
		QString idaudiovisual() const;
		QString idspace() const;
		QString day() const;
		QDateTime beginDateTime() const;
		QDateTime endDateTime() const;
		bool isActive() const;
		QString destinationreserve() const;
		
	private:
		QString m_idReserve;
		QString m_typereserve;
		QString m_iduserreserve;
		QString m_iduserresponsable;
		QString m_idaudiovisual;
		QString m_idspace;
		QString m_day;
		QDateTime m_beginDateTime;
		QDateTime m_endDateTime;
		bool m_isactive;
		QString m_destinationreserve;
		bool m_valid;
		QString tipo;

};

Q_DECLARE_METATYPE(ADReserve *);
#endif
