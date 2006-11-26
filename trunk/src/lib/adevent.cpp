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
			case Logic::Info:
			{
// 				foreach(QVariant var, m_data.toList() )
// 				{
// 					if(m_module == Logic::Users)
// 					{
// 						dataE.appendChild( qvariant_cast<ADUser *>( var )->toXml(doc) );
// 					}
// 				}
			}
			break;
			case Logic::Authenticate:
			{
				dDebug() << "authenticate";
				dataE.setAttribute("user", m_data.toList()[0].toString());
				dataE.setAttribute("passwd", m_data.toList()[1].toString());
			}
			break;
			case Logic::Add:
			{
				dataE.appendChild( qvariant_cast<ADUser *>( m_data )->toXml(doc) );
// 				listE.appendChild( qvariant_cast<ADUser *>( var )->toXml(doc) );
				
			}
			break;
		}
	}
	else
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
						
					}
				}
				dataE.appendChild(listE);
			}
			break;
			case Logic::Authenticate:
			{
// 				dDebug() << "server authenticate";
				dataE.appendChild( qvariant_cast<ADUser *>( m_data )->toXml(doc) );
				
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

