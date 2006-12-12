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
#include "adcancellation.h"
#include "adreport.h"

#include "histogram.h"

#include "adreportgenerator.h"
#include <cmath>
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
	
	
	connect(cnx, SIGNAL(requestEvent( ADServerConnection *,ADEvent * )), this, SLOT(handleEvent(ADServerConnection *,ADEvent * )));
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
	
	if ( cnx->isLogged())
	{
		return;
	}
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
		
		ADSelect infoUser(QStringList() << "loginuser" << "codeuser" << "nameuser" << "rol" , "aduser");
		infoUser.setWhere("loginuser="+SQLSTR(login));
		SResultSet rs = SDBM->execQuery(&infoUser);
		
		QString rol = rs.map()["rol"][0];
		SHOW_VAR(rol);
		ADSelect permisos(QStringList() << "action" << "permission", "adrols");
		permisos.setWhere("rol="+SQLSTR(rol));
		SResultSet rsP = SDBM->execQuery(&permisos);
		
		ADPermission permissions;
		permissions.setValues( rsP.map() );
		
		SHOW_VAR(permissions.value(Logic::Users, Logic::Add));
		
		dDebug() << rs.map().size();
		ADUser *user= new  ADUser( rs.map()["nameuser"][0], rs.map()["codeuser"][0],rs.map()["loginuser"][0], "", permissions,rs.map()["rol"][0].toInt() );
		ADEvent event(ADEvent::Server, Logic::Users, Logic::Authenticate, QVariant::fromValue (user));

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
		SHOW_VAR(event->toString());
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
						case Logic::Add:
						{
							ADUser *user = qvariant_cast<ADUser *>(event->data());
							ADInsert insert("aduser", QStringList()<< "rol" << "nameuser" << "codeuser" << "loginuser" << "passwduser", QStringList() <<QString::number(user->rol() ) <<  SQLSTR(user->name()) <<  SQLSTR(user->code()) << SQLSTR(user->login()) << SQLSTR(user->passwd()));
							SDBM->execQuery(&insert);
							
							if ( SDBM->lastError().isValid() )
							{
								cnx->sendToClient( PostgresErrorHandler::handle( SDBM->lastError() ) );
							}
							else
							{
								ADEvent e( ADEvent::Server, Logic::Users, Logic::Add, event->data());
								sendToAll(e.toString());
							}
						}
						break;
						case Logic::Del:
						{
							ADDelete deleteUser("aduser");
							deleteUser.setWhere( "codeUser = " + SQLSTR(event->data().toString()));
							SDBM->execQuery(&deleteUser);
							if ( SDBM->lastError().isValid() )
							{
								cnx->sendToClient( PostgresErrorHandler::handle( SDBM->lastError() ) );
							}
							else
							{
								ADEvent e( ADEvent::Server, Logic::Users, Logic::Del, event->data());
								sendToAll(e.toString());
							}
						}
						break;
						case Logic::Update:
						{
							ADUser *user = qvariant_cast<ADUser *>(event->data());
							ADUpdate updateUser("aduser" ,QStringList()<< "rol" << "nameuser" << "loginuser" << "passwduser", QStringList() << QString::number(user->rol()) << SQLSTR(user->name()) <<  SQLSTR(user->login()) << SQLSTR(user->passwd()) );
							updateUser.setWhere( "codeUser = " + SQLSTR(user->code()));
							SDBM->execQuery(&updateUser);
							if ( SDBM->lastError().isValid() )
							{
								cnx->sendToClient( PostgresErrorHandler::handle( SDBM->lastError() ) );
							}
							else
							{
								ADEvent e( ADEvent::Server, Logic::Users, Logic::Update, event->data());
								sendToAll(e.toString());
							}
						}
						break;
						case Logic::Find:
						{
							ADSelect infoUser(QStringList() << "*" , "aduser");
							if(event->data() !="all" )
							{
								QString condition = qvariant_cast<QString>(event->data());
								infoUser.setWhere(condition);
							}
							
							SResultSet rs = SDBM->execQuery(&infoUser);
							QList<QVariant> listUsers;
							
							for(int pos =0; pos < rs.map()["nameuser"].count(); pos++)
							{
								QString rol = rs.map()["rol"][pos];
								ADSelect permisos(QStringList() << "action" << "permission", "adrols");
								permisos.setWhere("rol="+SQLSTR(rol));
								SResultSet rsP = SDBM->execQuery(&permisos);
								
								
								ADPermission permissions;
								permissions.setValues( rsP.map() );
								
								ADUser *user = new  ADUser( rs.map()["nameuser"][pos], rs.map()["codeuser"][pos],rs.map()["loginuser"][pos], "", permissions, rol.toInt());
								
								listUsers.append(QVariant::fromValue (user));
							}
							ADEvent event(ADEvent::Server,Logic::Users, Logic::Find, listUsers);
							cnx->sendToClient(event.toString());
						}
						break;
						case Logic::Authenticate:
						{
							
							authenticate(cnx, event->data().toList()[0].toString(),event->data().toList()[1].toString());
						}
						break;
					}
					break;
				}
				break;
				case Logic::Spaces:
				{
					switch(event->action())
					{
						case Logic::Add:
						{
							(event->toString());
							
							ADSpace *space = qvariant_cast<ADSpace *>(event->data());
							
							ADInsert insert("adspace", QStringList()<< "codespace" << "typespace" << "coolairspace" << "capacityspace" << "namespace", QStringList() <<  SQLSTR(space->codeSpace()) <<  SQLSTR(space->typeSpace()) <<  SQLSTR(space->coolAirSpace()) << SQLSTR(space->capacitySpace()) << SQLSTR(space->nameSpace()));
							SDBM->execQuery(&insert);
							
							if ( SDBM->lastError().isValid() )
							{
								cnx->sendToClient( PostgresErrorHandler::handle( SDBM->lastError() ) );
							}
							else
							{
								ADEvent e( ADEvent::Server, Logic::Spaces, Logic::Add, event->data());
								sendToAll(e.toString());
							}
						}
						break;
						case Logic::Update:
						{
							
							ADSpace *space = qvariant_cast<ADSpace *>(event->data());
							
							ADUpdate update("adspace", QStringList()<< "typespace" << "coolairspace" << "capacityspace" << "namespace", QStringList() <<  SQLSTR(space->typeSpace()) <<  SQLSTR(space->coolAirSpace()) << SQLSTR(space->capacitySpace()) << SQLSTR(space->nameSpace()));
							update.setWhere( "codespace = " + SQLSTR(space->codeSpace()));
							SDBM->execQuery(&update);
							
							if ( SDBM->lastError().isValid() )
							{
								cnx->sendToClient( PostgresErrorHandler::handle( SDBM->lastError() ) );
							}
							else
							{
								ADEvent e( ADEvent::Server, Logic::Spaces, Logic::Update, event->data());
								sendToAll(e.toString());
							}
						}
						break;
						case Logic::Del:
						{
							dDebug() << "borrando";
							
							/// En el momento de borrado se debe de cambiarle los datos a las ayudas audiovisuales, en la base de datos hice un condicional que en caso de borrado coloque null, pero ademas de esto se debe enviar la notificacion a todos los usuarios del cambio. Para esto se selecciona las ayudas del espacio a borrar y se manda a los usuarios modificadas antes de ser realmente modificadas en la base de datos.
							ADSelect infoAv(QStringList() << "*" , "adaudiovisual");
							infoAv.setWhere("codespace = " + SQLSTR(event->data().toString()));
							
							
							SResultSet rs = SDBM->execQuery(&infoAv);
							QList<QVariant> listAvs;
							
							for(int pos =0; pos < rs.map()["numberinventoryav"].count(); pos++)
							{
								ADAudioVisual *av = new ADAudioVisual(rs.map()["typeav"][pos], rs.map()["marksequipmentav"][pos], rs.map()["estateav"][pos], rs.map()["numberinventoryav"][pos], "null");
								
								ADEvent event(ADEvent::Server,Logic::Audiovisuals, Logic::Update, QVariant::fromValue (av));
								sendToAll(event.toString());
							}

							ADDelete qdelete("adspace");
							qdelete.setWhere( "codespace = " + SQLSTR(event->data().toString()));
							SDBM->execQuery(&qdelete);
							if ( SDBM->lastError().isValid() )
							{
								cnx->sendToClient( PostgresErrorHandler::handle( SDBM->lastError() ) );
							}
							else
							{
								ADEvent e( ADEvent::Server, Logic::Spaces, Logic::Del, event->data());
								sendToAll(e.toString());
							}
						}
						break;
						
						case Logic::Find:
						{
							ADSelect infoSpaces(QStringList() << "*" , "adspace");
							if(event->data() !="all" )
							{
								QString condition = qvariant_cast<QString>(event->data());
								infoSpaces.setWhere(condition);
							}
							
							SResultSet rs = SDBM->execQuery(&infoSpaces);
							QList<QVariant> listSpaces;
							for(int pos =0; pos < rs.map()["codespace"].count(); pos++)
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

						case Logic::GetTypes:
						{
							ADSelect typesSpaces(QStringList() << "*" , "adspacetype");
							SResultSet rs = SDBM->execQuery(&typesSpaces);
	
							QList<QVariant> listTypeSpaces;
							for(int pos =0; pos < rs.map()["type"].count(); pos++)
							{	
								listTypeSpaces.append(QVariant(rs.map()["type"][pos]));
							}
							
							ADEvent event(ADEvent::Server,Logic::Spaces, Logic::GetTypes, listTypeSpaces);
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
						case Logic::Add:
						{
							SHOW_VAR(event->toString());
							
							ADAudioVisual *audiovisual = qvariant_cast<ADAudioVisual *>(event->data());
							ADInsert insert("adaudiovisual", QStringList()<< "typeav" << "marksequipmentav" << "estateav" << "numberinventoryav" << "codespace", QStringList() <<  SQLSTR(audiovisual->type()) <<  SQLSTR(audiovisual->marksEquipment()) <<  SQLSTR(audiovisual->state()) << SQLSTR(audiovisual->numberInventory()) << SQLSTR(audiovisual->codeSpace()));
							SDBM->execQuery(&insert);
							
							if ( SDBM->lastError().isValid() )
							{
								cnx->sendToClient( PostgresErrorHandler::handle( SDBM->lastError() ) );
							}
							else
							{
								ADEvent e( ADEvent::Server, Logic::Audiovisuals, Logic::Add, event->data());
								sendToAll(e.toString());
							}
						}
						break;
						case Logic::Del:
						{
							ADDelete qdelete("adaudiovisual");
							qdelete.setWhere( "numberinventoryav = " + SQLSTR(event->data().toString()));
							SDBM->execQuery(&qdelete);
							if ( SDBM->lastError().isValid() )
							{
								cnx->sendToClient( PostgresErrorHandler::handle( SDBM->lastError() ) );
							}
							else
							{
								ADEvent e( ADEvent::Server, Logic::Audiovisuals, Logic::Del, event->data());
								sendToAll(e.toString());
							}
						}
						break;
						case Logic::Update:
						{
							event->toString();
							
							ADAudioVisual *audiovisual = qvariant_cast<ADAudioVisual *>(event->data());
							ADUpdate update("adaudiovisual", QStringList()<< "typeav" << "marksequipmentav" << "estateav" << "numberinventoryav" << "codespace", QStringList() <<  SQLSTR(audiovisual->type()) <<  SQLSTR(audiovisual->marksEquipment()) <<  SQLSTR(audiovisual->state()) << SQLSTR(audiovisual->numberInventory()) << SQLSTR(audiovisual->codeSpace()));
							update.setWhere( "numberinventoryav = " + SQLSTR(audiovisual->numberInventory()));
							SDBM->execQuery(&update);
							
							if ( SDBM->lastError().isValid() )
							{
								cnx->sendToClient( PostgresErrorHandler::handle( SDBM->lastError() ) );
							}
							else
							{
								ADEvent e( ADEvent::Server, Logic::Audiovisuals, Logic::Update, event->data());
								sendToAll(e.toString());
							}
						}
						break;
						case Logic::Find:
						{
							ADSelect infoAv(QStringList() << "*" , "adaudiovisual");
							if(event->data() !="all" )
							{
								QString condition = qvariant_cast<QString>(event->data());
								infoAv.setWhere(condition);
							}
							
							SResultSet rs = SDBM->execQuery(&infoAv);
							QList<QVariant> listAvs;
							
							for(int pos =0; pos < rs.map()["numberinventoryav"].count(); pos++)
							{
								
								ADAudioVisual *av = new ADAudioVisual(rs.map()["typeav"][pos], rs.map()["marksequipmentav"][pos], rs.map()["estateav"][pos], rs.map()["numberinventoryav"][pos], rs.map()["codespace"][pos]);
								listAvs.append(QVariant::fromValue (av));
							}
							
							ADEvent event(ADEvent::Server,Logic::Audiovisuals, Logic::Find, listAvs);
							cnx->sendToClient(event.toString());
							break;
						}
						
						case Logic::GetTypes:
						{
							ADSelect typesAudioVisual(QStringList() << "*" , "adaudiovisualtype");
							SResultSet rs = SDBM->execQuery(&typesAudioVisual);
	
							QList<QVariant> listTypeAudiovisual;
							for(int pos =0; pos < rs.map()["type"].count(); pos++)
							{	
								listTypeAudiovisual.append(QVariant(rs.map()["type"][pos]));
							}
							
							ADEvent event(ADEvent::Server,Logic::Audiovisuals, Logic::GetTypes, listTypeAudiovisual);
							cnx->sendToClient(event.toString());
							break;
						}
						
					}
					break;
				}
				break;
				case Logic::ReservesF:
				{
					
					switch(event->action())
					{
						case Logic::Add:
						{
							event->toString();
							ADReserve *reserve = qvariant_cast<ADReserve *>(event->data());
							
							ADInsert insert("adreserve", QStringList()<< "typereserve" << "iduserreserve" << "iduserresponsable" << "idspace" << "idaudiovisual" << "day" << "beginhour" << "endhour" << "begindate" << "enddate" << "isactive" << "destinationreserve", QStringList() << SQLSTR(reserve->typeReserve()) <<  SQLSTR(reserve->iduserreserve()) << SQLSTR(reserve->iduserresponsable()) << SQLSTR(reserve->idspace()) << SQLSTR(reserve->idaudiovisual()) << SQLSTR(reserve->day()) << SQLSTR(reserve->beginDateTime().time().toString("hh:mm")) << SQLSTR(reserve->endDateTime().time().toString("hh:mm")) << SQLSTR(reserve->beginDateTime().date().toString("yyyy-MM-dd")) << SQLSTR(reserve->endDateTime().date().toString("yyyy-MM-dd")) << SQLSTR(reserve->isActive()) << SQLSTR(reserve->destinationreserve()));
							SDBM->execQuery(&insert);
							
							if ( SDBM->lastError().isValid() )
							{
								cnx->sendToClient( PostgresErrorHandler::handle( SDBM->lastError() ) );
							}
							else
							{
								ADSelect idReserve(QStringList() << "idreserve", "adreserve");
								QString condition = "typereserve = 'semestral' and isactive = true and  idspace = "+SQLSTR(reserve->idspace())+" and idaudiovisual = "+ SQLSTR(reserve->idaudiovisual()) + " and day = "+ SQLSTR(reserve->day()) + " and beginhour = "+SQLSTR(reserve->beginDateTime().time().toString("hh:mm"))+ " and endHour = "+SQLSTR(reserve->endDateTime().time().toString("hh:mm"));
								idReserve.setWhere(condition);
								
								SResultSet rs = SDBM->execQuery(&idReserve);
								reserve->setIdReserve(rs.map()["idreserve"][0]);
								
								ADEvent e( ADEvent::Server, Logic::ReservesF, Logic::Add, QVariant::fromValue(reserve));
								sendToAll(e.toString());
							}
						}
						break;
						case Logic::Del:
						{
							ADCancellation *cancel = qvariant_cast <ADCancellation *>(event->data());
							ADInsert insertCancel("adcancellation", QStringList()<< "idcancellation" << "idUserCancellation" << "hourCancellation" << "dateCancellation" << "razonCancellation", QStringList() << SQLSTR(cancel->idReserveCancellation()) << SQLSTR(cancel->idUserCancellation()) << SQLSTR(cancel->dateTimeCancellation().time().toString("hh:mm")) << SQLSTR(cancel->dateTimeCancellation().date().toString("yyyy-MM-dd")) << SQLSTR(cancel->razonCancellation()));
						
							SDBM->execQuery(&insertCancel);
							
							if ( SDBM->lastError().isValid() )
							{
								cnx->sendToClient( PostgresErrorHandler::handle( SDBM->lastError() ) );
							}
							else
							{
								ADUpdate updateReserve("adreserve", QStringList() << "isactive",  QStringList() << SQLSTR("false"));
								updateReserve.setWhere( "idreserve = " + SQLSTR(cancel->idReserveCancellation()));
								
								SResultSet rs = SDBM->execQuery(&updateReserve);
								
								ADEvent e( ADEvent::Server, Logic::ReservesF, Logic::Del,  QVariant::fromValue(cancel));
								sendToAll(e.toString());
							}
						}
						break;
						case Logic::Update:
						{
							/// En el caso que ya hallan hecho una cancelacion de la reserva no se debe de actualizar
							ADReserve *reserve = qvariant_cast<ADReserve *>(event->data());
							ADSelect infoCancellation(QStringList() << "idcancellation", "adcancellation");
							infoCancellation.setWhere("idcancellation = "+SQLSTR(reserve->idReserve()));
							
							SResultSet rs = SDBM->execQuery(&infoCancellation);
							if(rs.map()["idcancellation"].count() < 1)
							{
								ADUpdate updateReserve("adreserve", QStringList() << "day" <<"beginhour" << "endhour" << "begindate" << "enddate" << "destinationreserve",  QStringList() << SQLSTR(reserve->day()) << SQLSTR(reserve->beginDateTime().time().toString("hh:mm")) << SQLSTR(reserve->endDateTime().time().toString("hh:mm")) << SQLSTR(reserve->beginDateTime().date().toString("yyyy-MM-dd")) << SQLSTR(reserve->endDateTime().date().toString("yyyy-MM-dd")) << SQLSTR(reserve->destinationreserve()));
								
								updateReserve.setWhere( "idreserve = " + SQLSTR(reserve->idReserve()));
								SDBM->execQuery(&updateReserve);
								if ( SDBM->lastError().isValid() )
								{
									cnx->sendToClient( PostgresErrorHandler::handle( SDBM->lastError() ) );
								}
								else
								{
									ADEvent e( ADEvent::Server, Logic::ReservesF, Logic::Update, event->data());
									sendToAll(e.toString());
								}
							}
							else
							{
								cnx->sendToClient( "No se puede actualizar una reserva que ha sido eliminada anteriormente");
							}
						}
						break;
						
						case Logic::Find:
						{	
							ADSelect infoReserves(QStringList() << "*", "adreserve");
							if(event->data() == "all" )
							{
								QString condition = "typereserve = 'semestral' and isactive = true";
								infoReserves.setWhere(condition);
							}
							
							SResultSet rs = SDBM->execQuery(&infoReserves);
							QList<QVariant> listReservesF;
							
							for(int pos =0; pos < rs.map()["typereserve"].count(); pos++)
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
								
								ADReserve *avReserve = new ADReserve(rs.map()["idreserve"][pos], rs.map()["typereserve"][pos], rs.map()["iduserreserve"][pos], rs.map()["iduserresponsable"][pos], rs.map()["idaudiovisual"][pos], rs.map()["idspace"][pos], rs.map()["day"][pos], QDateTime( QDate::fromString( rs.map()["begindate"][pos], "yyyy-MM-dd"), QTime::fromString( rs.map()["beginhour"][pos], "hh:mm:ss")) ,QDateTime( QDate::fromString( rs.map()["enddate"][pos],"yyyy-MM-dd"), QTime::fromString( rs.map()["endhour"][pos], "hh:mm:ss") ), active, rs.map()["destinationreserve"][pos]);
								
								listReservesF.append(QVariant::fromValue (avReserve));
							}
							
							ADEvent event(ADEvent::Server,Logic::ReservesF, Logic::Find, listReservesF);
							cnx->sendToClient(event.toString());
							
							break;
						}
						case Logic::Dates:
						{
							ADSelect infoDatesSemestral(QStringList() << "begindateSem" << "enddatesem", "ConfigurationSchooll");
							
							SResultSet rs = SDBM->execQuery(&infoDatesSemestral);
							if(!rs.map().isEmpty())
							{
								QList<QVariant> listDatesSem;
								listDatesSem.append(QVariant(rs.map()["begindatesem"][0]));
								listDatesSem.append(QVariant(rs.map()["enddatesem"][0]));
							
								ADEvent event(ADEvent::Server,Logic::ReservesF, Logic::Dates, listDatesSem);
								cnx->sendToClient(event.toString());
							}
							break;
						}
					}
					break;
				}
				break;
				case Logic::ReservesT:
				{
					switch(event->action())
					{
						case Logic::Add:
						{
							event->toString();
							ADReserve *reserve = qvariant_cast<ADReserve *>(event->data());
							
							ADInsert insert("adreserve", QStringList()<< "typereserve" << "iduserreserve" << "iduserresponsable" << "idspace" << "idaudiovisual" << "day" << "beginhour" << "endhour" << "begindate" << "enddate" << "isactive" << "destinationreserve", QStringList() << SQLSTR(reserve->typeReserve()) <<  SQLSTR(reserve->iduserreserve()) << SQLSTR(reserve->iduserresponsable()) << SQLSTR(reserve->idspace()) << SQLSTR(reserve->idaudiovisual()) << SQLSTR(reserve->day()) << SQLSTR(reserve->beginDateTime().time().toString("hh:mm")) << SQLSTR(reserve->endDateTime().time().toString("hh:mm")) << SQLSTR(reserve->beginDateTime().date().toString("yyyy-MM-dd")) << SQLSTR(reserve->endDateTime().date().toString("yyyy-MM-dd")) << SQLSTR(reserve->isActive()) << SQLSTR(reserve->destinationreserve()));
							SDBM->execQuery(&insert);
							
							if ( SDBM->lastError().isValid() )
							{
								cnx->sendToClient( PostgresErrorHandler::handle( SDBM->lastError() ) );
							}
							else
							{
								ADSelect idReserve(QStringList() << "idreserve", "adreserve");
								QString condition = "typereserve = 'temporal' and isactive = true and  idspace = "+SQLSTR(reserve->idspace())+" and idaudiovisual = "+ SQLSTR(reserve->idaudiovisual()) + " and day = "+ SQLSTR(reserve->day()) + " and beginhour = "+ SQLSTR(reserve->beginDateTime().time().toString("hh:mm"))+ " and endHour = "+SQLSTR(reserve->endDateTime().time().toString("hh:mm"));
								idReserve.setWhere(condition);
								
								SResultSet rs = SDBM->execQuery(&idReserve);
								reserve->setIdReserve(rs.map()["idreserve"][0]);
								
								
								ADEvent e( ADEvent::Server, Logic::ReservesT, Logic::Add, QVariant::fromValue(reserve));
								sendToAll(e.toString());
							}
						}
						break;
						case Logic::Del:
						{
							ADCancellation *cancel = qvariant_cast <ADCancellation *>(event->data());
							ADInsert insertCancel("adcancellation", QStringList()<< "idcancellation" << "idUserCancellation" << "hourCancellation" << "dateCancellation" << "razonCancellation", QStringList() << SQLSTR(cancel->idReserveCancellation()) << SQLSTR(cancel->idUserCancellation()) << SQLSTR(cancel->dateTimeCancellation().time().toString("hh:mm")) << SQLSTR(cancel->dateTimeCancellation().date().toString("yyyy-MM-dd")) << SQLSTR(cancel->razonCancellation()));
						
							SDBM->execQuery(&insertCancel);
							
							if ( SDBM->lastError().isValid() )
							{
								cnx->sendToClient( PostgresErrorHandler::handle( SDBM->lastError() ) );
							}
							else
							{
								ADUpdate updateReserve("adreserve", QStringList() << "isactive",  QStringList() << SQLSTR("false"));
								updateReserve.setWhere( "idreserve = " + SQLSTR(cancel->idReserveCancellation()));
								
								SResultSet rs = SDBM->execQuery(&updateReserve);
								
								ADEvent e( ADEvent::Server, Logic::ReservesT, Logic::Del,  QVariant::fromValue(cancel));
								sendToAll(e.toString());
							}
						}
						break;
						case Logic::Update:
						{
							/// En el caso que ya hallan hecho una cancelacion de la reserva no se debe de actualizar
							ADReserve *reserve = qvariant_cast<ADReserve *>(event->data());
							ADSelect infoCancellation(QStringList() << "idcancellation", "adcancellation");
							infoCancellation.setWhere("idcancellation = "+SQLSTR(reserve->idReserve()));
							
							SResultSet rs = SDBM->execQuery(&infoCancellation);
							if(rs.map()["idcancellation"].count() < 1)
							{
								ADUpdate updateReserve("adreserve", QStringList() << "day" <<"beginhour" << "endhour" << "begindate" << "enddate" << "destinationreserve",  QStringList() << SQLSTR(reserve->day()) << SQLSTR(reserve->beginDateTime().time().toString("hh:mm")) << SQLSTR(reserve->endDateTime().time().toString("hh:mm")) << SQLSTR(reserve->beginDateTime().date().toString("yyyy-MM-dd")) << SQLSTR(reserve->endDateTime().date().toString("yyyy-MM-dd")) << SQLSTR(reserve->destinationreserve()));
								
								updateReserve.setWhere( "idreserve = " + SQLSTR(reserve->idReserve()));
								SDBM->execQuery(&updateReserve);
								if ( SDBM->lastError().isValid() )
								{
									cnx->sendToClient( PostgresErrorHandler::handle( SDBM->lastError() ) );
								}
								else
								{
									ADEvent e( ADEvent::Server, Logic::ReservesT, Logic::Update, event->data());
									sendToAll(e.toString());
								}
							}
							else
							{
								cnx->sendToClient( "No se puede actualizar una reserva que ha sido eliminada anteriormente");
							}
						}
						break;
						
						
						case Logic::Find:
						{
							ADSelect infoReserves(QStringList() << "*", "adreserve");
							if(event->data() == "all" )
							{
								QString condition = "typereserve = 'temporal' and isactive = true";
								infoReserves.setWhere(condition);
							}
							
							SResultSet rs = SDBM->execQuery(&infoReserves);
							QList<QVariant> listReservesT;
							
							for(int pos =0; pos < rs.map()["typereserve"].count(); pos++)
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

								ADReserve *avReserve = new ADReserve(rs.map()["idreserve"][pos],
										rs.map()["typereserve"][pos], rs.map()["iduserreserve"][pos],
										rs.map()["iduserresponsable"][pos],
										rs.map()["idaudiovisual"][pos], rs.map()["idspace"][pos],
										rs.map()["day"][pos], QDateTime( QDate::fromString(
										rs.map()["begindate"][pos],"yyyy-MM-dd" ), QTime::fromString(
										rs.map()["beginhour"][pos],"hh:mm:ss")),
										QDateTime(QDate::fromString( rs.map()["enddate"][pos], "yyyy-MM-dd"),
										QTime::fromString( rs.map()["endhour"][pos],"hh:mm:ss")),
										active, rs.map()["destinationreserve"][pos]);
								
								
								listReservesT.append(QVariant::fromValue (avReserve));
							}
							
							ADEvent event(ADEvent::Server, Logic::ReservesT, Logic::Find, listReservesT);
							cnx->sendToClient(event.toString());
							
						}
						break;
					}
				}
				break;
				case Logic::Cancellation:
				{
					switch(event->action())
					{
						case Logic::Find:
						{
							ADSelect infoCancellation(QStringList() << "*", "adcancellation");
							if(event->data() != "all" )
							{
								QString condition = qvariant_cast<QString>(event->data());
								infoCancellation.setWhere(condition);
							}
							
							SResultSet rs = SDBM->execQuery(&infoCancellation);
							QList<QVariant> listCancellations;
							
							for(int pos =0; pos < rs.map()["idcancellation"].count(); pos++)
							{
								ADCancellation *cancel = new ADCancellation(
									rs.map()["idcancellation"][pos],
									rs.map()["idusercancellation"][pos],
									QDateTime( QDate::fromString(rs.map()["datecancellation"][pos], "yyyy-MM-dd" ), 
									QTime::fromString( rs.map()["hourcancellation"][pos],"hh:mm:ss")),
									rs.map()["razoncancellation"][pos]);
								
								
								listCancellations.append(QVariant::fromValue (cancel));
							}
							ADEvent event(ADEvent::Server, Logic::Cancellation, Logic::Find, listCancellations);
							cnx->sendToClient(event.toString());
							
						}
						break;
					}
				}
				break;
				case Logic::Reports:
				{
					switch(event->action())
					{
						case Logic::Add:
						{
							ADReport *report = qvariant_cast<ADReport *>(event->data());
							
							addReport(cnx, report);
						
						}
						break;
						case Logic::Del:
						{
							ADDelete deleteReport("adreport");
							QStringList keys = event->data().toString().split( '.' );
							deleteReport.setWhere("creator = " + SQLSTR(keys[0]) + " and " + "created = " + SQLSTR(keys[1]) );
							SDBM->execQuery(&deleteReport);
							if ( SDBM->lastError().isValid() )
							{
								cnx->sendToClient( PostgresErrorHandler::handle( SDBM->lastError()));
							}
							else
							{
								ADEvent e( ADEvent::Server, Logic::Reports, Logic::Del, event->data());
								sendToAll(e.toString());
							}
						}
						break;
						case Logic::Find:
						{
							if( event->data().toString() == "all"  )
							{
								ADSelect infoReport(QStringList() << "*" , "adreport");
								
								SResultSet rs = SDBM->execQuery(&infoReport);
								QList<QVariant> list;
								
								for(int pos =0; pos < rs.map()["creator"].count(); pos++)
								{
									ADReport *report = new  ADReport( rs.map()["creator"][pos],
										ADReport::TypeConsult(rs.map()["consult"][pos].toInt()),
										ADReport::TypeReport(rs.map()["typereport"][pos].toInt()),
										QDate::fromString(rs.map()["begindate"][pos], Qt::ISODate),
										QDate::fromString(rs.map()["enddate"][pos],Qt::ISODate),
										QDateTime::fromString(rs.map()["created"][pos], Qt::ISODate));
									
									report->setContent(rs.map()["content"][pos]);
									list.append(QVariant::fromValue(report));
								}
								ADEvent event(ADEvent::Server, Logic::Reports, Logic::Find, list);
								cnx->sendToClient(event.toString());
							}
						}
					}
					break;
				}
				break;
			}
		}
	}
	else
	{
		dFatal() << "no existe el evento";
	}
}

