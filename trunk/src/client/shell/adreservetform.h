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
#ifndef ADRESERVETFORM_H
#define ADRESERVETFORM_H

#include "adformbase.h"
#include <QMap>
#include "global.h"
#include <QComboBox>
#include <QPushButton>

//cambios
#include "adreserve.h"
#include "adschedule.h"

/**
	@author Hector Fabio Cruz Mosquera,0329876 <hecfacru@gmail.com>
*/


class ADReserveTForm : public ADFormBase
{
	Q_OBJECT
	public:
		ADReserveTForm(QWidget *parent = 0);
		ADReserveTForm(const ADReserve & reserve, QWidget *parent = 0);
		~ADReserveTForm();
	
	private:
		QMap<QString, QWidget*> m_inputs;
		QMap< QString, QString> nameResources;
		QList<QMap<QString, QString> > listSchedules;
		QComboBox *tipoRecC, *tipoResC, *recursosC, *resourcesNameC;
		bool m_inserter, m_reserve;
		QString m_responsable, destinationReserve;
		QStringList recursosEsp, recursosAud;
		
		void fill();
		void setup();
		int list;
		bool valite();
		ADSchedule *horario;
		
	signals:
		void requestInsertReserve(const QString& table, const QString& typeR, const QString& userReserve, const QString& userResponsable, const QString& idRecurso, const QString& day, const QString& beginhour, const QString& endhour, const QString& begindate, const QString& enddate, const bool& isactive, const QString& destinationReserve);
		
		void requestUpdateReserve(const QString& typeR, const QString& recurso, const QString& resposable);
		void requestTypeReserve(const QString& typeR);
		void requestTypeResources(const QString& table, const QString& typeR);
		void consultSchedule(const QString& table, const QString& resource);
		
	public slots:
		void emitInsertReserve();
		void insertListTypes(const QList<XMLResults>& results);
		void insertListNameResources(const QList<XMLResults>& results);
// 		void changeTypeReserve(int);
		void changeTypeResource(int);
		void changeTypeSpace(QString);
		void changeNameSpace(const QString&);
		void permisos(const QString &login, const bool permiso);
		void requestSchedule( const QList<XMLResults>& results );
		
};
Q_DECLARE_METATYPE(ADReserveTForm *);

#endif
