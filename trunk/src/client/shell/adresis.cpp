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
	
	connect ( m_connector, SIGNAL(sendEvent(ADEvent *)), this, SLOT(handleEvent(ADEvent *) ));
	connect ( m_connector, SIGNAL(message(Msg::Type, const QString&)), this, SIGNAL(requestShowMessage(Msg::Type, const QString&) ));
	
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
		/** ***************************************** S  E  R  V  I  D  O  R ********************************************** */
		if(event->source() == ADEvent::Server)
		{
			dDebug() << event->toString();
			
			if(event->action() == Logic::Find)
			{
				m_infoModules.insert(Logic::Module(event->module()), event->data().toList());
				emit requestShowModule( Logic::Module(event->module()),event->data().toList() );
			}
			else
			{
				switch(event->module())
				{
					case Logic::Users:
					{
// 						dDebug() << "Users action =" << event->action();
						switch(event->action())
						{
							case Logic::Authenticate:
							{
								dDebug() << "Info";
								m_user = qvariant_cast<ADUser *> (event->data());
								//TODO: enviar el evento de encontrar todos los elementos de los modulos en los cuales se tenga permisos
								for(int i=0; i < 6; i++)
								{
									Logic::Module module = Logic::Module(i);
									ADEvent findAll(ADEvent::Client, module, Logic::Find, "all");
									handleEvent(&findAll);
	
									if(Logic::Module(i) == Logic::Spaces || Logic::Module(i) == Logic::Audiovisuals)
									{
										ADEvent findType(ADEvent::Client, module, Logic::GetTypes, "");
										handleEvent(&findType);
									}
								}
							}
							break;
							case Logic::Add:
							{
								m_infoModules[Logic::Users] << (event->data());
								emit requestAddDataToModule(Logic::Users , event->data());
							}
							case Logic::Del:
							{
								QList<QVariant>::iterator it = m_infoModules[Logic::Users].begin();
								while(it != m_infoModules[Logic::Users].end())
								{
									if(qvariant_cast<ADUser *>(*it)->code() == event->data().toString())
									{
										m_infoModules[Logic::Users].erase ( it);
										break;
									}
									++it;
								}
								emit requestRemoveDataToModule(Logic::Users , event->data().toString());
							}
							break;
						}
					}
					break;
					case Logic::Spaces:
					{
						switch(event->action())
						{
							case Logic::GetTypes:
							{
								dDebug() << "GGGEEETTT TTYYYPPPEEESS de SPACES";
								m_listTypes.insert( Logic::Module(event->module()), event->data().toList() );
							}
							break;
						}
					}
					break;
					case Logic::Audiovisuals:
					{
						switch(event->action())
						{
							case Logic::GetTypes:
							{
								dDebug() << "GGGEEETTT TTYYYPPPEEESS de AUDIOVISUAL";
								m_listTypes.insert( Logic::Module(event->module()), event->data().toList() );
							}
							break;
						}
					}
					break;
					case Logic::ReservesF:
					{
					}
					break;
					case Logic::ReservesT:
					{
					}
					break;
				}
			}
		}
		/** ************************ C  L  I  E  N  T  E ************************** */
		else
		{
			if( event->action() == Logic::Find)
			{
				//CLiente 
				//se valida si la accion la puede ejecutar m_user y si es asi entonces enviarsela a m_connector
				if(m_user->permission(Logic::Module(event->module()), Logic::Action(event->action())))
				{
					m_connector->sendToServer(event->toString());
				}
			}
			else
			{
				switch(event->module())
				{
					case Logic::Users:
					{
						if(m_user->permission(Logic::Module(event->module()), Logic::Action(event->action())))
						{
							m_connector->sendToServer(event->toString());
						}
					}
					break;
					
					case Logic::Spaces:
					{
						switch(event->action())
						{
							case Logic::GetTypes:
							{
								m_connector->sendToServer(event->toString());
							}
							break;
						}
					}
					break;
					
					case Logic::Audiovisuals:
					{
						switch(event->action())
						{	
							case Logic::GetTypes:
							{
								m_connector->sendToServer(event->toString());
							}
							break;
						}
					}
					break;
					case Logic::ReservesF:
					{
						switch(event->action())
						{
							case Logic::GetTypes:
							{
								QList<QVariant > types;
								types.insert(0, QVariant(m_listTypes.values(Logic::Spaces)[0]));
								types.insert(1, QVariant(m_listTypes.values(Logic::Audiovisuals)[0]));
								
								ADEvent listTypes(ADEvent::Client, Logic::ReservesF, Logic::GetTypes, types);
								
								(qvariant_cast<ADReserveFForm *>(event->data()))->receiveEvent(&listTypes);
								
							}
							break;
							case Logic::Info:
							{
								QList<QVariant> datos = (event->data()).toList();
								
								if((datos.at(0).toString()) == "nameResources")
								{
									QMap<QString, QVariant> nameResource;
									QList<QVariant> list;
									
									if((datos.at(2).toString()) == "space")
									{
										list = m_infoModules.values(Logic::Spaces)[0];
										for(int i=0; i < list.count();i++)
										{
											ADSpace *space = qVariantValue<ADSpace *>(list.at(i));
											if(space->typeSpace() == datos.at(3).toString())
											{
												nameResource.insert(space->codeSpace(), QVariant(space->nameSpace()));
											}
										}
									}
									else
									{
										list = m_infoModules.values(Logic::Audiovisuals)[0];
										int n = 1;
										for(int i=0; i < list.count();i++)
										{
											ADAudioVisual *audiovisual = qVariantValue<ADAudioVisual *>(list.at(i));
											if(audiovisual->type() == datos.at(3).toString())
											{
												nameResource.insert(audiovisual->numberInventory(), QVariant(audiovisual->type()+" "+QString::number(n)));
												n++;
											}
										}
									}
									QList<QVariant> listResult;
									listResult << QVariant("nameResources") << QVariant(nameResource);
									ADEvent names(ADEvent::Client, Logic::ReservesF, Logic::Info, listResult);
									
									(qvariant_cast<ADReserveFForm *>(datos.at(1)))->receiveEvent(&names);
								}
								
								else if((datos.at(0).toString()) == "reservesResource")
								{
									QList<QVariant> listReserves;
									
									
									QList<QVariant> list = m_infoModules.values(Logic::ReservesF)[0];
									for(int i=0; i < list.count();i++)
									{
										ADReserve *reserve = qVariantValue<ADReserve *>(list.at(i));
										if(reserve->idspace() == datos.at(2).toString() || reserve->idaudiovisual() == datos.at(2).toString())
										{
											listReserves << list.at(i);
										}
									}
									
									QList<QVariant> listResult;
									listResult << QVariant("reserveResource") << QVariant(listReserves);
									ADEvent reserves (ADEvent::Client, Logic::ReservesF, Logic::Info, listResult);
									(qvariant_cast<ADReserveFForm *> (datos.at(1)))->receiveEvent(&reserves);
								}
							}
							break;
						}
						
					}
					break;
					
					case Logic::ReservesT:
					{
					}
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

void Adresis::connectToHost( const QString & hostName, quint16 port)
{
	m_connector->connectToHost( hostName, port);
}

//slots
void Adresis::login(const QString &user, const QString &passwd)
{
	QList<QVariant> data;
	data << user << passwd;
	ADEvent event(ADEvent::Client, Logic::Users, Logic::Authenticate, data );
	m_connector->sendToServer( event.toString() );
}

