/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado      krawek@gmail.com            *
 *                         Jorge Cuadrado      kuadrosxx@gmail.com         *
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

#include "adserver.h"
#include "adserverconnection.h"

#include <QHostInfo>

#include <ddebug.h>

#include "serrorpackage.h"
#include "ssuccesspackage.h"
#include "sresourcepackage.h"
#include "sresultset.h"

#include "postgreserrorhandler.h"

ADServer::ADServer(QObject *parent) : QTcpServer(parent)
{
}

ADServer::ADServer(ADS::ConnectionType type, const QString &host, QObject *parent) : QTcpServer(parent)
{
	openConnection( type, host);
}

ADServer::~ADServer()
{
}

bool ADServer::openConnection(ADS::ConnectionType type, const QString &host)
{
	QList<QHostAddress> addrs = QHostInfo::fromName(host).addresses();
	
	if ( !addrs.isEmpty() )
	{
		int port = 0;
		
		switch(type)
		{
			case ADS::Client:
			{
				port = ADS::CLIENT_PORT;
			}
			break;
		}
		
		if(! listen(QHostAddress(addrs[0]), port) )
		{
			dError() << "Can't connect to " << host<<":"<<port<< " error was: " << errorString();
			return false;
		}
		else
		{
			m_type = type;
		}
	}
	else
	{
		dError() << "Error while try to resolve " << host;
		return false;
	}
	
	return true;
}

void ADServer::incomingConnection(int socketDescriptor)
{
	SHOW_VAR(m_connections.count());
	
	ADServerConnection *newConnection = new ADServerConnection(socketDescriptor,this);
	
	handle(newConnection);
	
	m_connections << newConnection;
	
	newConnection->start();
}

void ADServer::handle(const ADServerConnection *cnx)
{
	connect(cnx, SIGNAL(finished()), cnx, SLOT(deleteLater()));
	
	connect(cnx, SIGNAL(requestSendToAll( const QString& )), this, SLOT(sendToAll( const QString& )));
	connect(cnx, SIGNAL(requestSendToAll( const QDomDocument& )), this, SLOT(sendToAll( const QDomDocument& )));
	
	connect(cnx, SIGNAL(requestRemoveConnection(ADServerConnection *)), this, SLOT(removeConnection(ADServerConnection *)));
	
	connect(cnx, SIGNAL(requestAuth(ADServerConnection *, const QString &, const QString &)), this, SLOT(authenticate(ADServerConnection *,const QString &, const QString &)));
	
	connect(cnx, SIGNAL(requestOperation( ADServerConnection *,const ADQuery* )), this, SLOT(doOperation(ADServerConnection *, const ADQuery* )));
}


void ADServer::sendToAll(const QString &msg)
{
	foreach(ADServerConnection *connection, m_connections)
	{
		connection->sendToClient(msg);
	}
}

void ADServer::sendToAll(const QDomDocument &pkg)
{
	D_FUNCINFO;
	foreach(ADServerConnection *connection, m_connections)
	{
		connection->sendToClient(pkg);
	}
}

void ADServer::removeConnection(ADServerConnection *cnx)
{
	D_FUNCINFO;
	cnx->close();
	cnx->setLogin(0);
	m_connections.removeAll(cnx);
}

void ADServer::authenticate(ADServerConnection *cnx, const QString &login, const QString &password)
{
	// TODO: HACER VALIDACION!
	// TODO: Hacer una blacklist!
	// TODO: encriptar el password
	
	dDebug() << "Request auth!";
	dDebug() << "Login: " << login << " Password: " << password;
	
	if ( cnx->isLogged())
	{
		return;
	}
// 	dDebug() << "1";
	ADSelect select(QStringList() << "passwduser", "aduser" );
	select.setWhere("loginuser="+SQLSTR(login));
	SResultSet rs = SDBM->execQuery(&select);
	
	if ( !rs.isValid() )
	{
		cnx->sendToClient( SErrorPackage(1, tr("Bad login") ) );
		cnx->close();
		
		return;
	}
	
	QString truePasswd = rs.map()["passwduser"][0];
	if ( truePasswd.isEmpty() || password != truePasswd )
	{
		cnx->sendToClient( SErrorPackage(3, tr("Bad password") ) );
		cnx->close();
	}
// 	else
	{
		cnx->setLogin(login);
		ADSelect user(QStringList() << "passwduser", "aduser");
		
// 		cnx->sendToClient( SSuccessPackage(tr("autenticado")));
		ADUser user = 
		ADEvent event(ADEvent::Server, Logic::Users, Logic::Info,   );
		
		cnx->sendToClient( SDBM->execQuery(&user) );
	}
}

void ADServer::doOperation(ADServerConnection *cnx, const ADQuery *query)
{
	SResultSet rs = SDBM->execQuery(query);
	
	if ( SDBM->lastError().isValid() )
	{
		cnx->sendToClient( PostgresErrorHandler::handle( SDBM->lastError() ) );
	}
	else
	{
		switch(query->type())
		{
			case ADQuery::Select:
			{
				cnx->sendToClient(rs);
		
				break;
			}
			case ADQuery::Insert:
			{
				
				break;
			}
			case ADQuery::Delete:
			{
				
				break;
			}
			case ADQuery::Update:
			{
				
				break;
			}
		}
	}
}



