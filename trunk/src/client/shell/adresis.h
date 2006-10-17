/***************************************************************************
 *   Copyright (C) 2006 by Jorge Cuadrado                                  *
 *   kuadrosx@gmail.com                                                    *
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
#ifndef ADRESIS_H
#define ADRESIS_H

#include <QObject>

#include "adeventhandler.h"
#include "adconnector.h"
#include "global.h"
#include "aduser.h"
#include "adaudiovisual.h"
#include "adspace.h"
#include "adreserve.h"


#include "adeventhandler.h"

/**
* @author Jorge Cuadrado <kuadrosxx@gmail.com>
*/

class Adresis : public QObject, public ADAbstractEventHandler
{
	Q_OBJECT;
	public:
		Adresis(QObject * parent=0);
		~Adresis();
		
	public slots:
		void connectToHost( const QString & hostName, quint16 port);
		void login(const QString &user, const QString &passwd);
		void autenticated(const XMLResults& values);
		
	private:
		ADConnector *m_connector;
		ADUser *m_user;
		
	signals:
		void requestShowMessage( Msg::Type, const QString&);
		
	public slots:
		void handleEvent(ADEvent * event = 0);
		
};

#endif
