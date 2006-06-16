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
#include "adusermodulelist.h"
#include <ddebug.h>
#include <dconfig.h>
#include <doptionaldialog.h>

ADUserModuleList::ADUserModuleList(QWidget *parent): ADCModuleList("Users", QStringList() << "login"<< "name", parent )
{

}


ADUserModuleList::~ADUserModuleList()
{
}

void ADUserModuleList::fill( const QList<XMLResults>&results)
{
	m_pTree->clear ();
	
	QList<XMLResults>::const_iterator it= results.begin();
	
	while( it != results.end() )
	{
		
		QStringList list;
		list << (*it)["loginuser"] << (*it)["nameuser"];
		addItem( list );
		++it;
	}
}

void ADUserModuleList::requestAction(int action)
{
	dError() << action;
	switch(action)
	{
		case ADModuleButtonBar::Add:
		{
			emit requestUserForm();
			break;
		}
		case ADModuleButtonBar::Del:
		{
			DCONFIG->beginGroup("Users");
			bool noAsk = qvariant_cast<bool>(DCONFIG->value("RemoveWithoutAskUser", false));
			if ( ! noAsk )
			{
				DOptionalDialog dialog(tr("usted reamente quire borrar este usuario?"),tr("borrar?"), this);
				if( dialog.exec() == QDialog::Rejected )
				{
					return;
				}
				
				DCONFIG->setValue("RemoveWithoutAskUser", dialog.shownAgain());
				DCONFIG->sync();
				emit requestDelete(Logic::users, m_pTree->currentItem()->text( 0 ));
			}
			
			break;
		}
	}
}


