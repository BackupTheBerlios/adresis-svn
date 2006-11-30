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

#include <dtabbedmainwindow.h>
#include <dactionmanager.h>

#include "adeventhandler.h"
#include "adusermodulelist.h"
#include "adspacemodulelist.h"
#include "adaudiovisualmodulelist.h"
#include "adreservemodulelist.h"
#include "adconfigschoollmodule.h"
#include "adinsertresource.h"

#include <QMap>
#include <QTextEdit>
#include <QAction>
#include <QToolBar>
#include <QVariant>
#include "adresis.h"


#include "aduserform.h"
#include "adaudiovisualform.h"
#include "adspaceform.h"
#include "adreservefform.h"
#include "adreservetform.h"
#include "ddockwindow.h"


#include "adeventhandler.h"
#include "dtoolview.h"

/**
 * @author Jorge Cuadrado <kuadrosxx@gmail.com>
*/
class ADMainWindow : public DTabbedMainWindow, public ADAbstractEventHandler
{
	Q_OBJECT;
	public:
		ADMainWindow();
		~ADMainWindow();
		void handleEvent(ADEvent * event = 0);
		
	private:
		Adresis *m_adresis;
		DActionManager *m_actionManager;
		QMap<Logic::Module, ADCModuleList*> m_modules;
		QString tipo;
		QString typeRequest;
		
		QMenu *fileMenu;
		QMenu *preferencesMenu;
		QMenu *configureMenu;
		QMenu *helpMenu;
		
		QToolBar *toolBar;

		QAction *exitAct;
		QAction *theme;
		QAction *aboutAct;
		QAction *conect;
		QAction *configSchoollAct;
		QAction *configResourceAct;
		
		void setupActions();
		void setupMenu();
		void setupToolbar();
		void createActions();
		
	private slots:
		void showTipDialog();
		void connectToHost();
		void about();
		void changeTheme();
		void configSchooll();
		void showModule(Logic::Module module,const QList<QVariant> &);
		
	public slots:
		void showDialog(Msg::Type type, const QString& message);
		void addForm(ADFormBase * form, const QString & title);
		void showForm(Logic::Module module,const QString & key);
};

#endif
