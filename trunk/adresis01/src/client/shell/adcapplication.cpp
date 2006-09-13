/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@gmail.com                                                      *
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

#include "adcapplication.h"

#include "adcfirstrundialog.h"

#include <dconfig.h>

ADCApplication::ADCApplication(int & argc, char ** argv) : DApplication(argc, argv)
{
	
}


ADCApplication::~ADCApplication()
{
	DCONFIG->sync();
}

bool ADCApplication::firstRun()
{
	ADCFirstRunDialog configurator;
	
	if ( configurator.exec() != QDialog::Rejected )
	{
		DCONFIG->beginGroup("General");
		DCONFIG->setValue("Home", configurator.home());
		DCONFIG->setValue("Repository", configurator.repository());
		DCONFIG->sync();
		return true;
	}
	
	return false;
}
