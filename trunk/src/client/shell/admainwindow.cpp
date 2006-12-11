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

#include "adviewhtml.h"

//adresisLib
#include "cconnectiondialog.h"
#include "adconsultschedule.h"
#include "adreportgenerator.h"
#include "adreportform.h"


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
	connect(m_adresis, SIGNAL(requestShowReport(ADReport *)), this, SLOT(showReport(ADReport *)));
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
	consultSchedule = new QAction(tr("&Consultar Horarios"), this);
	consultSchedule->setStatusTip(tr("Muestra un dialogo para hacer la consulta de los horarios"));
	connect(consultSchedule, SIGNAL(triggered()), this, SLOT(showConsultSchedule()));

	
	exitAct = new QAction(tr("&Salir"), this);
	exitAct->setShortcut(tr("Ctrl+Q"));
	exitAct->setStatusTip(tr("Salir de la aplicacion"));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

	conect = new QAction(QPixmap(THEME_DIR+"/icons/connect.png"),tr("Conectar"), this);
	conect->setStatusTip(tr("Conectar a la base de datos"));
	connect(conect, SIGNAL(triggered()), this, SLOT(connectToHost()));
	connect(this, SIGNAL(disabledConnect(bool)), conect, SLOT(setEnabled(bool)));

	configResourceAct = new QAction(tr("Agregar/Eliminar Recursos"), this);
	configResourceAct->setStatusTip("Agragar o Eliminar Recursos de la Escuela");
	configResourceAct->setEnabled(false);
	connect( configResourceAct, SIGNAL( triggered() ), this, SLOT( configResource() ));

	theme = new QAction(tr("Tema"), this);
	theme->setStatusTip(tr("Cambiar el tema de la ventana"));
	connect(theme, SIGNAL(triggered()), this, SLOT(changeTheme()));

	aboutAct = new QAction(tr("&Acerca"), this);
	aboutAct->setStatusTip(tr("Acerca de ADRESIS"));
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

}

void ADMainWindow::about()
{
	QMessageBox::about(this, tr("Acerca de ADRESIS"),
			   tr("ADRESIS\nDesarrolladores:\nJorge Humberto Cuadrado\t\"EL TOPO\"\tDirector de Proyecto\nHector Fabio Cruz\t\"HECTORCAZ\"\tProgramador\nCharly Aguirre\t\"LA MARIMONDA\"\tAsegurador de la Calidad\nJuan Carlos Borrero\t\"EL ABUELO\"\tJefe de Documentacion\nSebastian Henao\t\"FRESITA\"\tDocumentador/Bases de datos"));
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
	setUpdatesEnabled(false);
	ADCModuleList *list = new ADCModuleList(module);
	list->fill( values );
	addToolView(list, Qt::LeftDockWidgetArea)->setDescription("titulo");
	m_modules.insert( module, list );
	
	connect(list, SIGNAL(requestShowMessage( Msg::Type, const QString&)), this, SLOT(showDialog( Msg::Type, const QString& )));
	
	connect(list, SIGNAL(requestShowForm( Logic::Module , const  QString & )), this, SLOT(showForm( Logic::Module, const QString & )));
	
	connect(list, SIGNAL(requestShowElement( Logic::Module , const  QString & )), this, SLOT(showElement( Logic::Module, const QString & )));
	
	connect(list, SIGNAL(sendEvent(ADEvent * )), m_adresis, SLOT( handleEvent( ADEvent * )));
	
	connect(m_adresis, SIGNAL(requestAddDataToModule(Logic::Module, const QVariant & )), list, SLOT(addData( Logic::Module, const QVariant & )));
	connect(m_adresis, SIGNAL(requestRemoveDataToModule(Logic::Module, const QString & )), list, SLOT(removeData( Logic::Module, const QString & )));
	connect(m_adresis, SIGNAL(requestUpdateDataToModule(Logic::Module, const QVariant & )), list, SLOT(updateData( Logic::Module, const QVariant & )));
	
	setUpdatesEnabled(true);
}

