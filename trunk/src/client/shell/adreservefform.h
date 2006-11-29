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
#ifndef ADRESERVEFFORM_H
#define ADRESERVEFFORM_H

#include "adformbase.h"
#include <QMap>
#include "global.h"
#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>
		
//cambios
#include "adreserve.h"
#include "adschedule.h"
#include "adevent.h"

/**
	@author Hector Fabio Cruz Mosquera,0329876 <hecfacru@gmail.com>
*/


class ADReserveFForm : public ADFormBase
{
	Q_OBJECT
	public:
		ADReserveFForm(QWidget *parent = 0);
		ADReserveFForm(const ADReserve & reserve, QWidget *parent = 0);
		~ADReserveFForm();
		
	private:
		QComboBox *typeResourceC, *resourceC, *resourcesNameC;
		QTextEdit *areaTexto;
		ADSchedule *horario;
		QString m_responsable, destinationReserve;
		QStringList typeSpaces, typeAudiovisual;
		QStringList resourcesSpaces, resourcesAud;
		QMap< QString, QString> nameResources;	//El primer String es la clave principal en la BD y el segundo es el nombre del recurso.
		QMap<QString, QWidget*> m_inputs;
		QMap<QString, QString> m_dateSemestral;
		QList<QMap<QString, QString> > listSchedules;
		bool m_inserter, m_reserve;
		
		void fill();
		void setup();
		bool valite();
		void requestDatesSemestral();
		void requestLogin();
		
		
		
		
	signals:
		void sendEvent( ADEvent *);
		
		/////////////////////////////////
		void requestInsertReserve(const QString& table, const QString& typeR, const QString& userReserve, const QString& userResponsable, const QString& idRecurso, const QString& day, const QString& beginhour, const QString& endhour, const QString& begindate, const QString& enddate, const bool& isactive, const QString& destinationReserve);
		
		void requestUpdateReserve(const QString& typeR, const QString& recurso, const QString& resposable);
		void requestTypeReserve(const QString& typeR);
		void requestTypeResources(const QString& table, const QString& typeR);
		void consultSchedule(const QString& table, const QString& resource);
		
	public slots:
		void receiveEvent(ADEvent *);
		void changeTypeResource(int);
		void insertListTypes();
		void changeResource(QString);
		void insertListNameResources();
		void changeNameResource(const QString&);
		void receiveReserves( const QList<ADReserve *>& results );
		void emitInsertReserve();
		/////////////////////77
		
// 		void changeTypeReserve(int);
// 		
		
		
};

Q_DECLARE_METATYPE(ADReserveFForm *);
#endif
