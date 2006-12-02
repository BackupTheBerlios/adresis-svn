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


ADUser::ADUser(const QString & name, const QString & code,const QString &login,const QString& passwd, ADPermission permissions, int rol )
	: ADObject(), m_name(name), m_code(code), m_login(login), m_passwd(passwd), m_permissions(permissions), m_rol(rol)
{
	m_valid = true;
}

ADUser::ADUser(const ADUser & copy): ADObject(), m_name(copy.m_name), m_code(copy.m_code), m_login(copy.m_login), m_passwd(copy.m_passwd), m_permissions(copy.m_permissions), m_rol(copy.m_rol)
{
	m_valid = true;
}

ADUser::~ADUser()
{
}


QDomElement ADUser::toXml(QDomDocument &doc) const
{
	QDomElement root = doc.createElement("user");
	
	root.setAttribute( "name", m_name );
	root.setAttribute( "code", m_code );
	root.setAttribute( "login", m_login );
	root.setAttribute( "passwd", m_passwd );
	root.setAttribute( "rol", m_rol );
	root.appendChild(m_permissions.toXml(doc));
	
	return root;
}


bool ADUser::permission( Logic::Module mod, Logic::Action act)
{
	D_FUNCINFO;
	bool valor = m_permissions.value( mod, act);
	return valor;
	
}


void ADUser::assignPermissions(ADPermission permissions)
{
	m_permissions = permissions;
}


void ADUser::setValues(XMLResults values)
{
	m_name = values["nameuser"];
	m_code = values["codeuser"];
	m_login = values["loginuser"];
	m_passwd = values["passwduser"];

	m_valid = true;
}


bool ADUser::isValid() const
{
	return m_valid;
}

ADPermission ADUser::permissions() const
{

	return  m_permissions;
}


QString ADUser::name() const
{
	return m_name;
}


QString ADUser::code() const
{
	return m_code;
}


QString  ADUser::login() const
{
	return m_login;
}


QString  ADUser::passwd() const
{
	return m_passwd;
}

/**
 * Retorna un diccionario que asocia el nombre del rol con su correspondiente id
 * 
*/

QMap<QString, int> ADUser::rols()
{
	QMap<QString, int> dict;
	dict.insert(QObject::tr("Admin"), 0);
	dict.insert(QObject::tr("Profesor"), 1);
	dict.insert(QObject::tr("Student"), 2);
	return dict;
}

void ADUser::setRol( int id)
{
	m_rol = id;
}

int ADUser::rol() const
{
	return m_rol;
}
