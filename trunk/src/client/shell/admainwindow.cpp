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
#include <ktpreferences.h>

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
#include "cconnectiondialog.h"


ADMainWindow::ADMainWindow() : DMainWindow()
{
	setWindowTitle(tr("Adresis Client"));
	DCONFIG->beginGroup("TipOfDay");
	m_adresis = new Adresis();
	
	connect(m_adresis, SIGNAL(requestShowMessage( Msg::Type, const QString&)), this, SLOT(showDialog( Msg::Type, const QString& )));
	connect(m_adresis, SIGNAL(requestCreateModules()), this, SLOT(createModules()));
	connect(m_adresis, SIGNAL(requestFillModule(Logic::TypeModule, const QList<XMLResults>&)), this, SLOT(fillModule(Logic::TypeModule, const QList<XMLResults>&)));
	connect(m_adresis, SIGNAL(showUser(const ADUser& )), this, SLOT(createUserForm(const ADUser& )));
	
	connect(m_adresis, SIGNAL(showSpace(const ADSpace& )), this, SLOT(createSpaceForm(const ADSpace& )));	
	
// 	createModule("users", QStringList() << tr("login") << tr("name"));

// 	connectToHost();

	DCONFIG->beginGroup("TipOfDay");
	bool showTips = qvariant_cast<bool>(DCONFIG->value("ShowOnStart", true ));
	
	if ( showTips )
	{
		QTimer::singleShot(0, this, SLOT(showTipDialog()));
	}
	m_actionManager = new DActionManager(this);
	
	
	setupActions();
	setupToolbar();
	setupMenu();
	
}

void ADMainWindow::setupActions()
{


	exitAct = new QAction(tr("E&xit"), this);
        exitAct->setShortcut(tr("Ctrl+Q"));
        exitAct->setStatusTip(tr("Exit the application"));
        connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
	
	conect = new QAction(tr("Connect"), this);
	exitAct->setStatusTip(tr("Connect to the Data Base"));
        connect(conect, SIGNAL(triggered()), this, SLOT(connectToHost()));

	theme = new QAction(tr("Theme"), this);
        theme->setStatusTip(tr("Change your window theme"));
        connect(theme, SIGNAL(triggered()), this, SLOT(changeTheme()));
	
        aboutAct = new QAction(tr("&About"), this);
        aboutAct->setStatusTip(tr("Show the application's About box"));
        connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

}

void ADMainWindow::about()
{
	QMessageBox::about(this, tr("About Application"),
	tr("ADRESIS (ADministrador de REcursos de SIStemas)\n"
		"es una aplicacion para el manejo\n"
		"de los recursos de la escuela EISC\n"
		"\n"
		"DESARROLLADORES\n"
		"Jorge Humberto Cuadrado	'EL TOPO'\n"
		"Hector Fabio Cruz		'Hectorcaz'\n"
		"Charly Aguirre		'Charlito'\n"
		"Juan Carlos Borrero		'El Abuelo'\n"
		"Sebastian Henao		'El Borracho'\n"));
}

void ADMainWindow::changeTheme()
{
	KTPreferences *themeSel = new KTPreferences(0);
	themeSel->show();
	
}


void ADMainWindow::setupMenu()
{

	fileMenu = menuBar()->addMenu(tr("&File"));
        fileMenu->addAction(exitAct);
	fileMenu->addAction(conect);

        preferencesMenu = menuBar()->addMenu(tr("&Preferences"));
	preferencesMenu->addAction(theme);

	menuBar()->addSeparator();

	
        helpMenu = menuBar()->addMenu(tr("&Help"));
        helpMenu->addAction(aboutAct);

	
}

ADMainWindow::~ADMainWindow()
{
}

void ADMainWindow::setupToolbar()
{
	
}

void ADMainWindow::showTipDialog()
{
	dDebug() << DATA_DIR+"/tips";
	DTipDialog *m_tipDialog = new DTipDialog(DATA_DIR+"tips", this);
	m_tipDialog->show();
}


void ADMainWindow::connectToHost()
{
	CConnectionDialog connectionDialog;
	if ( connectionDialog.exec() != QDialog::Rejected )
	{
		m_adresis->connectToHost(connectionDialog.server(), connectionDialog.port());
		m_adresis->login(connectionDialog.user(), connectionDialog.password());
	}
}

void ADMainWindow::showDialog(Msg::Type type, const QString& message)
{
	switch(type)
	{
		default:
		{
			QMessageBox::information ( 0 , "Message", message, 0);
		}
	}
}

void ADMainWindow::fillModule(Logic::TypeModule module, const QList<XMLResults>&results)
{
	m_modules[module]->fill(results);
}

