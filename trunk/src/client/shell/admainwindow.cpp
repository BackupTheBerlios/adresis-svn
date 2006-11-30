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


ADMainWindow::ADMainWindow() : DTabbedMainWindow()
{
	DCONFIG->beginGroup("ConfigurationOfSchooll");
	QString titulo = "Adresis - " + qvariant_cast<QString>(DCONFIG->value("NameSchooll"));
	setWindowTitle(titulo);
	QIcon icon = QIcon(qvariant_cast<QString>(DCONFIG->value("LogoPath")));
	setWindowIcon(icon);
	
	DCONFIG->beginGroup("TipOfDay");
	m_adresis = new Adresis();
	
	connect(m_adresis, SIGNAL(requestShowMessage( Msg::Type, const QString&)), this, SLOT(showDialog( Msg::Type, const QString& )));
	
	connect(m_adresis, SIGNAL(requestShowModule( Logic::Module, const QList<QVariant> &)), this, SLOT(showModule( Logic::Module, const QList<QVariant> & )));
	
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
	exitAct = new QAction(tr("&Exit"), this);
	exitAct->setShortcut(tr("Ctrl+Q"));
	exitAct->setStatusTip(tr("Exit the application"));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

	conect = new QAction(QPixmap(THEME_DIR+"/icons/connect.png"),tr("Connect"), this);
	conect->setStatusTip(tr("Connect to the Data Base"));
	connect(conect, SIGNAL(triggered()), this, SLOT(connectToHost()));
	connect(this, SIGNAL(disabledConnect(bool)), conect, SLOT(setEnabled(bool)));

	configSchoollAct = new QAction(tr("Configure Schooll"), this);
	configSchoollAct->setStatusTip("Configure the information about the schooll");
	configSchoollAct->setEnabled(false);
	connect(configSchoollAct, SIGNAL(triggered()), this, SLOT(configSchooll()));

	configResourceAct = new QAction(tr("Add/Del Resources"), this);
	configResourceAct->setStatusTip("Add or del resource from schooll");
	configResourceAct->setEnabled(false);
	connect( configResourceAct, SIGNAL( triggered() ), this, SLOT( configResource() ));

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
	tr("ADRESIS \n"));
}

void ADMainWindow::changeTheme()
{
	KTPreferences *themeSel = new KTPreferences(0);
	themeSel->show();
}

void ADMainWindow::configSchooll()
{
	ADConfigSchoollModule *config = new ADConfigSchoollModule(0);
	config->show();
}

void ADMainWindow::showModule(Logic::Module module,const QList<QVariant> &values)
{
	D_FUNCINFO;
	ADCModuleList *list = new ADCModuleList(module);
	list->fill( values );
	addToolView(list, Qt::LeftDockWidgetArea)->setDescription("titulo");
	m_modules.insert( module, list );
	connect(list, SIGNAL(requestShowForm( Logic::Module , const  QString & )), this, SLOT(showForm( Logic::Module, const QString & )));
	
	connect(list, SIGNAL(sendEvent(ADEvent * )), m_adresis, SLOT( handleEvent( ADEvent * )));
	
	connect(m_adresis, SIGNAL(requestAddDataToModule(Logic::Module, const QVariant & )), list, SLOT(addData( Logic::Module, const QVariant & )));
	connect(m_adresis, SIGNAL(requestRemoveDataToModule(Logic::Module, const QString & )), list, SLOT(removeData( Logic::Module, const QString & )));
	connect(m_adresis, SIGNAL(requestUpdateDataToModule(Logic::Module, const QVariant & )), list, SLOT(updateData( Logic::Module, const QVariant & )));
	
}

void ADMainWindow::setupMenu()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(exitAct);
	fileMenu->addAction(conect);

	preferencesMenu = menuBar()->addMenu(tr("&Preferences"));
	preferencesMenu->addAction(theme);
	
	configureMenu = menuBar()->addMenu(tr("&Configure"));
	configureMenu->addAction(configSchoollAct);
	configureMenu->addAction(configResourceAct);

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


void ADMainWindow::addForm(ADFormBase * form, const QString & title )
{
	D_FUNCINFO;
	if ( form )
	{
		form->setTitle(title);
		connect(form, SIGNAL(requestClose()), this, SLOT(closeTab()));
		connect(form, SIGNAL(sendEvent(ADEvent *)), m_adresis, SLOT(handleEvent(ADEvent *)));
		form->show();
		addWidget(form);
	}
}

void ADMainWindow::showForm( Logic::Module module, const QString & key )
{
	ADFormBase * form;
	switch( module )
	{
		case Logic::Users:
		{
			if(key.isNull())
			{
				form = new ADUserForm;
				addForm(form, tr("Add user"));
			}
			else
			{
				form = new ADUserForm(static_cast<ADUser *>(m_adresis->getObject(Logic::Users, key ))) ;
				addForm(form, tr("Modify user"));
			}
		}
		break;
		case Logic::Spaces:
		{
			form = new ADSpaceForm;
			addForm(form, tr("Add space"));
		}
		break;
		case Logic::Audiovisuals:
		{
			form = new ADAudiovisualForm;
			addForm(form, tr("Add audiovisual"));
		}
		break;
		case Logic::ReservesT:
		{
			form = new ADReserveTForm();
			addForm(form, tr("Add Reserve Temporal"));
		}
		break;
		case Logic::ReservesF:
		{
			form = new ADReserveFForm();
			addForm(form, tr("Add Reserve Semestral"));
		}
		break;
	}
}

/**
 * Realiza acciones para modificar la informacion que llega del servidor
 * Por ejemplo eliminar de la lista de usuarios si llega un evento de borrado del modulo Usuario
 */
void ADMainWindow::handleEvent(ADEvent * event)
{
	if(event->source() == ADEvent::Client)
	{
		switch(event->module())
		{
			case Logic::Users:
			{
			}
			break;
			case Logic::Audiovisuals:
			{
			}
			break;
			case Logic::ReservesF:
			{
			}
			break;
			case Logic::ReservesT:
			{
			}
			break;
			case Logic::Spaces:
			{
			}
			break;
		}
	}
	else
	{
		switch(event->module())
		{
			case Logic::Users:
			{
			}
			break;
			case Logic::Audiovisuals:
			{
			}
			break;
			case Logic::ReservesF:
			{
			}
			break;
			case Logic::ReservesT:
			{
			}
			break;
			case Logic::Spaces:
			{
			}
			break;
		}
	}
}

