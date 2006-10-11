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
#include "adpermission.h"

ADPermission::ADPermission(Logic::Module module, Logic::Action action, bool value): ADObject(), m_module(module), m_action(action), m_value(value)
{
	
}


ADPermission::~ADPermission()
{
}


void ADPermission::setValues(XMLResults values)
{
	
}

bool ADPermission::isValid()  const
{
	
}

QString ADPermission::toXml() const
{
	
}

void ADPermission::fromXml(const QString &xml)
{
	
}

int ADPermission::module() const
{
	return m_module;
}

int ADPermission::action() const
{
	return m_action;
}

bool ADPermission::value() const 
{
	return m_value;
}
