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
	
	actions << tr("Manage user")
			<< tr("Consult usuario")
			<< tr("Manage space")
			<< tr("Consult space")
			<< tr("Manage audiovisual")
			<< tr("Consult audiovisual")
			<< tr("Manage semester")
			<< tr("Manage temporal")
			<< tr("Consult reserves")
			<< tr("Manage report")
			<< tr("Consult report");
	int count = 0;
	QStringList act = ADPermission::actionList();
	foreach(QString action, actions)
	{
		QCheckBox *check = new QCheckBox(action,this);
		layout->addWidget(check, count, 1);
		m_checks.insert(act[count], check);
		count++;
	}
}


ADPermissionsView::~ADPermissionsView()
{
}

ADPermission ADPermissionsView::permissions()
{
	QHash<QString, QCheckBox* >::iterator it = m_checks.begin();
	ADPermission per;
	while(it != m_checks.end())
	{
		bool ok = true;
		if(it.value()->checkState () == Qt::Unchecked)
		{
			ok = false;
		}
		per.setValue(it.key(), ok);
		++it;
	}
	
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

