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
#include <QDomDocument>
#include <QString>
#include <QMap>
#include <QStringList>
#include <ddebug.h>
#include <QXmlAttributes>

/**
	@author Jorge Cuadrado <kuadrosx@kuadrosx>
*/
class ADPermission : public ADObject
{
	public:
		ADPermission();
		~ADPermission();
		
		void setValues(const XMLResults values);
		
		QDomElement toXml(QDomDocument &doc) const;
		void setValues(QMap<QString, QStringList> values);
		void setValues(const QXmlAttributes& atts);
		bool isValid()  const;
		
		void setValue(const QString & action, bool enable );
		
	public:
		bool value(Logic::Module module, Logic::Action action);
		static QStringList actionList();
		
	private:
		QMap<QString, QString> m_permisos;
		bool m_valid;
		bool m_value;
		
		
};

#endif
