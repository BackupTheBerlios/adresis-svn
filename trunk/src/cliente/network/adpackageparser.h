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

#ifndef ADPACKAGEPARSER_H
#define ADPACKAGEPARSER_H

#include <qxml.h>
#include <QList>
#include <QPair>

#include "global.h"

/**
 * @author Cuadros Cuadrado <krawek@gmail.com>
*/
class ADPackageParser : public QXmlDefaultHandler
{
	public:
		ADPackageParser();
		~ADPackageParser();
		
		bool startElement(const QString& , const QString& , const QString& qname, const QXmlAttributes& atts);
		
		bool endElement( const QString& ns, const QString& localname, const QString& qname);
		
		bool characters ( const QString & ch );
		
		bool error ( const QXmlParseException & exception );
		bool fatalError ( const QXmlParseException & exception );
		
		QString root() const;
		
		QList<XMLResults> results() const;
		
	private:
		void reset();
		
	private:
		QString m_root, m_qname;
		
		bool m_isParsing;
		
		bool m_readChar;
		
		QList<XMLResults> m_valuesList;
// 		QList<QPair<QString, QString> > m_resources; // ###: Es posible ponerlo en m_valuesList ?
};

#endif
