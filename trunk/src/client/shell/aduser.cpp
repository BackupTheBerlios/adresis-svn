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
#include "aduser.h"
#include "ddebug.h"
ADUser::ADUser()
	: ADObject()
{
	m_valid = false;
}


ADUser::ADUser(const QString & name, const QString & code,const QString &login,const QString& passwd, QMap<Logic::TypeModule, bool>permissions )
	: ADObject(), m_name(name), m_code(code), m_login(login), m_passwd(passwd), m_permissions(permissions)
{
	m_valid = true;
}


ADUser::~ADUser()
{
}

ADInsertPackage ADUser::insertPackage() 
{
	QString strPermissions;
	QMap<Logic::TypeModule, bool>::const_iterator it = m_permissions.begin();
	while(it != m_permissions.end())
	{
		if(it.value())
		{
			strPermissions += '1';
		}else
		{
			strPermissions += '0';
		}
		++it;
	}
	
	ADInsertPackage insert("aduser", QStringList() << "nameuser" << "codeuser" << "loginuser"<< "passwduser" << "permissionsuser", QStringList() <<  m_name<< m_code<< m_login<< m_passwd<< strPermissions );
	
	dDebug() << insert.toString();
	return insert;
}

ADUpdatePackage ADUser::updatePackage() 
{
	QString strPermissions;
	QMap<Logic::TypeModule, bool>::const_iterator it = m_permissions.begin();
	while(it != m_permissions.end())
	{
		if(it.value())
		{
			strPermissions += '1';
		}else
		{
			strPermissions += '0';
		}
		++it;
	}
	ADUpdatePackage update("aduser", QStringList() << "nameuser" << "codeuser" << "loginuser"<< "passwduser" << "permissionsuser",QStringList() <<  m_name<< m_code<< m_login<< m_passwd<< strPermissions );
	return update;
}

void ADUser::setValues(XMLResults values)
{
	m_name = values["nameuser"];
	m_code = values["codeuser"];
	m_login = values["loginuser"];
	m_passwd = values["passwduser"];
	
	QString strPermissions;
	
	strPermissions = values["permissionsuser"];
	dDebug() << m_name <<m_code << m_login<<m_passwd<<strPermissions;
	for(int i = 0; i < strPermissions.length (); i++)
	{
		strPermissions[i];
		if(strPermissions[i] == '1')
		{
			
			m_permissions.insert(Logic::TypeModule(i), true);
		}
		else
		{
			
			dDebug() << "false";
			m_permissions.insert(Logic::TypeModule(i), false);
		}
	}
	m_valid = true;
}

bool ADUser::isValid() const
{
	return m_valid;
}

QMap<Logic::TypeModule, bool> ADUser::permissions() const
{
	return  m_permissions;
}


QString ADUser::name() const
{
	return m_name;
}

QString ADUser::code() const
{
	return m_passwd;
}

QString  ADUser::login() const
{
	return m_login;
}

QString  ADUser::passwd() const
{
	return m_passwd;
}
