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
#ifndef ADREPORT_H
#define ADREPORT_H

/**
 * @author Jorge Cuadrado <kuadrosxx@gmail.com>
*/

#include "adobject.h"
#include <QDateTime>
#include <QTextDocument>

class ADReport : public ADObject
{
	public:
		enum TypeConsult{ TimeAudio = 0, TimeProfesor, Cancelations };
		enum TypeReport{ List = 0, Histogram };
		ADReport(const QString & creator, TypeConsult consult, TypeReport type, const QDate &beginDate, const QDate &endDate, const QDateTime & created = QDateTime::currentDateTime()  );
		~ADReport();
		
		virtual QDomElement toXml(QDomDocument &doc) const;
		void setValues(XMLResults values);
		bool isValid()  const;
		
		QTextDocument *text();
		
		int type();
		QString typeStr() const;
		int consult();
		QString consultStr() const;
		
		QDateTime created() const;
		QDate beginDate() const;
		QDate endDate() const;
		
		QString creator() const;
		QString content() const;
		
		
		
		void setContent(const QString & content);
		
	private:
		QString m_creator;
		TypeConsult m_consult;
		TypeReport m_type;
		QDate  m_beginDate, m_endDate;
		QDateTime m_created;
		QString m_content;
};

Q_DECLARE_METATYPE(ADReport *);

#endif
