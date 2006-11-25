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
#include "adpermissionsview.h"
#include <QVBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include <QGridLayout>
#include <ddebug.h>

ADPermissionsView::ADPermissionsView(QWidget * parent)
	: QGroupBox(tr("Permissions"), parent)
{
	setSizePolicy ( QSizePolicy::Expanding ,QSizePolicy::Fixed);
	QGridLayout *layout = new QGridLayout(this);
	QStringList actions;
	
	actions << tr("Gestionar usuario")
			<< tr("Consultar usuario")
			<< tr("Gestionar espacio")
			<< tr("Consultar espacio")
			<< tr("Gestionar audiovisual")
			<< tr("Consultar audiovisual")
			<< tr("Gestionar semestral")
			<< tr("Gestionar temporal")
			<< tr("Consultar reservas")
			<< tr("Gestionar reportes")
			<< tr("Consultar reportes");
	int count = 0;
	foreach(QString action, actions)
	{
		layout->addWidget(new QLabel(action, this), count,0);
		QCheckBox *check = new QCheckBox(this);
		layout->addWidget(check, count, 1);
		m_checks.insert(action, check);
		count++;
	}
}


ADPermissionsView::~ADPermissionsView()
{
}

ADPermission ADPermissionsView::permissions()
{
// 	QMap<Logic::Module, bool> per;
// 	for(int i =0; i < 5; i++)
// 	{
// 		per.insert(Logic::Module(i), true);
// 	}
// 	if( m_checks[tr("administrador")]->checkState()  == Qt::Checked)
// 	{
// 		dDebug() << true;
// 		per.insert(Logic::, true);
// 	}
// 	else
// 	{
// 		dDebug() << false;
// 		per.insert(Logic::administrador, false);
// 	}
	
/*	QMap<Logic::TypeUser, bool>::iterator it = per.begin();
	while(it != per.end())
	{
		dDebug() << "permissions()" << it.value();
		++it;
	}*/
// 	return per;
	return m_permission;
}

void ADPermissionsView::setPermissions(const ADPermission& permissions)
{
// 	m_permission = permissions;
	for(int i = 0; i < 6; i++)
	{
		
// 	if(value(Logic::Module(i), Logic::Action());
	}
}

