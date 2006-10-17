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
#ifndef ADUSER_H
#define ADUSER_H
#include <QMap>

#include "adobject.h"
#include <QMetaType>
#include <QDomElement>
#include "adpermission.h"

/**
*	@author Jorge Cuadrado <kuadrosx@gmail>
*/

class ADUser : public ADObject
{
	public:
		ADUser();
		ADUser(const QString & name, const QString & code,const QString &login,const QString& passwd, ADPermission permissions);
		ADUser(const ADUser & copy);
		~ADUser();
// 		ADInsertPackage insertPackage();
// 		ADUpdatePackage updatePackage();
		
		QString toXml() const;
		QDomElement toXml(QDomDocument &doc);
		
		void fromXml(const QString & xml );
		
		void setValues(XMLResults values);
		bool isValid() const;
		bool permission(Logic::Module, Logic::Action);
		ADPermission permissions() const;
		QString name() const ; 
		QString code() const;
		QString login() const;
		QString passwd() const;
		void assignPermissions(ADPermission permissions);
		
	private:
		QString m_name;
		QString m_code;
		QString m_login;
		QString m_passwd;
		ADPermission m_permissions;
		bool m_valid;
};

Q_DECLARE_METATYPE(ADUser *);

#endif
