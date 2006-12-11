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
#include "adevent.h"
#include <QDomDocument>

#include <ddebug.h>

#include "aduser.h"
#include "adaudiovisual.h"
#include "adreserve.h"
#include "adspace.h"
#include "adcancellation.h"
#include "adreport.h"

#include <QVariant>

ADEvent::ADEvent()
{
	m_valid = false;
}


ADEvent::ADEvent(Source source, Logic::Module module, Logic::Action action, const QVariant  & data):  m_source(source), m_module(module), m_action(action), m_data(data)
{
	m_valid = true;
}


ADEvent::~ADEvent()
{
}

QString ADEvent::toString() const
{
	D_FUNCINFO;
	QDomDocument doc;
	QDomElement root = doc.createElement ( "Event" );
	doc.appendChild(root);
	
	QDomElement sourceE = doc.createElement( "Source" );
	sourceE.setAttribute("value", int(source()));
	root.appendChild(sourceE);
	
	QDomElement moduleE = doc.createElement( "Module" );
	moduleE.setAttribute("value", int(module()));
	root.appendChild(moduleE);
	
	QDomElement actionE = doc.createElement( "Action" );
	actionE.setAttribute("value", int(action()));
	root.appendChild(actionE);
	
	QDomElement dataE = doc.createElement( "Data" );
	SHOW_VAR(m_action); 
	if(m_action == Logic::Add || m_action == Logic::Update)
	{
		switch( m_module )
		{
			case Logic::Users:
			{
				dataE.appendChild( qvariant_cast<ADUser *>( m_data )->toXml(doc) );
			}
			break;
			case Logic::ReservesF:
			{
				dataE.appendChild( qvariant_cast<ADReserve *>( m_data )->toXml(doc) );
			}
			break;
			case Logic::ReservesT:
			{
				dataE.appendChild( qvariant_cast<ADReserve *>( m_data )->toXml(doc) );
			}
			break;
			case Logic::Audiovisuals:
			{
				dataE.appendChild( qvariant_cast<ADAudioVisual *>( m_data )->toXml(doc) );
			}
			break;
			case Logic::Spaces:
			{
				dataE.appendChild( qvariant_cast<ADSpace *>( m_data )->toXml(doc) );
			}
			break;
			case Logic::Reports:
			{
				dataE.appendChild( qvariant_cast<ADReport *>( m_data )->toXml(doc) );
			}
			break;
		}
	}
	else
	{
		if(m_source == Client)
		{
			switch(m_action)
			{
				case Logic::Find:
				{
					QDomElement conditionE = doc.createElement ( "Condition" );
					conditionE.setAttribute("value", m_data.toString());
					dataE.appendChild(conditionE);
				}
				break;
				case Logic::Authenticate:
				{
					dataE.setAttribute("user", m_data.toList()[0].toString());
					dataE.setAttribute("passwd", m_data.toList()[1].toString());
				}
				break;
				case Logic::Del:
				{
					if(m_module == Logic::ReservesF || m_module == Logic::ReservesT)
					{
						dataE.appendChild( qvariant_cast<ADCancellation *>( m_data )->toXml(doc) );
					}
					else
					{
						QDomElement keyE = doc.createElement ( "Key" );
						keyE.setAttribute("value", m_data.toString());
						dataE.appendChild(keyE);
					}
				}
				break;
			}
		}
		else	// SERVER
		{
			switch(m_action)
			{
				case Logic::Find:
				{
					QDomElement listE = doc.createElement ( "List" );
					foreach(QVariant var, m_data.toList() )
					{
						switch(m_module)
						{
							case Logic::Users:
							{
								listE.appendChild( qvariant_cast<ADUser *>( var )->toXml(doc) );
							}
							break;
							case Logic::Audiovisuals:
							{
								listE.appendChild( qvariant_cast<ADAudioVisual *>( var )->toXml(doc) );
							}
							break;
							case Logic::ReservesT:
							{
								listE.appendChild( qvariant_cast<ADReserve*>( var )->toXml(doc) );
							}
							break;
							case Logic::ReservesF:
							{
								listE.appendChild( qvariant_cast<ADReserve*>( var )->toXml(doc) );
							}
							break;
							case Logic::Spaces:
							{
								listE.appendChild( qvariant_cast<ADSpace *>( var )->toXml(doc) );
							}
							break;
							case Logic::Reports:
							{
								dDebug() << "here";
								listE.appendChild( qvariant_cast<ADReport *>( var )->toXml(doc) );
							}
							break;
							case Logic::Cancellation:
							{
								listE.appendChild( qvariant_cast<ADCancellation *>( var )->toXml(doc) );
							}
							break;
						}
					}
					dataE.appendChild(listE);
				}
				break;
				case Logic::Authenticate:
				{
					dataE.appendChild( qvariant_cast<ADUser *>( m_data )->toXml(doc) );
					break;
				}
				case Logic::Dates:
				{
					QDomElement listE = doc.createElement ( "List" );
					int n=0;
					foreach(QVariant var, m_data.toList() )
					{
						switch(m_module)
						{
							case Logic::ReservesF:
							{
								QDomElement root = doc.createElement("dates");
								root.setAttribute( "date", qvariant_cast<QString>( var ) );
								
								listE.appendChild(root);
								n++;
							}
							break;
						}
					}
					dataE.appendChild(listE);
					break;
				}
				case Logic::GetTypes:
				{
					QDomElement listE = doc.createElement ( "List" );
					foreach(QVariant var, m_data.toList() )
					{
						switch(m_module)
						{
							
							case Logic::Audiovisuals:
							{
								QDomElement root = doc.createElement("types");
								root.setAttribute( "type", qvariant_cast<QString>( var ) );
								
								listE.appendChild(root);
							}
							break;
							case Logic::Spaces:
							{
								QDomElement root = doc.createElement("types");
								root.setAttribute( "type", qvariant_cast<QString>( var ) );
								
								listE.appendChild(root);
							}
							break;
							
						}
					}
					dataE.appendChild(listE);
				}
				break;
				case Logic::Del:
				{
					if(m_module == Logic::ReservesF || m_module == Logic::ReservesT)
					{
						dataE.appendChild( qvariant_cast<ADCancellation *>( m_data )->toXml(doc) );
						
					}
					else
					{
						QDomElement keyE = doc.createElement ( "Key" );
						keyE.setAttribute("value", m_data.toString());
						dataE.appendChild(keyE);
					}
				}
				break;
			}
		}
	}
	root.appendChild(dataE);
	SHOW_VAR(doc.toString());
	return doc.toString();
}

int ADEvent::source() const
{
	return m_source;
}

int ADEvent::module() const
{
	return m_module;
}

int ADEvent::action() const
{
	return m_action;
}

QVariant ADEvent::data() const
{
	return m_data;
}

void ADEvent::setData(const QVariant& data)
{
	m_data = data;
}

bool ADEvent::isValid() const 
{
	return m_valid;
}


void ADEvent::setSource(Source source)
{
	m_source = source;
}

void ADEvent::setModule(Logic::Module module)
{
	
	m_module = module;
}

void ADEvent::setAction(Logic::Action action)
{
	m_action = action;
}

