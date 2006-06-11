/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
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

#include "sdatabase.h"

#include <QMap>
#include <QVariant>
#include <QString>

#include <ddebug.h>
#include "qsql_psql.h"

SDatabase *SDatabase::m_instance = 0;

SDatabase::SDatabase(QObject *parent) : QObject(parent), QSqlDatabase(new QPSQLDriver(this))
{
}


SDatabase::~SDatabase()
{
	if( m_instance ) delete m_instance;
}

SDatabase *SDatabase::instance()
{
	if ( m_instance == 0 )
	{
		m_instance = new SDatabase;
	}
	return m_instance;
}

SResultSet SDatabase::execRawQuery(const QString &sql)
{
	dDebug() << "Executing: " << sql << " on " << databaseName () ;
	
	SResultSet rs;
	QSqlQuery query = exec(sql);
	QSqlRecord record = query.record();
	
	QStringList fields;
	
	for(int i = 0; i < record.count(); i++)
	{
		fields << record.fieldName(i);
	}
	
	while(query.next())
	{
		QStringList values;
		
		for(int i = 0; i < query.record().count() ; i++)
		{
			values << query.value(i).toString();
		}
		
		rs.setRecord( fields, values);
	}
	
	QString error = lastError().text();
	if ( error != " ")
	{
		dFatal() << error;
	}
	
	return rs;
}

void SDatabase::setDatabaseName(const QString &dbname)
{
	QSqlDatabase::setDatabaseName(dbname);
}

void SDatabase::setHostName(const QString &host)
{
	QSqlDatabase::setHostName(host);
}

void SDatabase::setPassword(const QString &password)
{
	QSqlDatabase::setPassword(password);
}

void SDatabase::setPort(int p)
{
	QSqlDatabase::setPort(p);
}

void SDatabase::setUserName(const QString &login )
{
	QSqlDatabase::setUserName(login);
}


void SDatabase::setupConnection(const QString &dbname, const QString & login, const QString & password, const QString & host )
{
	dDebug() << "Setting up the database with: " << dbname << " " << login << " " << host;
	setDatabaseName(dbname);
	setHostName(host);
	setUserName(login);
	setPassword(password);
}

SResultSet SDatabase::execQuery(const ADQuery *query )
{
	return execRawQuery(query->toString());
}

