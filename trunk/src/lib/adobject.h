/***************************************************************************
 *   Copyright (C) 2006 by Jorge Cuadrado   *
 *   kuadrosx@gmail.com   *
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
#ifndef ADOBJECT_H
#define ADOBJECT_H

/**
 * @author Jorge Cuadrado <kuadrosx@gmail.com>
*/
// #include<QObject>
#include "global.h"
// #include "adinsertpackage.h"
// #include "adupdatepackage.h"

class ADObject /*: public QObject*/
{
	public:
		ADObject();
		virtual ~ADObject();
// 		virtual ADInsertPackage insertPackage() = 0;
// 		virtual ADUpdatePackage updatePackage() = 0;
		virtual void setValues(XMLResults values) = 0;
		virtual bool isValid()  const = 0;
		/**
		 * Cualquier objeto tiene que poderse convertir en un string xml
		 * @return 
		 */
		virtual QString toXml()const = 0;
		/**
		 * Cualquier objeto tiene que poder objeter sus datos de un xml
		 * @param xml 
		 */
		virtual void fromXml(const QString & xml ) = 0;
};

#endif
