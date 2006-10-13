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
#include "adselectpackage.h"
#include <ddebug.h>
Adresis::Adresis(QObject * parent)
	: QObject(parent)
{
	m_connector = new ADConnector(this);
	
// 	connect ( m_connector, SIGNAL(message(Msg::Type , const QString &)), this, SIGNAL(requestShowMessage( Msg::Type, const QString& )));
	
	connect ( m_connector, SIGNAL(userAutenticated(const XMLResults&)) , this, SLOT(autenticated(const XMLResults&) ));
	
	connect ( m_connector, SIGNAL(sendEvent(ADEvent *)), this, SLOT(handleEvent(ADEvent *) ));
	
// 	connect ( m_connector, SIGNAL(fillModule(Logic::Module, const QList<XMLResults>&)), this, SIGNAL(requestFillModule(Logic::Module, const QList<XMLResults>&)) );
	
// 	connect( m_connector, SIGNAL(requestShowUser(const XMLResults& )), this, SLOT(createUser(const XMLResults& )));
	
// 	connect( m_connector, SIGNAL(requestShowSpace(const XMLResults& )), this, SLOT(createSpace(const XMLResults& )));
	
// 	connect( m_connector,SIGNAL(requestShowAudiovisual(const XMLResults& )),this,SLOT(createAudiovisual(const XMLResults& )));

// 	connect ( m_connector, SIGNAL(requestShowListAudiovisual(const QList<XMLResults>&)), this, SIGNAL (requestShowListAudioVisualAD( const QList<XMLResults>&)) );
	
// 	connect ( m_connector, SIGNAL(requestListTypes(const QList<XMLResults>&)), this, SIGNAL (requestListTypesAD( const QList<XMLResults>&)) );

// 	connect ( m_connector, SIGNAL(requestSchedule (const QList<XMLResults>&)), this, SIGNAL (requestScheduleAD( const QList<XMLResults>&)) );
	
	DINIT;
}

Adresis::~Adresis()
{
	DEND;
}

void Adresis::handleEvent(ADEvent * event)
{
	D_FUNCINFO;
	if(event)
	{
		if(event->source() == ADEvent::Server)
		{
			
			switch(event->module())
			{
				case Logic::Users:
				{
					dDebug() << "Users" << event->action();
					
					switch(event->action())
					{
						
						case Logic::Info:
						{
							dDebug() << "Info";
							m_user = qvariant_cast<ADUser> (event->data());
							SHOW_VAR( m_user.name());
							
							
						}
						break;
						
					}
				}
				break;
				case Logic::Audiovisuals:
				{
					
				}
				break;
				case Logic::Reserves:
				{
					
				}
				break;
				case Logic::Spaces:
				{
					
				}
				break;
			}
		}
		else
		{
			//CLiente 
			//TODO: validar si la accion la puede ejecutar m_user y si es asi entonces enviarsela a m_connector
			m_connector->sendToServer(event->toString());
		}
	}
	else
	{
		dFatal() << "no existe el evento";
	}
	
	
	
}

void Adresis::connectToHost( const QString & hostName, quint16 port)
{
	m_connector->connectToHost( hostName, port);
}

//slots
void Adresis::login(const QString &user, const QString &passwd)
{
	m_connector->login(user, passwd);
// 	ADSelectPackage u(QStringList()<< "aduser", QStringList() << "nameuser" << "codeuser" << "loginuser"<< "passwduser" << "permissionsuser");
// 	u.setWhere( "loginuser='"+  user + "'" );
// 	m_connector->sendQuery(Logic::userAuthenticated, u);
	
}



void Adresis::autenticated(const XMLResults & values)
{
	D_FUNCINFO;
	
	m_user.setValues(values);
 	Logic::Module module;
// 	if(m_user.permissions()[Logic::administrador])
// 	{
	//TODO: enviar el evento de encontrar todos los elementos de los modulos en los cuales se tenga permisos
	for(int i=0; i < 5; i++)
	{
		Logic::Module module = Logic::Module(i);
// 		if(m_user.permissions()[module])
// 		{
			ADEvent findAll(ADEvent::Client, module, Logic::Find, "all");
			m_connector->sendToServer( findAll.toString());
// 		}
	}
// 	
}
