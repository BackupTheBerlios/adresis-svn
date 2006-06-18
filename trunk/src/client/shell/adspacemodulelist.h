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
#ifndef ADSPACEMODULELIST_H
#define ADSPACEMODULELIST_H

#include<QList>
#include"adcmodulelist.h"
#include "global.h"

/**
@author Hector Fabio Cruz Mosquera,0329876
*/
class ADSpaceModuleList : public ADCModuleList
{
	Q_OBJECT
	public:
		ADSpaceModuleList(QWidget *parent=0);
		~ADSpaceModuleList();
		void fill( const QList<XMLResults>&results);
	
	private slots:
		void requestAction(int action);
		
	signals:
		void requestSpaceForm();
		void requestDelete(Logic::TypeModule module, const QString & key);
};

#endif
