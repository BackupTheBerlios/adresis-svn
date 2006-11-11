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

#include "adreservemodulelist.h"
#include <ddebug.h>
#include <dconfig.h>
#include <doptionaldialog.h>


ADReserveModuleList::ADReserveModuleList(QWidget *parent): ADCModuleList(Logic::Reserves, parent )
{
}

ADReserveModuleList::~ADReserveModuleList()
{
}


void ADReserveModuleList::fill( const QList<XMLResults>&results)
{
	D_FUNCINFO;
	QList<XMLResults>::const_iterator it= results.begin();
	
	while( it != results.end() )
	{
		QStringList list;
		list << (*it)["name"] << (*it)["id"] << (*it)["type"]; // Estos nombres los coloque asi pero en el momento de hacer el paquete select tengo que renombrar los campos, ademas de que tengo que hacer una subconsulta si quiero obtener el nombre de el espacio o de la ayuda (Como lo estoy manejando en este momento es ambas en este misma lista)
		addItem( list );
		++it;
	}
}

void ADReserveModuleList::clean()
{
	m_pTree->clear ();
}

void ADReserveModuleList::requestAction(int action)
{
	dError() << action;
	switch(action)
	{
		case ADModuleButtonBar::Add:
		{
			
			emit requestReserveForm();
			dDebug() << "EMITI LA CREACION DE LA FORMA";
			break;
		}

		case ADModuleButtonBar::Del:
		{
			DCONFIG->beginGroup("Reserves");

			bool noAsk = qvariant_cast<bool>(DCONFIG->value("RemoveWithoutAskReserves", false));

			if ( ! noAsk )
			{
				if(m_pTree->currentItem())
				{
					DOptionalDialog dialog(tr("usted realmente quiere borrar esta reserva?"),tr("borrar?"), this);
					if( dialog.exec() == QDialog::Rejected )
					{
						return;
					}
	
					DCONFIG->setValue("RemoveWithoutAskAudiovisuals", dialog.shownAgain());
					DCONFIG->sync();
				
// 					Voy a omtir esta linea por ahora dado que cuando se va a eliminar una reserva se debe de consultar por que se cancela.
// 					emit requestDelete(Logic::audiovisuals, m_pTree->currentItem()->text( 1 ));
				}
			}
			break;
		}

		case  ADModuleButtonBar::Modify:
		{
// 			if(m_pTree->currentItem())
// 			{
// 				
// 				dDebug() << "emit requestUpdate(Logic::audiovisuals, m_pTree->currentItem()->text( 1));";
// 				emit requestUpdate(Logic::reserves, m_pTree->currentItem()->text(1));
// 			}

			break;
		}
		
	}
}