void ADMainWindow::setupMenu()
{
	fileMenu = menuBar()->addMenu(tr("&Archivo"));
	fileMenu->addAction(consultSchedule);
	fileMenu->addAction(exitAct);
	
	netMenu = menuBar()->addMenu(tr("&Red"));
	netMenu->addAction(conect);
	
	preferencesMenu = menuBar()->addMenu(tr("&Preferencias"));
	preferencesMenu->addAction(theme);
	
	menuBar()->addSeparator();
	
	helpMenu = menuBar()->addMenu(tr("&Ayuda"));
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


/**
 * Se conecta al servidor y autentica al usuario
 */
void ADMainWindow::connectToHost()
{
	
	CConnectionDialog connectionDialog;
	if ( connectionDialog.exec() != QDialog::Rejected )
	{
		setUpdatesEnabled ( false);
		m_adresis->connectToHost(connectionDialog.server(), connectionDialog.port());
		m_adresis->login(connectionDialog.user(), connectionDialog.password());
		setUpdatesEnabled ( true);
	}
	
}

void ADMainWindow::showDialog(Msg::Type type, const QString& message)
{
	switch(type)
	{
		default:
		{
			QMessageBox::information ( 0 , "Mensaje", message, 0);
		}
	}
}


void ADMainWindow::addForm(ADFormBase * form, const QString & title )
{
	D_FUNCINFO;
	if ( form )
	{
		form->setTitle(title);
		form->setWindowTitle(title);
		connect(form, SIGNAL(requestClose()), this, SLOT(closeTab()));
		connect(form, SIGNAL(sendEvent(ADEvent *)), m_adresis, SLOT(handleEvent(ADEvent *)));
		form->show();
		addWidget(form);
	}
}

void ADMainWindow::closeTab()
{
	closeCurrentTab();
}


void ADMainWindow::showForm( Logic::Module module, const QString & key )
{
	ADFormBase *form;
	QString title;
	switch( module )
	{
		case Logic::Users:
		{
			if(key.isNull())
			{
				form = new ADUserForm;
				title = tr("Agregar Usuario");
			}
			else
			{
				form = new ADUserForm(static_cast<ADUser *>(m_adresis->getObject(Logic::Users, key )));
				
				title = tr("Modificar Usuario");
			}
		}
		break;
		case Logic::Spaces:
		{
			if(key.isNull())
			{
				QList<QVariant> ayudas = m_adresis->getList(Logic::Audiovisuals);
				QList<QVariant> ayudasLibres;
				
				foreach( QVariant variant, ayudas)
				{
					ADAudioVisual a = *(qvariant_cast<ADAudioVisual *>(variant));
					if(a.codeSpace() == "null")
					{
						ayudasLibres << variant;
					}
				}
				form = new ADSpaceForm(m_adresis->getTypes(Logic::Spaces), ayudasLibres);
				title = tr("Agregar Espacio");
			}
			else
			{
				QList<QVariant> ayudas = m_adresis->getList(Logic::Audiovisuals);
				QList<QVariant> ayudasLibres;
				QList<QVariant> ayudasAsignadas;
				
				foreach( QVariant variant, ayudas)
				{
					ADAudioVisual a = *(qvariant_cast<ADAudioVisual *>(variant));
					if(a.codeSpace() == "null")
					{
						ayudasLibres << variant;
					}
					else if(a.codeSpace() == key)
					{
						dDebug() << "ayuda asignada a " << a.codeSpace() << "  estoy buscando ayudas asignadas a " << key;
						ayudasAsignadas << variant;
						dDebug() << "asignadas = " << ayudasAsignadas.count();
					}
				}
				ADSpace space = (*static_cast<ADSpace *>(m_adresis->getObject(Logic::Spaces, key )));
				form = new ADSpaceForm(space, ayudasLibres, ayudasAsignadas);
				title = tr("Modificar Espacio");
			}
		}
		break;
		case Logic::Audiovisuals:
		{
			if(key.isNull())
			{
				form = new ADAudiovisualForm(m_adresis->getList(Logic::Spaces), m_adresis->getTypes(Logic::Audiovisuals));
				title = tr("Agregar Ayuda Audiovisual");
			}
			else
			{
				form = new ADAudiovisualForm(static_cast<ADAudioVisual *>(m_adresis->getObject(Logic::Audiovisuals, key )), m_adresis->getList(Logic::Spaces), m_adresis->getTypes(Logic::Audiovisuals));
				title = tr("Modificar Ayuda Audiovisual");
			}
		}
		break;
		case Logic::ReservesT:
		{
			if(key.isNull())
			{
				form = new ADReserveTForm();
				title = tr("Agregar Reserva Temporal");
			}
			else
			{
				ADReserve *m_reserve = static_cast<ADReserve *>(m_adresis->getObject(Logic::ReservesT , key ));
				/// INFORMACION DEL RECURSO
				QList<QString> infoResource;
				if(m_reserve->idspace() != "") //INFO SPACE
				{
					ADSpace *space = static_cast<ADSpace *>(m_adresis->getObject(Logic::Spaces , m_reserve->idspace() ));
					infoResource << space->typeSpace() << space->nameSpace();
				}
				else // INFO AUDIOVISUAL
				{
					ADAudioVisual *audiovisual = static_cast<ADAudioVisual *>(m_adresis->getObject(Logic::Audiovisuals , m_reserve->idaudiovisual() ));
					infoResource << audiovisual->type() << audiovisual->type();
				}
				
				
				/// LISTAS DE LAS RESERVAS
				QList<ADReserve *> listReserves;
				listReserves << m_reserve;
				QList<QVariant> list = m_adresis->getList(Logic::ReservesF);
				list << m_adresis->getList(Logic::ReservesT);
									
				for(int i=0; i < list.count();i++)
				{
					ADReserve *reserve = qVariantValue<ADReserve *>(list.at(i));
					if(reserve->idspace() == m_reserve->idspace() && reserve->idaudiovisual() == m_reserve->idaudiovisual() && reserve->idReserve() != m_reserve->idReserve() )
					{
						listReserves << reserve;
					}
				}
				
				
				form = new ADReserveTForm( m_reserve, infoResource, listReserves );
				title = tr("Modificar Reserva Temporal");
			}
			
		}
		break;
		case Logic::ReservesF:
		{
			if(key.isNull())
			{
				form = new ADReserveFForm();
				title = tr("Agregar Reserva Semestral");
			}
			else
			{
				ADReserve *m_reserve = static_cast<ADReserve *>(m_adresis->getObject(Logic::ReservesF, key ));
				
				QList<QString> infoResource;
				if(m_reserve->idspace() != "") //INFO SPACE
				{
					ADSpace *space = static_cast<ADSpace *>(m_adresis->getObject(Logic::Spaces , m_reserve->idspace() ));
					infoResource << space->typeSpace() << space->nameSpace();
				}
				else // INFO AUDIOVISUAL
				{
					ADAudioVisual *audiovisual = static_cast<ADAudioVisual *>(m_adresis->getObject(Logic::Audiovisuals , m_reserve->idaudiovisual() ));
					infoResource << audiovisual->type() << audiovisual->type();
				}
				
				/// LISTAS DE LAS RESERVAS
				QList<ADReserve *> listReserves;
				listReserves << m_reserve;
				QList<QVariant> list = m_adresis->getList(Logic::ReservesF);
				list << m_adresis->getList(Logic::ReservesT);
									
				for(int i=0; i < list.count();i++)
				{
					ADReserve *reserve = qVariantValue<ADReserve *>(list.at(i));
					if(reserve->idspace() == m_reserve->idspace() && reserve->idaudiovisual() == m_reserve->idaudiovisual() && reserve->idReserve() != m_reserve->idReserve() )
					{
						listReserves << reserve;
					}
				}
				
				form = new ADReserveFForm( m_reserve, infoResource, listReserves );
				title = tr("Modificar Reserva Semestral");
			}
		}
		break;
		case Logic::Cancellation:
		{
			ADCancellation *cancel = static_cast<ADCancellation *>(m_adresis->getObject(Logic::Cancellation, key ));
			form = new ADCancellationForm( cancel );
			title = tr("Informacion Cancelacion");
		}
		break;
		case Logic::Reports:
		{
			form = new ADReportForm(m_adresis->user()->login());
			title = tr("Create Report");
		}
		break;
	}
	addForm( form, title );
}


void ADMainWindow::showElement(Logic::Module module,const QString & key)
{
	if(module == Logic::Reports)
	{
		showReport(static_cast<ADReport *>(m_adresis->getObject(Logic::Reports, key )));
	}
	else
	{
		dWarning() << "no se puede consultar el elemento";
	}
}

void ADMainWindow::showReport(ADReport * report)
{
	if(report)
	{
		QString html;
		if(report->type() == ADReport::List)
		{
			html = report->content();
		}
		else if(report->type() == ADReport::Histogram)
		{
			html = ADReportGenerator::generateGraphicReport( report->content() )->toHtml();
		}
		showHtml(html , "Report ");
		
		
	}
}

void ADMainWindow::showConsultSchedule()
{
	ADConsultSchedule dialog(m_adresis->getList(Logic::Audiovisuals ), m_adresis->getList(Logic::Spaces ));
	if( dialog.exec() != QDialog::Rejected )
	{
		QString key = dialog.currentElemet();
		QList<QVariant> reservesF = m_adresis->getList( Logic::ReservesF );
		//Encontramos todas las reservas de el elemento seleccionado
		QList<ADReserve *> reservesOfElement;
		bool ok = false;
		foreach(QVariant rf, reservesF)
		{
			//primero las reservas fijas
			ADReserve *rfR = qvariant_cast<ADReserve *>(rf);
			if(rfR)
			{
				if( dialog.type() == "Ayuda audiovisual" )
				{
					ok = true;
					dDebug() << rfR->idaudiovisual() + "=="+  key;
					if(rfR->idaudiovisual() == key)
					{
						reservesOfElement << rfR;
					}
				}
				else
				{
					if(rfR->idspace() == key)
					{
						reservesOfElement << rfR;
					}	
				}
			}
		}
		QString title = "Horario semestral ";
		if(ok)
		{
			title += "de la ayuda audiovisual ";
		}
		else
		{
			title += "del espacio";
		}
		QString html = ADReportGenerator::generateSchedule( reservesOfElement, title+key )->toHtml();
		showHtml(html, title + key );
	}
}


void ADMainWindow::showHtml(const QString& html, const QString & title )
{
	ADViewHtml *browser = new ADViewHtml();
	browser->setWindowTitle(title);
	browser->setHtml (html);
	addWidget(browser);
}
