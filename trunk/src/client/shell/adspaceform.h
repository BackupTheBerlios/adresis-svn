/**************************************************************************
*   Copyright (C) 2006 by Hector Cruz                                     *
*   hectorcaz@gmail.com                                                   *
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
#include "adaudiovisual.h"
#include <QComboBox>
#include <QSpinBox>
#include <QRegExpValidator>
#include <QMessageBox>

/**
		@author Hector Fabio Cruz Mosquera, 0329876
 */

		class ADSpaceForm : public ADFormBase
{
	Q_OBJECT
	public:
		ADSpaceForm(const QStringList listSpaces, QList<QVariant> ayudas, QWidget *parent = 0);
		ADSpaceForm(const ADSpace& space, QList<QVariant>  listAudiovisualL, QList<QVariant>  listAudiovisualE, QWidget *parent = 0);
		~ADSpaceForm();
		
	
	private:
		ADSpace *m_space;
		QMap<QString, QWidget*> m_inputs;
		QList< ADAudioVisual > m_listAudiovisualL; /// Lista de ayudas libres
		QList< ADAudioVisual > m_listAudiovisualE; /// Lista de ayudas del Espacio
		QComboBox *tiposC;
		QCheckBox *acC;
		QSpinBox *capacity;
		ADListSelect *listSelect;
		bool m_inserter;
		bool m_list;	/// Representacion de la lista que estoy tomando << true = Lista libres || false = Lista Espacios >> 
		QRegExpValidator *v;
		
		void fill();
		void setup();
		void checkListsToSave();
		QStringList takeListKeys(const QString &list);
		void requestListTypes();
		bool valite();
		void clearFields();


	public slots:
		void emitEvent();
		void listChangedSF(const QString &lista, int pos);
		void insertListAudiovisual();
		void insertListTypes(const QStringList results);
		void modifyContend( Logic::Module, const QList< QVariant >&);
	
};

#endif
