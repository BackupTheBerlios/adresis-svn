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
#include "adresis.h"
#include <ddebug.h>
Adresis::Adresis(QObject * parent)
	: QObject(parent)
{
	m_connector = new ADConnector(this);
	connect ( m_connector, SIGNAL(message(Msg::Type , const QString &)), this, SIGNAL(requestShowMessage( Msg::Type, const QString& )));
	connect ( m_connector, SIGNAL(userAutenticated(const XMLResults&)) , this, SLOT(autenticated(const XMLResults&) ));
	DINIT;
}

Adresis::~Adresis()
{
	DEND;
}

void Adresis::connectToHost( const QString & hostName, quint16 port)
{
	m_connector->connectToHost( hostName, port);
}

//slots
void Adresis::login(const QString &user, const QString &passwd)
{
	m_connector->login(user, passwd);
	
	ADSelectPackage u(QStringList()<< "aduser", QStringList() << "*");
	u.setWhere( "loginuser='"+  user + "'" );
	m_connector->sendQuery( Logic::userAuthenticated,u); 
}

void Adresis::autenticated(const XMLResults & values)
{
	
	m_user.setValues( values);
	dDebug() << m_user.name();
	dDebug() << m_user.code();
	dDebug() << m_user.passwd();
// 	dDebug() << m_user.passwd();
// 	m_user.permissions();
}


