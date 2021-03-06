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

#include "adconnectpackage.h"
#include "adpackageparser.h"

#include "global.h"

ADConnector::ADConnector(QObject * parent) : ADConnectorBase(parent)
{
	m_parser = new ADPackageParser;
	m_reader.setContentHandler(m_parser);
	m_reader.setErrorHandler(m_parser);
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
		
	if ( m_readed.isEmpty() )
	{
		return;
	}
	
	m_readed.remove(m_readed.lastIndexOf("%%"), 2);
	
	QXmlInputSource xmlsource;
	xmlsource.setData(m_readed);
	
	dDebug() << "READED: " << m_readed;
	bool parse =  m_reader.parse(&xmlsource);
	SHOW_VAR(parse);
	if ( parse )
	{
		QString root = m_parser->root();
		SHOW_VAR(root);
		if ( root == "Results" )
		{
			QList<XMLResults> results = m_parser->results();

			Logic::TypeQuery type = m_querys.dequeue();
			if(!results.empty())
			{
	#if 1
				foreach(XMLResults r, results)
				{
					XMLResults::const_iterator it = r.begin();
					while(it != r.end())
					{
						dDebug() << it.key() << " = " << it.value();
						++it;
					}
				}
	#endif
				SHOW_VAR(type);
				SHOW_VAR(m_querys.size());
				switch(type)
				{
					case Logic::userAuthenticated:
					{
						dDebug() << "emit userAutenticated(results[0]);";
						emit userAutenticated(results[0]);
						break;
					}
					case Logic::fillUserModule:
					{
						dDebug() << "emit fillModule(Logic::users, results );";
						emit fillModule(Logic::users, results );
						break;
					}
					case Logic::fillSpaceModule:
					{
						dDebug() << "emit fillModule(Logic::spaces, results );";
						emit fillModule(Logic::spaces, results );
						break;
					}
					case Logic::fillAudiovisualModule:
					{
						dDebug() << "fillModule(Logic::audiovisuals, results );";
						emit fillModule(Logic::audiovisuals, results );
						break;
					}
					case Logic::fillReserveModule :
					{
						dDebug() << "fillModule(Logic::reserves, results );";
						emit fillModule(Logic::reserves, results );
						break;
					}
					case Logic::queryUser:
					{
						dDebug() << "requestShowUser(results )";
						emit requestShowUser( results[0] );
						break;
					}
					case Logic::queryAudiovisual:
					{
						dDebug() << "requestShowAudiovisual(results )";
						emit requestShowAudiovisual( results[0] );
						break;
					}
					case Logic::querySpace:
					{
						dDebug() << "requestShowSpace(results )";
						emit requestShowSpace( results[0] );
						break;
					}
					case Logic::queryListAudiovisual:
					{
						dDebug() << "requestShowListAudiovisual(results )";
						emit requestShowListAudiovisual( results );
						break;
					}
					case Logic::querytypes:
					{
						dDebug() << "requestListTypes( results )";
						emit requestListTypes( results );
						break;
					}
					case Logic::querySchedule:
					{
						dDebug() << "requestSchadule( results )"; 
						emit requestSchedule( results );
						break;
					}
					default:
					{
						emit message(Msg::Error, "Error: operacion no permitida" );
						break;
					}
				}
			}
		}
		else if ( root == "Error" )
		{
			XMLResults result = m_parser->results()[0];
			emit message(Msg::Error, "Error "+result["id"]+": "+result["message"] );
		}
		else if( root == "Success")
		{
// 			emit readedModuleForms( m_parser->moduleForms() );
			emit message(Msg::Info, m_parser->results()[0]["message"]);
		}
		m_readed = "";
	}
	else
	{
		dDebug() << "Error parsing: " << m_readed;
	}
	
	if( canReadLine() ) emit readyRead(); // HACK: Si los mensajes son enviados muy rapido, el socket no lo detecta
}

void ADConnector::login(const QString &user, const QString &passwd)
{
	QString toSend = ADConnectPackage(user, passwd).toString();
	toSend.remove('\n');
	sendToServer( toSend );
}

void ADConnector::handleError(QAbstractSocket::SocketError error)
{
	dError() << "Error: " << error;
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
			break;
		}
	}
}

void ADConnector::sendQuery(Logic::TypeQuery type, const ADSelectPackage& select)
{
	D_FUNCINFO;
	SHOW_VAR(type);
	m_querys.enqueue ( type );
	sendPackage(select);
}

void ADConnector::sendPackage(const ADSqlPackageBase & package)
{
	QString toSend = package.toString();
	sendToServer( toSend);
}

