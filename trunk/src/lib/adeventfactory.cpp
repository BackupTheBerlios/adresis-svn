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
 
#include "adeventfactory.h"
#include <ddebug.h>
#include "adevent.h"
#include "aduser.h"
#include "adspace.h"
#include "adaudiovisual.h"
#include "adreserve.h"
#include "adcancellation.h"


#include <QXmlSimpleReader>
#include <QXmlInputSource>
#include "adpermission.h"


ADEventFactory::ADEventFactory(): QXmlDefaultHandler()
{
}


ADEventFactory::~ADEventFactory()
{

}

bool ADEventFactory::startElement(const QString& , const QString& , const QString& qname, const QXmlAttributes& atts)
{
	if (!m_isParsing)
	{
		m_isParsing = true;
	}
	
	if ( qname == "Event" )
	{
		m_event = new ADEvent;
	}
	else if(qname == "Source")
	{
		m_event->setSource(ADEvent::Source(atts.value("value").toInt()));
	}
	else if(qname == "Module")
	{
		m_event->setModule(Logic::Module(atts.value("value").toInt()));
	}
	else if(qname == "Action")
	{
		m_event->setAction(Logic::Action(atts.value("value").toInt()));
	}
	else if(qname == "Data")
	{
		if(m_event->action() == Logic::Authenticate)
		{
			QList<QVariant> data;
			data << atts.value("user") << atts.value("passwd");
			m_data = data;
		}
	}
	else if(qname == "List")
	{
		m_list = QList<QVariant>();
	}
	else if(qname == "Condition")
	{
		m_data = atts.value("value");
	}
	else if(qname == "Key")
	{
		m_data = atts.value("value");
	}
	else if(qname == "user")
	{
		ADPermission permission;
		ADUser *user = new ADUser(atts.value("name"), atts.value("code"), atts.value("login"), atts.value("passwd"), permission, atts.value("rol").toInt() );
		m_data = QVariant::fromValue(user);
	}
	else if(qname == "space")
	{
		ADSpace * space = new ADSpace();
		space->setValues(atts);
		m_data = QVariant::fromValue(space);
	}
	else if(qname == "audiovisual")
	{
		ADAudioVisual *audiovisual = new ADAudioVisual(atts.value("typeav"), atts.value("marksequipmentav"), atts.value("estateav"), atts.value( "numberInventoryav"), atts.value( "codespace"));
		m_data = QVariant::fromValue(audiovisual);
	}
	else if(qname == "reserve")
	{
		ADReserve *reserve = new ADReserve(
				atts.value( "idreserve" ),
				atts.value( "typereserve" ),
				atts.value( "iduserreserve"),
				atts.value( "iduserresponsable"),
				atts.value( "idaudiovisual"),
				atts.value( "idspace"),
				atts.value( "day"),
				QDateTime( QDate::fromString( atts.value("begindate"),"dd/MM/yyyy"), QTime::fromString( atts.value("beginhour"),"hh:mm")), QDateTime( QDate::fromString( atts.value("enddate"),"dd/MM/yyyy"), QTime::fromString( atts.value("endhour"),"hh:mm" ) ),  bool(atts.value( "isactive").toInt()), atts.value( "destinationreserve"));
		m_data = QVariant::fromValue(reserve);
		
	}
	else if(qname == "cancellation")
	{
		ADCancellation *cancel = new ADCancellation(
				atts.value( "idcancellation"),
				atts.value( "idusercancellation"),
				QDateTime( QDate::fromString( atts.value("datecancellation"),"dd/MM/yyyy"), QTime::fromString( atts.value("hourcancellation"),"hh:mm")), 
				atts.value( "razoncancellation"));
		m_data = QVariant::fromValue(cancel);
	}
	else if(qname == "types")
	{
		QString d = atts.value("type");
		m_data = QVariant(d);
	}
	else if(qname == "dates")
	{
		QString dI = atts.value("date");
		m_data = QVariant(dI);
	}
	else if(qname == "permissions")
	{
		ADPermission permissions;
		permissions.setValues(atts);
		qvariant_cast<ADUser *>(m_data)->assignPermissions(permissions);
		
		for(int i=0;i<5;i++)
		{
			Logic::Module module = Logic::Module(i);
		}
	}
	m_qname = qname;
	
	return true;
}

bool ADEventFactory::endElement( const QString& ns, const QString& localname, const QString& qname)
{
	if ( qname == "Data" )
	{
		m_event->setData(m_data);
	}
	else if(qname == "user" || qname == "space"||qname == "audiovisual" || qname == "reserve" || qname == "cancellation")
	{
		if(m_event->action() == Logic::Find)
		{
			m_list << m_data;
		}
	}
	else if(qname =="types" && m_event->action() == Logic::GetTypes )
	{
		m_list << m_data;
	}
	
	else if(qname =="dates" && m_event->action() == Logic::Dates )
	{
		m_list << m_data;
	}
	
	else if(qname == "List")
	{
		m_data = m_list;
	}
	return true;
}

bool ADEventFactory::characters ( const QString & ch )
{
	return true;
}

bool ADEventFactory::error ( const QXmlParseException & exception )
{
	dError() << exception.lineNumber() << "x" << exception.columnNumber() << ": " << exception.message();
	
	return true;
}

bool ADEventFactory::fatalError ( const QXmlParseException & exception )
{
	dFatal() << exception.lineNumber() << "x" << exception.columnNumber() << ": " << exception.message();
	
	return true;
}

ADEvent *ADEventFactory::build(const QString &document)
{
	dDebug() << "Builds from: " << document;
	
	QXmlSimpleReader m_reader;
	m_reader.setContentHandler(this);
	m_reader.setErrorHandler(this);
	
	QXmlInputSource xmlsource;
	xmlsource.setData(document);
	
	
	if( m_reader.parse(&xmlsource) )
	{
		return m_event;
	}
	
	return 0;
}
