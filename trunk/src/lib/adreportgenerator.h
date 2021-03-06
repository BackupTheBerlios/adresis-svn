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
#ifndef ADREPORTGENERATOR_H
#define ADREPORTGENERATOR_H

#include <QString>
#include <QVariant>
#include <QTextDocument>

#include "adreserve.h"
#include "sresultset.h"
/**
	@author Jorge Cuadrado <kuadrosxx@gmail.com>
*/
class ADReportGenerator
{
	public:
		ADReportGenerator();
		~ADReportGenerator();
		/**
		 * retorna un QTextDocument con el horario de los espacios
		 * @param spaces 
		 * @return 
		 */
		static QTextDocument *generateSchedule(const QList<ADReserve *>& reserves, const QString & title);
		static QTextDocument *generateListReport(const QDate & beginDate, const QDate & endDate, const SResultSet & rs , const QStringList & headers );
		static QTextDocument *generateGraphicReport(const QDate & beginDate, const QDate & endDate, const QString & strGraphic);
		
};

#endif
