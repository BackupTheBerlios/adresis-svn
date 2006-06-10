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

#include "adspackageparser.h"

#include "adsglobal.h"

#include <ddebug.h>

ADSPackageParser::ADSPackageParser() : QXmlDefaultHandler(), m_isParsing(false), m_readCharacters(false)
{
}


ADSPackageParser::~ADSPackageParser()
{
}

void ADSPackageParser::reset()
{
	m_root = QString();
	m_qname = QString();
	m_values.clear();
}

bool ADSPackageParser::startElement( const QString& , const QString& , const QString& qname, const QXmlAttributes& atts)
{
	dDebug() << qname;
	if (!m_isParsing)
	{
		reset();
		m_root = qname;
		
		m_isParsing = true;
	}
	else if ( m_root == "Connection" )
	{
		if ( qname == "Login" )
		{
			m_values[qname] = atts.value("value");
		}
		else if ( qname == "Password")
		{
			m_values[qname] = atts.value("value");
			// TODO: parse encode
		}
	}
// 	else if ( m_root == "Chat" )
// 	{
// 		if ( qname == "Message" )
// 		{
// 			m_values[qname] = atts.value("value");
// 		}
// 	}
	else if ( m_root == "Insert" )
	{
		if ( qname == "field" )
		{
			QString field = atts.value("id");
			QString value = atts.value("value");
			
			m_values[qname] += field+"::"+value+ADS::FIELD_SEPARATOR; // FIXME!
		}
		else if ( qname == "table" )
		{
			QString table = atts.value("id");
			
			m_values[qname] = table;
		}
	}
	else if ( m_root == "Update" )
	{
		if ( qname == "field" )
		{
			QString field = atts.value("id");
			QString value = atts.value("value");
			
			m_values[qname] += field+"::"+value+ ADS::FIELD_SEPARATOR; // FIXME!
		}
		else if ( qname == "table" )
		{
			QString table = atts.value("id");
			
			m_values[qname] = table;
		}
		else if ( qname == "where" || qname == "condition" )
		{
			m_readCharacters = true;
		}
	}
	else if ( m_root == "Select" )
	{
		if ( qname == "Select" )
		{
			m_values["distinct"] = atts.value("distinct", "0");
		}
		else if ( qname == "field" )
		{
			QString field = atts.value("id");
			
			m_values[qname] += field+ADS::FIELD_SEPARATOR;
		}
		else if ( qname == "table" )
		{
			QString table = atts.value("id");
			
			m_values[qname] += table+ADS::FIELD_SEPARATOR;
		}
		else if ( qname == "where" || qname == "condition" )
		{
			m_readCharacters = true;
		}
	}
	else if ( m_root == "Delete" )
	{
		if ( qname == "table" )
		{
			QString table = atts.value("id");
			
			m_values[qname] = table;
		}
		else if ( qname == "where" || qname == "condition" )
		{
			m_readCharacters = true;
		}
	}
	else
	{
		return false;
	}
	
	m_qname = qname;
	return true;
}

bool ADSPackageParser::endElement(const QString&, const QString& , const QString& qname)
{
	if( qname == "Connection" )
	{
	}
	
	if ( qname == m_root )
	{
		m_isParsing = false;
	}
	
	return true;
}

bool ADSPackageParser::characters ( const QString & ch )
{
	if (m_readCharacters )
	{
		if ( m_root == "Insert" || m_root == "Update" || m_root == "Delete" || m_root == "Select" )
		{
			if ( m_qname == "where" || m_qname == "condition" )
			{
				m_values[m_qname] = ch;
			}
		}
		
		m_readCharacters = false;
	}
	
	return true;
}

bool ADSPackageParser::error ( const QXmlParseException & exception )
{
	dError() << exception.lineNumber() << "x" << exception.columnNumber() << ": " << exception.message();
	
	return true;
}


bool ADSPackageParser::fatalError ( const QXmlParseException & exception )
{
	dFatal() << exception.lineNumber() << "x" << exception.columnNumber() << ": " << exception.message();
	
	return true;
}

QString ADSPackageParser::root() const
{
	return m_root;
}

QMap<QString, QString> ADSPackageParser::values() const
{
	return m_values;
}
