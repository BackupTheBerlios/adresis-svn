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
#include "addeletepackage.h"
#include <ddebug.h>
Adresis::Adresis(QObject * parent)
	: QObject(parent)
{
	m_connector = new ADConnector(this);
	connect ( m_connector, SIGNAL(message(Msg::Type , const QString &)), this, SIGNAL(requestShowMessage( Msg::Type, const QString& )));
	connect ( m_connector, SIGNAL(userAutenticated(const XMLResults&)) , this, SLOT(autenticated(const XMLResults&) ));
	connect ( m_connector, SIGNAL(fillModule(Logic::TypeModule, const QList<XMLResults>&)) , this, SIGNAL(requestFillModule(Logic::TypeModule, const QList<XMLResults>&)) );
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
	
	ADSelectPackage u(QStringList()<< "aduser", QStringList() << "nameuser" << "codeuser" << "loginuser"<< "passwduser" << "permissionsuser");
	u.setWhere( "loginuser='"+  user + "'" );
	m_connector->sendQuery( Logic::userAuthenticated,u); 
}

void Adresis::autenticated(const XMLResults & values)
{
	m_user.setValues( values);
	requestCreateModules();
}

void Adresis::getInfoModule(Logic::TypeModule module )
{
	if(m_user.permissions()[module])
	{
		switch(module)
		{
			case Logic::users:
			{
				ADSelectPackage select(QStringList()<< "aduser", QStringList() << "nameuser"<< "loginuser" );
				m_connector->sendQuery(Logic::fillUserModule, select);
			}
			
			case Logic::spaces:
			{
				ADSelectPackage select(QStringList()<< "adspace", QStringList() << "codeSpace"<<"typeSpace"<<"nameSpace");
				m_connector->sendQuery(Logic::fillSpaceModule, select);
			}

			case Logic::audiovisuals:
			{
				ADSelectPackage select(QStringList()<< "adaudiovisual", QStringList() << "typeAV"<<"numberinventoryAV");
				m_connector->sendQuery(Logic::fillAudiovisualModule, select);
			}
			
			case Logic::reserves:
			{
// 				ADSelectPackage select(QStringList()<< "adaudiovisual", QStringList() << "type"<<"inventoryNumber");
// 				m_connector->sendQuery(Logic::fillAudioVisualModule, select);
			}
		}
	}
	else
	{
		emit requestShowMessage(Msg::Error, "Error, no tiene permisos sobre este modulo");
	}
}

void Adresis::addUser(const QString& name, const QString& code,const QString& login,const QString& passwd,QMap<Logic::TypeModule, bool> permissions )
{
	ADUser newUser(name,  code, login,passwd, permissions);
	
	m_connector->sendPackage(  newUser.insertPackage() );
	getInfoModule(Logic::users);
}

void Adresis::execDelete(Logic::TypeModule module, const QString& key)
{
	QString where;
	QString table;
	switch(module)
	{
		case Logic::users:
		{
			where = "loginuser = '" + key + "'";
			table = "aduser";
		}
	}
	ADDeletePackage del(table);
	del.setWhere(where);
	m_connector->sendPackage(del);
	getInfoModule(Logic::users);
}
