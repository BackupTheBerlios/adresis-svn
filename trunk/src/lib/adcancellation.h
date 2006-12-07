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
#ifndef ADCANCELLATION_H
#define ADCANCELLATION_H

#include "adobject.h"
#include <QMetaType>
#include <QDateTime>
#include <QDomElement>
#include "adevent.h"

/**
	@author Hector Fabio Cruz Mosquera,0329876 <hecfacru@s4pc18>
 */
class ADCancellation: public ADObject
{
	public:
		ADCancellation();
		ADCancellation(QString idCancellation,QString iduser, QDateTime dateCancellation, QString razonCancellation);
		~ADCancellation();
		QString idReserveCancellation();
		QString idUserCancellation();
		QDateTime dateTimeCancellation();
		QString razonCancellation();
		QDomElement toXml(QDomDocument &doc) const;
		void fromXml(const QString & xml );
		void setValues(XMLResults values);
		bool isValid() const;
		void setRazonCancellation(QString r);
		
	
		
	private:
		QString m_idReserveCancellation;
		QDateTime m_dateTimeCancellation;
		QString m_razonCancellation;
		QString m_idUserCancellation;
		bool m_valid;
};

Q_DECLARE_METATYPE(ADCancellation *);
#endif

