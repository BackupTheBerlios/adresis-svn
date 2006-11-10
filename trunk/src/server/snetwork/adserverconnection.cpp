/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@gmail.com                                                      *
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

#include "adserverconnection.h"

#include <QtNetwork>

#include <ddebug.h>

#include "serrorpackage.h"
#include "schatpackage.h"

#include "adsglobal.h"

#include "adeventfactory.h"

ADServerConnection::ADServerConnection(int socketDescriptor, QObject *parent) : QThread(parent)
{
	m_client = new ADServerClient(this);
	m_client->setSocketDescriptor(socketDescriptor);
	
	m_parser = new ADSPackageParser;
	
	m_reader.setContentHandler(m_parser);
	m_reader.setErrorHandler(m_parser);
	
}

ADServerConnection::~ADServerConnection()
{
	delete m_client;
}

void ADServerConnection::run()
{
	while(m_client->state() != QAbstractSocket::UnconnectedState)
	{
		QString readed;
		while(m_client->canReadLine())
		{
			readed += m_client->readLine();
			if ( readed.endsWith("%%\n") )
			{
				break;
			}
		}
		if ( !readed.isEmpty() )
		{
			readed.remove(readed.lastIndexOf("%%"), 2);
			dDebug() << "Readed:" <<  readed;
			QDomDocument doc;
			
			if ( doc.setContent(readed) )
			{
				QString root = doc.documentElement().tagName();
				
				dDebug() << root;
				if(root == "Event")
				{
// 					dDebug() << "RECIBI EL EVENTO EN ADSERVERCONNECTION void ADServerConnection::run()";
					ADEventFactory factory;
					ADEvent *request = factory.build( readed );
					
					if ( request )
					{
						emit requestEvent(this, request );
// 						delete request;
					}
				}
			}
			/*
			QXmlInputSource xmlsource;
			xmlsource.setData(readed+'\n');
			
			dDebug() << "READED: " << readed;
			
			if ( m_reader.parse(&xmlsource) )
			{
				QString root = m_parser->root();
				QMap<QString, QString> values = m_parser->values();
				
				if ( root == "Connection")
				{
					
				}
// 				else if ( root == "Chat" )
// 				{
// 					emit requestSendToAll(  SChatPackage(m_login,values["Message"]).toString().remove('\n') );
// 				}
				else if ( root == "Insert" )
				{
					QStringList fields_and_values = values["field"].split(ADS::FIELD_SEPARATOR);
					
					QStringList fieldsList, valuesList;
					
					foreach(QString field_and_value, fields_and_values )
					{
						QStringList tmpFields = field_and_value.split("::");
						
						if ( tmpFields.count() != 2 ) continue;
						
						fieldsList << tmpFields[0];
						valuesList << tmpFields[1];
					}
					
					QString table = values["table"];
					
					ADInsert *insert = new ADInsert(table, fieldsList, valuesList);
					
					if ( !values["where"].isEmpty() )
					{
						insert->setWhere( values["where"] );
					}
					
					if ( !values["condition"].isEmpty() )
					{
						insert->setCondition( values["condition"] );
					}
					
					emit requestOperation( this, insert );
				}
				else if ( root == "Update" )
				{
					QStringList fields_and_values = values["field"].split(ADS::FIELD_SEPARATOR);
					
					QStringList fieldsList, valuesList;
					
					foreach(QString field_and_value, fields_and_values )
					{
						QStringList tmpFields = field_and_value.split("::");
						
						if ( tmpFields.count() != 2 ) continue;
						
						fieldsList << tmpFields[0];
						valuesList << tmpFields[1];
					}
					
					QString table = values["table"];
					
					ADUpdate *update = new ADUpdate(table, fieldsList, valuesList);
					
					if ( !values["where"].isEmpty() )
					{
						update->setWhere( values["where"] );
					}
					
					if ( !values["condition"].isEmpty() )
					{
						update->setCondition( values["condition"] );
					}
					
					emit requestOperation( this, update );
				}
				else if ( root == "Delete" )
				{
					QString table = values["table"];
					
					ADDelete *del = new ADDelete(table);
					
					if ( !values["where"].isEmpty() )
					{
						del->setWhere( values["where"] );
					}
					
					if ( !values["condition"].isEmpty() )
					{
						del->setCondition( values["condition"] );
					}
					
					emit requestOperation( this, del );
				}
				else if ( root == "Select" )
				{
					QString fields = values["field"];
					QString tables = values["table"];
					
					//remove the last (,)
					fields = fields.remove(fields.count()-1,1);
					tables = tables.remove(tables.count()-1,1);
					
					ADSelect *select = new ADSelect(QStringList() << fields, QStringList() << tables, values["distinct"].toInt() );
					
					if ( values.contains( "where" ) )
						select->setWhere( values["where"] );
					
					if ( values.contains("condition") )
						select->setCondition( values["condition"] );
					
					emit requestOperation( this, select);
				}
			}
			else
			{
				m_parser->reset();
				sendToClient( SErrorPackage(0, tr("Bad package ")+readed) );
				if ( !readed.isEmpty() )
				{
					close();
				}
			}*/
		}
	}
	
	// Finish connection
	emit requestRemoveConnection( this );
}

void ADServerConnection::sendToClient(const QString &msg)
{
	dDebug() << "SENDING: " << msg;
	m_client->reset();
	
	QTextStream out(m_client);
	
	QString toSend(msg);
	toSend.remove('\n');
	
	out << toSend+"%%" << endl;
	m_client->flush();
}

void ADServerConnection::sendToClient(const QDomDocument &doc)
{
	sendToClient( doc.toString(0));
}

void ADServerConnection::close()
{
	D_FUNCINFO;
// 	m_client->waitForDisconnected();
	m_client->disconnectFromHost();
	m_client->close();
}

void ADServerConnection::setLogin(const QString &login)
{
	m_login = login;
}

bool ADServerConnection::isLogged() const
{
	return !m_login.isEmpty();
}

