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
#ifndef ADSPACEFORM_H
#define ADSPACEFORM_H

#include "adformbase.h"
#include <QMap>
#include "global.h"
#include <QCheckBox>
#include "adlistselect.h"
#include "adspace.h"

/**
@author Hector Fabio Cruz Mosquera,0329876
*/

class ADSpaceForm : public ADFormBase
{
	Q_OBJECT
	public:
		ADSpaceForm(QWidget *parent = 0);
		ADSpaceForm(const ADSpace& space, QWidget *parent = 0);
		~ADSpaceForm();
		void setup();
	
	private:
		QMap<QString, QWidget*> m_inputs;
		QMap<QString, QString> m_listAudiovisualL;
		QMap<QString, QString> m_listAudiovisualE;
		QCheckBox *acC;
		ADListSelect *listSelect;
		bool m_inserter;
		bool m_list;
		
	signals:
		void requestInsertSpace(const QString& codeSpace, const QString& typeSpace,const bool coolAirSpace,const QString& capacitySpace, const QString& nameSpace);
		
		void requestUpdateSpace(const QString& codeSpace, const QString& typeSpace,const bool coolAirSpace,const QString& capacitySpace, const QString& nameSpace);

	public slots:
		void emitInsertSpace();
		void insertListAudiovisual(const QList<XMLResults>& results);
	
	
};

#endif
