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
#ifndef ADREPORTFROM_H
#define ADREPORTFROM_H


#include "adformbase.h"
#include <QComboBox>
#include "ddatepicker.h"

/**
 * @author Jorge Cuadrado <kuadrosxx@gmail.com>
*/
class ADReportForm : public ADFormBase
{
	Q_OBJECT
	public:
		ADReportForm(const QString & login , QWidget *parent = 0);
		~ADReportForm();
		
	private:
		QComboBox *m_type, *m_consult;
		DDatePicker *m_beginDate, *m_endDate;
		QStringList m_consultList, consultHistogram;
		bool m_inserter;
		QString m_user;
		
	public slots:
		void emitEvent();
		void modifyContend( Logic::Module, const QList< QVariant >&);
};

#endif
