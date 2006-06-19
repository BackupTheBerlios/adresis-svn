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
#ifndef ADAUDIOVISUALFORM_H
#define ADAUDIOVISUALFORM_H

#include "adformbase.h"
#include <QMap>
#include "global.h"
#include <QComboBox>

//cambios
#include "adaudiovisual.h"

/**
@author Hector Fabio Cruz Mosquera,0329876
*/
class ADAudiovisualForm : public ADFormBase
{
	Q_OBJECT
	public:
		ADAudiovisualForm(QWidget *parent = 0);
		ADAudiovisualForm(const ADAudioVisual & audiovisual, QWidget *parent = 0);
		~ADAudiovisualForm();
		void setup();
		
	private:
		QMap<QString, QWidget*> m_inputs;
		QComboBox *estadoC;
		bool m_inserter;
		
	signals:
		void requestInsertAudiovisual(const QString& typeav, const QString& marksEquipmentav,const QString& estateav,const QString& numberinventoryav, const QString& codeSpace);
		void requestUpdateAudiovisual(const QString& typeav, const QString& marksEquipmentav,const QString& estateav,const QString& numberinventoryav, const QString& codeSpace);
		
	public slots:
		void emitInsertAudiovisual();
		
};

#endif

