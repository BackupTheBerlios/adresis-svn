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
#include "admainwindow.h"
//dlib
#include <dtabwidget.h>
#include <ddebug.h>
#include <dconfig.h>
#include <dglobal.h>
#include <dtip.h>
#include <dalgorithm.h>

//QT
#include <QLabel>
#include <QTimer>
#include <QMenuBar>
#include <QStatusBar>
#include <QMenu>
#include <QToolBar>
#include <QMessageBox>
#include <QScrollArea>

//adresisLib
#include "adcmodulelist.h"


ADMainWindow::ADMainWindow() : DMainWindow()
{
	setWindowTitle(tr("Adresis Client"));
	DCONFIG->beginGroup("TipOfDay");
	

	QFrame *container = new QFrame;
	
	bool showTips = qvariant_cast<bool>(DCONFIG->value("ShowOnStart", true ));
	dDebug() << DATA_DIR+"tips" << showTips;
	if ( showTips )
	{
		showTipDialog();
		QTimer::singleShot(0, this, SLOT(showTipDialog()));
	}
	m_actionManager = new DActionManager(this);
	setupActions();
	setupToolbar();
	setupMenu();
	
	
	createModule("users", QStringList() << tr("login") << tr("name"));
}

void ADMainWindow::setupActions()
{
	
}

void ADMainWindow::setupMenu()
{
	
}

void ADMainWindow::setupToolbar()
{
	
}


ADMainWindow::~ADMainWindow()
{
}

void ADMainWindow::showTipDialog()
{
	dDebug() << DATA_DIR+"/tips";
	DTipDialog *m_tipDialog = new DTipDialog(DATA_DIR+"tips", this);
	m_tipDialog->show();
}

void ADMainWindow::createModule(const QString& moduleName, const QStringList & titles)
{
	ADCModuleList *module = new ADCModuleList(moduleName, titles, this);
	
	toolWindow( DDockWindow::Left )->addWidget( moduleName, module);
	
}


