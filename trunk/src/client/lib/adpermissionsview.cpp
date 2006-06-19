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
#include<QVBoxLayout>
#include<QCheckBox>
#include <ddebug.h>
ADPermissionsView::ADPermissionsView(QWidget * parent)
	: QGroupBox(tr("Permissions"), parent)
{
	setSizePolicy ( QSizePolicy::Expanding ,QSizePolicy::Fixed);
	
	QVBoxLayout *layout = new QVBoxLayout(this);
	QStringList titles;
	titles << tr("Usuarios") << tr("Espacio") << tr("Audiovisules") << tr("Reserva");
	QCheckBox *checkB;
	for(int i = 0; i < titles.count(); i++)
	{
		checkB = new QCheckBox(titles[i], this);
		layout->addWidget(checkB);
		m_checks.insert(titles[i].toLower() , checkB);
	}
}


ADPermissionsView::~ADPermissionsView()
{
}

QMap<Logic::TypeModule, bool> ADPermissionsView::permissions() 
{
	dDebug() << "QMap<Logic::TypeModule, bool> ADPermissionsView::permissions() ";
	QMap<Logic::TypeModule, bool> per;
	if( m_checks[tr("usuarios")]->checkState()  == Qt::Checked)
	{
		dDebug() << true;
		per.insert(Logic::users, true);
	}
	else
	{
		dDebug() << false;
		per.insert(Logic::users, false);
	}
	if( m_checks[tr("espacio")]->checkState()  == Qt::Checked)
	{
		
		dDebug() << true;
		per.insert(Logic::spaces, true);
	}
	else
	{
		dDebug() << false;
		per.insert(Logic::spaces, false);
	}
	if( m_checks[tr("reserva")]->checkState()  == Qt::Checked)
	{
		dDebug() << true;
		per.insert(Logic::reserves, true);
	}
	else
	{
		dDebug() << false;
		per.insert(Logic::reserves, false);
	}
	if( m_checks[tr("audiovisules")]->checkState()  == Qt::Checked)
	{
		dDebug() << true;
		per.insert(Logic::audiovisuals, true);
	}
	else
	{
		dDebug() << false;
		per.insert(Logic::audiovisuals, false);
	}
	
	QMap<Logic::TypeModule, bool>::iterator it = per.begin();
	while(it != per.end())
	{
		dDebug() << "permissions()" << it.value();
		++it;
	}
	
	
	return per;
}

