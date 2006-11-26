/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado krawek@gmial.com                 *
 *                         Jorge Cuadrado kuadrosxx@gmail.com              *
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

#include "adconnector.h"

#include <QDataStream>

#include <ddebug.h>

#include <QDomDocument>


#include "global.h"

#include "adeventfactory.h"

ADConnector::ADConnector(QObject * parent) : ADConnectorBase(parent)
{
}


ADConnector::~ADConnector()
{
}

void ADConnector::readFromServer()
{

	while(canReadLine())
	{
		m_readed += readLine();
		
		if ( m_readed.endsWith("%%\n") )
		{
			break;
		}
	}
	
	m_readed.remove(m_readed.lastIndexOf("%%"), 2);
	
	
	
	// 	QXmlInputSource xmlsource;
// 	xmlsource.setData(m_readed);
// 	dDebug() << "READED: " << m_readed;
// 	bool parse =  m_reader.parse(&xmlsource);
// 	SHOW_VAR(parse);
	
	if ( !m_readed.isEmpty() )
	{
	
		QDomDocument doc;
		if ( doc.setContent(m_readed) )
		{
			QString root = doc.documentElement().tagName();
			
			if(root =="Event")
			{
				ADEventFactory factory;
				ADEvent *request = factory.build( m_readed );
				emit sendEvent( request );
			}
			else if ( root == "Error" )
			{
// 				XMLResults result = m_parser->results()[0];
// 				emit message(Msg::Error, "Error "+result["id"]+": "+result["message"] );
			}
			else if( root == "Success")
			{
	// 			emit readedModuleForms( m_parser->moduleForms() );
// 				emit message(Msg::Info, m_parser->results()[0]["message"]);
			}
			m_readed = "";
		}
	}
	else
	{
		dDebug() << "Error parsing: " << m_readed;
	}
	if( canReadLine() ) emit readyRead(); // HACK: Si los mensajes son enviados muy rapido, el socket no lo detecta
}

void ADConnector::login(const QString &user, const QString &passwd)
{
// 	QString toSend = ADConnectPackage(user, passwd).toString();
// 	toSend.remove('\n');
// 	sendToServer( toSend );
}

void ADConnector::handleError(QAbstractSocket::SocketError error)
{
	dError() << "ADConnector::handleError(QAbstractSocket::SocketError error)  Error: " << error;
	switch(error)
	{
		case QAbstractSocket::RemoteHostClosedError:
		{
			emit message(Msg::Error, tr("The conection was lost.") );
		}
		break;
		case QAbstractSocket::ConnectionRefusedError:
		{
			emit message(Msg::Error, tr("The conection was refused by the server.") );
		}
		break;
		case QAbstractSocket::HostNotFoundError:
		{
			emit message(Msg::Error, tr("Host not found.") );
		}
		break;
		default:
		{
			dDebug() << "NO SABEMOS CUAL ES EL ERROR";
			break;
		}
	}
}

