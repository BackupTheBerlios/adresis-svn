/***************************************************************************
 *   Copyright (C) 2006 by Jorge Cuadrado   *
 *   kuadrosx@zi0n   *
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
#ifndef ADPERMISSIONSVIEW_H
#define ADPERMISSIONSVIEW_H

#include <QGroupBox>
#include <QMap>
#include<QCheckBox>
#include "global.h"
/**
	@author Jorge Cuadrado <kuadrosx@kuadrosx>
*/
class ADPermissionsView : public QGroupBox
{
	public:
		ADPermissionsView(QWidget *parent= 0);
		~ADPermissionsView();
		QMap<Logic::TypeUser, bool> permissions();
		void setPermissions(const QMap<Logic::TypeUser, bool> & permissions );
		
	private:
		QMap<QString,QCheckBox* > m_checks;
		
};

#endif
