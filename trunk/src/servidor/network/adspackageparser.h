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

#ifndef ADSPACKAGEPARSER_H
#define ADSPACKAGEPARSER_H

#include <qxml.h>
#include <QMap>

/**
 * Analizador de paquetes XML que el cliente envia al servidor, esta clase analiza utilizando <a href="http://www.saxproject.org">SAX2</a>.
 * @author David Cuadrado <krawek@gmail.com>
*/
class ADSPackageParser : public QXmlDefaultHandler
{
	public:
		ADSPackageParser();
		~ADSPackageParser();
		
		bool startElement(const QString& , const QString& , const QString& qname, const QXmlAttributes& atts);
		
		bool endElement( const QString& ns, const QString& localname, const QString& qname);
		
		bool characters ( const QString & ch );
		
		bool error ( const QXmlParseException & exception );
		bool fatalError ( const QXmlParseException & exception );
		
		/**
		 * Retorna el elemento raiz del documento XML.
		 * @return 
		 */
		QString root() const;
		
		/**
		 * Retorna una mapa, donde cada tag es la llave que contiene valores.
		 * @return 
		 */
		QMap<QString, QString> values() const;
		
		/**
		 * Esta funcion restaura el analizador.
		 */
		void reset();
		
	private:
		QString m_root, m_qname;
		
		QMap<QString, QString> m_values;
		
		bool m_isParsing;
		bool m_readCharacters;
};

#endif