void ADServer::addReport(ADServerConnection *cnx, ADReport *report)
{
	//FIXME: AQUI ES DONDE SE DEBEN HACER LAS CONSULTAS PARA LLENAR EL REPORTE
	QStringList headers;
	SResultSet rs;
	switch(report->consult())
	{
		case ADReport::TimeAudio:
		{
			ADSelect select( QStringList() << "idAudioVisual as ref" << "day" << "beginHour"  << "endHour" << "beginDate" << "endDate", QStringList() << "adreserve");
			QMap<QString, QStringList> query = SDBM->execQuery(&select).map();
			select.setWhere( "idSpace='null'" );
			
// 			DROP TABLE AVTemp;
// 			CREATE TABLE AVTemp(idAudioVisual varchar(20), horas integer); 
			SDBM->dropTable("AVTemp");
			SDBM->createTable( "AVTemp", QStringList() << "idAudioVisual" << "totalDays" << "beginHour" << "endHour", QStringList() <<  "varchar(20)" << "integer"<< "time" << "time");
			QStringList days;
			for(int i = 1; i < 8; i++)
			{
				days << QDate::longDayName ( i );
			}
			
			if(!query.isEmpty())
			{
				for(int i = 0; i < query["ref"].count(); i++)
				{
					QString idA = query["ref"][i];
					int day = days.indexOf(query["day"][i]) + 1;
					//dDebug() << "dia " << query["day"][i];
					QTime beginHour = QTime::fromString(query["beginhour"][i], Qt::ISODate);
					QTime endHour = QTime::fromString(query["endhour"][i], Qt::ISODate);
					QDate beginDate = QDate::fromString(query["begindate"][i], Qt::ISODate);
					QDate endDate = QDate::fromString(query["enddate"][i], Qt::ISODate);
					int total =0 ;
					if(beginDate>= report->beginDate() && endDate<= report->endDate())
					{
						total = calculateTotalDay(beginDate, endDate, day);
						dDebug() << "111 Begin > INBeg y End < INEnd " << day;
					}
					else if(beginDate>=report->beginDate() && endDate>=report->endDate())
					{
						
						total=calculateTotalDay(beginDate, report->endDate(), day);
						dDebug() << "222 Begin > INBeg y End > INEnd " << day;
					}
					else if(beginDate <= report->beginDate() && endDate <= report->endDate())
					{
						dDebug() << "333 Begin < INBeg y End < INEnd " << day;
						total=calculateTotalDay( report->beginDate(), endDate, day);
						dDebug() << "Total de Dias " << total;
					}
					else if(beginDate<=report->beginDate() && endDate >= report->endDate())
					{
						total=calculateTotalDay(report->beginDate(), report->endDate(), day);
						dDebug() << "444 Begin < INBeg y End > INEnd " << day;
					}
					else
					{
						total=0;
					}
					QTime timeReserve( endHour.hour() - beginHour.hour(),  endHour. minute() - beginHour.minute(), endHour.second() - beginHour.second());
					
					ADInsert insert("AVTemp", QStringList() << "idAudioVisual" << "totalDays" << "beginHour" << "endHour", QStringList() << SQLSTR( idA ) << QString::number(total) << SQLSTR(beginHour.toString(Qt::ISODate)) << SQLSTR(endHour.toString(Qt::ISODate)));
					SDBM->execQuery(&insert);
				}
			}
			//rs = SDBM->execQuery(&consult);
			//SELECT sum(totaldays*(endHour-beginHour)) AS cantHoras, idAudioVisual FROM AVTemp GROUP BY idAudioVisual;
			ADSelect consult(QStringList() << "idAudioVisual" << "sum(totaldays*(endHour-beginHour)) as cant", QStringList() << "avtemp");
			consult.groupBy("idAudioVisual");
			rs = SDBM->execQuery(&consult);
			headers << "Id" << "horas" ;
			dDebug() << "Consultando lista de tiempos de uso ayudas audiovisuales";
		}
		break;
		case ADReport::TimeProfesor:
		{
// Reporte en PDF y pantalla de los profesores con mayor número de horas reservadas en un periodo de tiempo.
			dDebug() << "Consultando lista de tiempos de reservas por profesor";
			
		//	SELECT idUserResponsable, beginHour, endHour, beginDate, endDate FROM ADReserve, ADUser 
		//		WHERE idUserResponsable=loginUser and rol=1;
			
//	MIRAR PORQUE EL SELECTWHERRE DEBAJO DLE QMAP
			ADSelect select( QStringList() << "loginUser as ref" << "nameUser" << "beginHour"  << "endHour" << "beginDate" << "endDate", QStringList() << "adreserve" << "aduser");
			QMap<QString, QStringList> query = SDBM->execQuery(&select).map();
			select.setWhere("idUserResponsable = loginUser and rol = 1");
			
			
// 			DROP TABLE AVTemp;
// 			CREATE TABLE AVTemp(idAudioVisual varchar(20), horas integer); 
			SDBM->dropTable("AVTemp");
			SDBM->createTable( "AVTemp", QStringList() << "loginUser" << "nameUser" << "totalDays" << "beginHour" << "endHour", QStringList() << "varchar(20)" << "varchar(50)" << "integer"<< "time" << "time");
			QStringList days;
			for(int i = 1; i < 8; i++)
			{
				days << QDate::longDayName ( i );
			}
			
			if(!query.isEmpty())
			{
				for(int i = 0; i < query["ref"].count(); i++)
				{
					QString loginUs = query["ref"][i];
					int day = days.indexOf(query["day"][i]) + 1;
					//dDebug() << "dia " << query["day"][i];
					QString nombre = query["nameUser"][i];
					QTime beginHour = QTime::fromString(query["beginhour"][i], Qt::ISODate);
					QTime endHour = QTime::fromString(query["endhour"][i], Qt::ISODate);
					QDate beginDate = QDate::fromString(query["begindate"][i], Qt::ISODate);
					QDate endDate = QDate::fromString(query["enddate"][i], Qt::ISODate);
					int total =0 ;
					if(beginDate>= report->beginDate() && endDate<= report->endDate())
					{
						dDebug() << "111 Begin > INBeg y End < INEnd " << day;
						total = calculateTotalDay(beginDate, endDate, day);
						dDebug() << "Total de Dias " << total;
					}
					else if(beginDate>=report->beginDate() && endDate>=report->endDate())
					{
						dDebug() << "222 Begin > INBeg y End > INEnd " << day;
						total=calculateTotalDay(beginDate, report->endDate(), day);
						dDebug() << "Total de Dias " << total;
					}
					else if(beginDate <= report->beginDate() && endDate <= report->endDate())
					{
						dDebug() << "333 Begin < INBeg y End < INEnd " << day;
						total=calculateTotalDay( report->beginDate(), endDate, day);
						dDebug() << "Total de Dias " << total;
					}
					else if(beginDate<=report->beginDate() && endDate >= report->endDate())
					{
						dDebug() << "444 Begin < INBeg y End > INEnd " << day;
						total=calculateTotalDay(report->beginDate(), report->endDate(), day);
						dDebug() << "Total de Dias " << total;
					}
					else
					{
						total=0;
					}
					//QTime timeReserve( endHour.hour() - beginHour.hour(),  endHour. minute() - beginHour.minute(), endHour.second() - beginHour.second());
					
					ADInsert insert("AVTemp", QStringList() << "loginUser" << "nameUser" << "totalDays" << "beginHour" << "endHour", QStringList() << SQLSTR( loginUs ) << SQLSTR( nombre ) << QString::number(total) << SQLSTR(beginHour.toString(Qt::ISODate)) << SQLSTR(endHour.toString(Qt::ISODate)));
					SDBM->execQuery(&insert);
				}
			}
			//rs = SDBM->execQuery(&consult);
			//SELECT sum(totaldays*(endHour-beginHour)) AS cantHoras, nameUser FROM AVTemp GROUP BY nameUser;
			ADSelect consult(QStringList() << "nameUser" << "sum(totaldays*(endHour-beginHour)) as cant", QStringList() << "avtemp");
			consult.groupBy("nameUser");
			rs = SDBM->execQuery(&consult);
			headers << "Horas" << "Profesor" ;

			//MIRAR SI PUEDO QUITAR EL LOGIN DE ALGUNAS TABLAS Y CONSULTAS
			dDebug() << "Consultando lista de tiempos de reservas por profesor";
		}
		break;
		case ADReport::Cancelations:
		{
			dDebug() << "Consultatando lista de cancelaciones";
			headers << "Id" << "Usuario" << "Hora" << "Fecha" << "Razï¿½";
			
			ADSelect consult(QStringList() << "*", "ADCancellation");
			consult.setWhere( "dateCancellation > "+  SQLSTR(report->beginDate().toString(Qt::ISODate) ) + " and " +  "dateCancellation < " + SQLSTR(report->endDate().toString(Qt::ISODate) ) );
			rs =  SDBM->execQuery(&consult);
		}
		break;
	}
		
	if(report->type() == ADReport::List)
	{
		QTextDocument *content = ADReportGenerator::generateListReport(report->beginDate(), report->endDate(), rs, headers);
		
		
		report->setContent(content->toHtml());
	}
	else if(report->type() == ADReport::Histogram)
	{
		//simulacion de la consulta de audiovisuales versus tiempo
		ADSelect select( QStringList() << "idaudiovisual as ref" << "sum(horas)", QStringList() << "(SELECT idaudiovisual, (endhour-beginhour) as horas from adreserve) as foo");
		
		select.groupBy("ref");
// 		select.addSubConsult(QString connector, const ADSelect &subconsult);
		QMap<QString, QStringList> map = SDBM->execQuery(&select).map();
		QStringList ref = map["ref"];
		QStringList time = map["sum"];
			
		QMap<QString, int> hoursAv;
			
		for(int i =0; i < ref.count() && i < time.count() ; i++)
		{
			dDebug() << time[i];
			hoursAv.insert(ref[i], QTime::fromString( time[i], Qt::ISODate ).hour() );
		}
// 			SELECT numberinventoryav as ref, sum(horas) from (SELECT idaudiovisual, (endhour-beginhour) as horas from adreserve) as foo, adaudiovisual where(foo.idaudiovisual = adaudiovisual.numberinventoryav) group by ref;
								
		Histogram histogram(QRect(QPoint(0,0), QSize(500,500)));
		histogram.setValues(hoursAv);
		report->setContent(histogram.toString());
		
	}
	
	ADInsert insert("adreport", QStringList() << "creator" << "typeReport" << "consult" << "begindate" << "enddate" << "created" << "content", QStringList() << SQLSTR(report->creator() ) << QString::number(report->type()) << QString::number(report->consult()) << SQLSTR(report->beginDate().toString(Qt::ISODate) ) << SQLSTR(report->endDate().toString(Qt::ISODate) ) << SQLSTR(report->created().toString(Qt::ISODate) ) << SQLSTR(report->content()));
	
	
	SDBM->execQuery(&insert);
	if ( SDBM->lastError().isValid() )
	{
		cnx->sendToClient( PostgresErrorHandler::handle( SDBM->lastError() ) );
	}
	else
	{
		ADEvent event(ADEvent::Server, Logic::Reports, Logic::Add, QVariant::fromValue ( report ));
		cnx->sendToClient(event.toString());
	}
}


int ADServer::calculateTotalDay(const QDate & beginDate, const QDate &endDate, int day)
{
	int diasEntreInicioYFin = beginDate.daysTo(endDate) + 1;
	
	int inicio = beginDate.dayOfWeek();
	dDebug() << "diasEntreInicioYFin " << diasEntreInicioYFin << " beginDate " << beginDate << "endDate" << endDate << " inicio " << inicio;
	
	if(day < inicio)
	{
		return ::floor((diasEntreInicioYFin-(6-inicio+day))/7) + 1;
	}
	else
	{
		return ::floor((diasEntreInicioYFin-(day-inicio))/7) + 1;
	}
}

