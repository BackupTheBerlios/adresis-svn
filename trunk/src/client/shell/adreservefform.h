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
		ADReserveFForm( ADReserve * reserve, QList<QString> list, QList<ADReserve *> reservas, QWidget *parent = 0);
		~ADReserveFForm();
		
	private:
		QComboBox *typeResourceC, *resourceC, *resourcesNameC, *loginC;
		QTextEdit *areaTexto;
		ADSchedule *horario;
		QString m_userReserve, destinationReserve;
		QStringList typeSpaces, typeAudiovisual;
		QStringList resourcesSpaces, resourcesAud;
		QMap< QString, QString> nameResources;	//El primer String es la clave principal en la BD y el segundo es el nombre del recurso.
		QMap<QString, QString> m_dateSemestral;
		QList<QMap<QString, QString> > listSchedules;
		bool m_inserter, userValite, resourceValite;
		
		ADReserve *m_reserve;
		QString idReserve;
		void setup();
		bool valite();
		void valiteUserAndResource(QString, QString);
		void requestDatesSemestral();
		void requestLogin();
		void requestLoginUsers();
		void clearFields();

	signals:
		void sendEvent( ADEvent *);
		
		
	public slots:
		void receiveEvent(ADEvent *);
		void changeTypeResource(int);
		void insertListTypes();
		void changeResource(QString);
		void insertListNameResources();
		void changeNameResource(const QString&);
		void receiveReserves( const QList<ADReserve *>& results );
		void emitEvent();
		
		void modifyContend( Logic::Module, const QList< QVariant >&);
		
};

Q_DECLARE_METATYPE(ADReserveFForm *);
#endif
