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

#include "aduser.h"
#include "adspace.h"
#include "adaudiovisual.h"
#include "adreserve.h"
#include "adevent.h"
#include "adpermission.h"


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
	
	
	connect(cnx, SIGNAL(requestEvent( ADEvent * )), this, SLOT(handleEvent(ADEvent * )));
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
// 		ADSelect infoUser(QStringList() << "loginuser" << "codeuser" << "nameuser" << "permissionsuser" , "aduser");

		ADSelect infoUser(QStringList() << "loginuser" << "codeuser" << "nameuser" << "rol" , "aduser");
		infoUser.setWhere("loginuser="+SQLSTR(login));
		SResultSet rs = SDBM->execQuery(&infoUser);
		
		QString rol = rs.map()["rol"][0];
		ADSelect permisos(QStringList() << "action" << "permission", "adrols");
		permisos.setWhere("rol="+SQLSTR(rol));
		SResultSet rsP = SDBM->execQuery(&permisos);
		
		ADPermission permissions;
		permissions.setValues( rsP.map() );
		
		dDebug() << rs.map().size();
		ADUser *user= new  ADUser( rs.map()["nameuser"][0], rs.map()["codeuser"][0],rs.map()["loginuser"][0], "", permissions );
		ADEvent event(ADEvent::Server, Logic::Users, Logic::Info, QVariant::fromValue (user)  );

		cnx->sendToClient( event.toString() );
		
		delete user;
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


void ADServer::handleEvent(ADServerConnection *cnx, ADEvent * event )
{

	D_FUNCINFO;
	if(event)
	{
		if(event->source() == ADEvent::Server)
		{
			
			
		}
		else
		{
			
			switch(event->module())
			{
				//enum Module{Users=0, Spaces, Audiovisuals, Reserves, Reports};
				case Logic::Users:
				{
					switch(event->action())
					{
						case Logic::Find:
						{
							ADSelect infoUser(QStringList() << "*" , "aduser");
							SResultSet rs = SDBM->execQuery(&infoUser);
							dDebug() << rs.toString();
							QList<QVariant> listUsers;
							
							for(int pos =0; pos < rs.map().count(); pos++)
							{	
								QString rol = rs.map()["rol"][pos];
								ADSelect permisos(QStringList() << "action" << "permission", "adrols");
								permisos.setWhere("rol="+SQLSTR(rol));
								SResultSet rsP = SDBM->execQuery(&permisos);
				
								ADPermission permissions;
								permissions.setValues( rsP.map() );
								
								
								ADUser *user= new  ADUser( rs.map()["nameuser"][pos], rs.map()["codeuser"][pos],rs.map()["loginuser"][pos], "", permissions);
								
								
								listUsers.append(QVariant::fromValue (user));
							}
							
							ADEvent event(ADEvent::Server,Logic::Users, Logic::Find, listUsers);
							cnx->sendToClient(event.toString());
							break;
						}
					}
					break;
				}
				
				case Logic::Spaces:
				{
					
					switch(event->action())
					{
						case Logic::Find:
						{
							ADSelect infoSpaces(QStringList() << "*" , "adspace");
							SResultSet rs = SDBM->execQuery(&infoSpaces);
							dDebug() << rs.toString();
							QList<QVariant> listSpaces;
							for(int pos =0; pos < rs.map().count(); pos++)
							{	
								bool ac;
								if(rs.map()["coolairspace"][pos] == "t")
								{
									ac=true;
								}
								else
								{
									ac=false;	
								}
								
								
								ADSpace *space = new ADSpace( rs.map()["codespace"][pos], rs.map()["typespace"][pos], ac, rs.map()["capacityspace"][pos], rs.map()["namespace"][pos]);
								
								
								listSpaces.append(QVariant::fromValue (space));
							}
							
							ADEvent event(ADEvent::Server,Logic::Spaces, Logic::Find, listSpaces);
							cnx->sendToClient(event.toString());
							
							break;
						}
					}
					break;
				}
				
				
				case Logic::Audiovisuals:
				{
					switch(event->action())
					{
						case Logic::Find:
						{
							ADSelect infoAv(QStringList() << "*" , "adaudiovisual");
							SResultSet rs = SDBM->execQuery(&infoAv);
							dDebug() << rs.toString();
							QList<QVariant> listAvs;
							
							
							for(int pos =0; pos < rs.map().count(); pos++)
							{	
		// 						ADAudioVisual(const QString & type, const QString & marksEquipment, const QString & estate, const QString & numberInventory, const QString & codeSpace);
								
								
								ADAudioVisual *av = new ADAudioVisual(rs.map()["typeav"][pos], rs.map()["marksequipmentav"][pos], rs.map()["estateav"][pos], rs.map()["numberinventoryav"][pos], rs.map()["codespace"][pos]);
								
								
								listAvs.append(QVariant::fromValue (av));
							}
							
							ADEvent event(ADEvent::Server,Logic::Audiovisuals, Logic::Find, listAvs);
							cnx->sendToClient(event.toString());
							break;
						}
					}
					break;
				}
				
				
				case Logic::Reserves:
				{
					switch(event->action())
					{
						case Logic::Find:
						{	
							ADSelect infoReservesAv(QStringList() << "*" , "adavreserve");
							SResultSet rs = SDBM->execQuery(&infoReservesAv);
							dDebug() << rs.toString();
							QList<QVariant> listReserves;
							
							for(int pos =0; pos < rs.map().count(); pos++)
							{	
								bool active;
								if(rs.map()["isactive"][pos] == "t")
								{
									active=true;
								}
								else
								{
									active=false;	
								}
								
								ADReserve *avReserve = new ADReserve(rs.map()["typereserve"][pos], rs.map()["iduserreserve"][pos], rs.map()["iduserresponsable"][pos], rs.map()[" idresource"][pos], rs.map()["day"][pos], rs.map()["beginhour"][pos], rs.map()["endhour"][pos], rs.map()["begindate"][pos], rs.map()["enddate"][pos], active, rs.map()["destinationreserve"][pos]);
								
								
								listReserves.append(QVariant::fromValue (avReserve));
							}
							
							
							
							
							
							ADSelect infoReservesSp(QStringList() << "*" , "adspacereserve");
							rs = SDBM->execQuery(&infoReservesSp);
							dDebug() << rs.toString();
							
							for(int pos =0; pos < rs.map().count(); pos++)
							{
								bool active;
								if(rs.map()["isactive"][pos] == "t")
								{
									active=true;
								}
								else
								{
									active=false;	
								}
								
								ADReserve *spaceReserve = new ADReserve(rs.map()["typereserve"][pos], rs.map()["iduserreserve"][pos], rs.map()["iduserresponsable"][pos], rs.map()[" idresource"][pos], rs.map()["day"][pos], rs.map()["beginhour"][pos], rs.map()["endhour"][pos], rs.map()["begindate"][pos], rs.map()["enddate"][pos], active, rs.map()["destinationreserve"][pos]);
								
								
								listReserves.append(QVariant::fromValue (spaceReserve));
							}
							
							ADEvent event(ADEvent::Server,Logic::Reserves, Logic::Find, listReserves);
							cnx->sendToClient(event.toString());
							
							break;
						}
					}
					break;
				}
				case Logic::Reports:
				{
					dDebug() << "REPORTES";
					break;
				}
			}
		}
	}
	else
	{
		dFatal() << "no existe el evento";
	}
}