//FIXME: crear todos los modulos
void ADMainWindow::createModules()
{
	ADUserModuleList *users = new ADUserModuleList();
	m_modules.insert( Logic::users, users);
	toolWindow( DDockWindow::Left )->addWidget( "Users", users);
	m_adresis->getInfoModule( Logic::users );
	
	connect(users, SIGNAL(requestUserForm()), this, SLOT(createUserForm()));
	connect(users, SIGNAL(requestDelete(Logic::TypeModule, const QString&)), m_adresis, SLOT(execDelete(Logic::TypeModule, const QString&)));
	connect(users, SIGNAL(requestUpdate(Logic::TypeModule, const QString& )), m_adresis, SLOT(getObject(Logic::TypeModule, const QString& )));
	
// __ESPACIOS____
	ADSpaceModuleList *spaces = new ADSpaceModuleList();
	m_modules.insert( Logic::spaces, spaces);
	toolWindow( DDockWindow::Left )->addWidget( "Spaces", spaces);
	m_adresis->getInfoModule( Logic::spaces );
	
	connect(spaces, SIGNAL(requestSpaceForm()), this, SLOT(createSpaceForm()));
	connect(spaces, SIGNAL(requestDelete(Logic::TypeModule, const QString&)), m_adresis, SLOT(execDelete(Logic::TypeModule, const QString&)));
	connect(spaces, SIGNAL(requestUpdate(Logic::TypeModule, const QString& )), m_adresis, SLOT(getObject(Logic::TypeModule, const QString& )));

// __AUDIOVISUAL____
	ADAudiovisualModuleList *audiovisual = new ADAudiovisualModuleList();
	m_modules.insert( Logic::audiovisuals, audiovisual);
	toolWindow( DDockWindow::Left )->addWidget( "Audiovisuals", audiovisual);
	m_adresis->getInfoModule( Logic::audiovisuals );
	
 	connect(audiovisual, SIGNAL(requestAudiovisualForm()), this, SLOT(createAudiovisualForm()));
 	connect(audiovisual, SIGNAL(requestDelete(Logic::TypeModule, const QString&)), m_adresis, SLOT(execDelete(Logic::TypeModule, const QString&)));
}

void ADMainWindow::createUserForm()
{
	dDebug() << "LLEGUE A CREAR USUARIO";
	dDebug();
	ADUserForm *form = new ADUserForm;
	connect(form, SIGNAL(requestInsertUser(const QString& , const QString& ,const QString& ,const QString& ,QMap<Logic::TypeModule, bool>  )), m_adresis, SLOT(addUser(const QString& , const QString& ,const QString& ,const QString& ,QMap<Logic::TypeModule, bool>  )));
	connect(form, SIGNAL(requestUpdateUser(const QString& , const QString& ,const QString& ,const QString& ,QMap<Logic::TypeModule, bool>  )), m_adresis, SLOT(modifyUser(const QString& , const QString& ,const QString& ,const QString& ,QMap<Logic::TypeModule, bool>)));
	addForm( form, tr("A�adir Usuario"));
}

void ADMainWindow::createSpaceForm()
{
	dDebug() << "LLEGUE A CREAR ESPACIO";
	ADSpaceForm *sform = new ADSpaceForm;
	connect(sform, SIGNAL(requestInsertSpace(const QString&, const QString&,const bool &, const QString&, const QString&) ), m_adresis, SLOT(addSpace(const QString&, const QString&,const bool&, const QString&, const QString&)));
	addForm( sform, tr("A�adir Espacio"));
	
}

void ADMainWindow::createSpaceForm(const ADSpace & space)
{
	D_FUNCINFO;
	ADSpaceForm *form = new ADSpaceForm(space);
	connect(form, SIGNAL(requestUpdateSpace(const QString&, const QString&, const bool, const QString&, const QString& )), m_adresis, SLOT(modifySpace(const QString&, const QString&, const bool&, const QString&, const QString& )));
	addForm( form, tr("Modificar Espacio"));
}

void ADMainWindow::createAudiovisualForm()
{
	dDebug() << "LLEGUE A CREAR AYUDA";
	
	ADAudiovisualForm *aform = new ADAudiovisualForm;
	connect(aform, SIGNAL(requestInsertAudiovisual(const QString&, const QString&, const QString&, const QString&, const QString&) ), m_adresis, SLOT(addAudiovisual(const QString&, const QString&, const QString&, const QString&, const QString&)));
	
	//cambio
	connect(aform, SIGNAL(requestUpdateAudiovisual(const QString&, const QString&, const QString&, const QString&, const QString& )), m_adresis, SLOT(modifyAudiovisual(const QString&, const QString&, const QString&, const QString&, const QString&)));
	
	addForm( aform, tr("A�adir Ayuda Audiovisual"));
	
}


void ADMainWindow::addForm(ADFormBase * form, const QString & title )
{
	D_FUNCINFO;
	if ( form )
	{
		form->setTitle(title);
		addWidget( form, title, false);
		connect(form, SIGNAL(requestClose()), this, SLOT(closeTab()));
	}
}

void ADMainWindow::createUserForm(const ADUser & user)
{
	D_FUNCINFO;
	ADUserForm *form = new ADUserForm(user);
	connect(form, SIGNAL(requestUpdateUser(const QString& , const QString& ,const QString& ,const QString& ,QMap<Logic::TypeModule, bool>  )), m_adresis, SLOT(modifyUser(const QString& , const QString& ,const QString& ,const QString& ,QMap<Logic::TypeModule, bool>)));
	addForm( form, tr("Modificar Usuario"));
}

void ADMainWindow::createAudiovisualForm(const ADAudioVisual & audiovisual)
{
	D_FUNCINFO;
	ADAudiovisualForm *aform = new ADAudiovisualForm(audiovisual);
	connect(aform, SIGNAL(requestUpdateAudiovisual(const QString&, const QString&, const QString&, const QString&, const QString& )), m_adresis, SLOT(modifyAudiovisual(const QString&, const QString&, const QString&, const QString&, const QString&)));
	addForm( aform, tr("Modificar Ayuda audio visual"));
}


