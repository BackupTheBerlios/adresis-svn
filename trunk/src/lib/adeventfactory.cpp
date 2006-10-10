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

#include <QXmlSimpleReader>
#include <QXmlInputSource>


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
	else if(qname == "Data")
	{
		
	}
	else if(qname == "Condition")
	{
		m_data = atts.value("value");
	}
	m_qname = qname;
	
	return true;
}

bool ADEventFactory::endElement( const QString& ns, const QString& localname, const QString& qname)
{
	if ( qname == "Data" )
	{
		m_event->setData(m_data);
// 		m_isParsing = false;
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
