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
#ifndef ADMAINWINDOW_H
#define ADMAINWINDOW_H

#include <dmainwindow.h>
#include <dactionmanager.h>

#include "adusermodulelist.h"
#include "adspacemodulelist.h"
#include "adaudiovisualmodulelist.h"

#include <QMap>
#include <QTextEdit>
#include <QAction>
#include "adresis.h"

#include "aduserform.h"
#include "adaudiovisualform.h"
#include "adspaceform.h"

/**
 * @author Jorge Cuadrado <kuadrosxx@gamail.com>
*/
class ADMainWindow : public DMainWindow
{
	Q_OBJECT;
	public:
		ADMainWindow();
		~ADMainWindow();

	private:
		Adresis *m_adresis;
		DActionManager *m_actionManager;
		QMap<Logic::TypeModule, ADCModuleList*>m_modules;
		
		void setupActions();
		void setupMenu();
		void setupToolbar();
		void createActions();
		QMenu *fileMenu;
		QMenu *preferencesMenu;
		QMenu *helpMenu;

		QAction *exitAct;
		QAction *theme;
		QAction *aboutAct;




		
	private slots:
		void showTipDialog();
		void connectToHost();
		void about();
		void changeTheme();

		
		
	public slots:
		void showDialog(Msg::Type type, const QString& message);
		void fillModule(Logic::TypeModule, const QList<XMLResults>&);
		
		void createModules();
		void createUserForm();
		void createAudiovisualForm();
		void createSpaceForm();
		void addForm(ADFormBase * form, const QString & title);
		
};

#endif
