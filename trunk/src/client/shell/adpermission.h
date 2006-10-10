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
#ifndef ADPERMISSION_H
#define ADPERMISSION_H

#include <adobject.h>

/**
	@author Jorge Cuadrado <kuadrosx@kuadrosx>
*/
class ADPermission : public ADObject
{
	public:
		ADPermission( Logic::Module, Logic::Action, bool value = true);
		~ADPermission();
		ADInsertPackage insertPackage(){ };
		ADUpdatePackage updatePackage(){};
		void setValues(XMLResults values);
		bool isValid()  const;
		QString toXml() const;
		void fromXml(const QString &xml);
		
		
	public:
		int module() const;
		int action() const;
		bool value() const;
		
		
	private:
		Logic::Module m_module;
		Logic::Action m_action;
		bool m_value;
};

#endif
