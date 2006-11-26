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
#include "adreservefform.h"
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>

#include <QGridLayout>
#include <QCheckBox>
#include <ddebug.h>
#include <QMessageBox>


ADReserveFForm::ADReserveFForm(QWidget *parent)
	: ADFormBase("<h1><b>Audiovisuals</b><h1>" , parent)
{
	list=1;
	m_inserter = true;
	destinationReserve="";
	setup();
}


ADReserveFForm::~ADReserveFForm()
{
}


ADReserveFForm::ADReserveFForm(const ADReserve & reserve, QWidget * parent)
{
	D_FUNCINFO;
	setup();
	m_inserter = false;
	list=1;
}


void ADReserveFForm::changeTypeResource(int opcion)
{
// 	opcion = 0 -> Espacios
//	opcion = 1 -> audiovisual

	if(opcion == 0)
	{
		recursosC->clear();
		ADEvent listTypes(ADEvent::Client, Logic::ReservesF, Logic::Find, "spaces");
		recursosC->addItems(recursosEsp);
	}

	else if(opcion == 1)
	{
		recursosC->clear();
		recursosC->addItems(recursosAud);
	}
}

// Este metodo se encarga de tomar el tipo de reserva en el momento en que sea cambiado en el comboBox y de acuerdo a esto construir una tabla horario, ademas cuando se cambia el tipo de reserva si el otro comboBox( resourcesNameC ) tiene activado un nombre valido, se llamara al metodo fill de horario para que llene las reservas de este tipo.
// void ADReserveFForm::changeTypeReserve(int opcion)
// {
// 	if(opcion == 0)
// 	{
// 		horario->makeTable( true );
// 	}
// 	else if(opcion == 1)
// 	{
// 		horario->makeTable( false );
// 	}
// 	
// 	if( (resourcesNameC->currentText()) !=("Escoja uno de los Recursos") && (resourcesNameC->currentText()) !=("") )
// 	{
// 		horario->assignTypeReserve( tipoResC->currentText() );
// 		horario->fill();
// 	}
// }


void ADReserveFForm::insertListTypes(const QList<XMLResults>& results)
{
	QList<XMLResults>::const_iterator it= results.begin();
	
	QStringList recursos;
	recursos.insert(0, "ESCOJA UN RECURSO");
	while( it != results.end() )
	{
		recursos << ((*it)["type"]);
		++it;
	}
	
	if(list == 1)
	{
		recursosEsp = recursos;
	}

	else if(list == 2)
	{
		dDebug() << "Llenar recursosAud";
		recursosAud = recursos;

		recursosC->addItems(recursosEsp);
		changeTypeResource(0);
		
		//Con este condicional estoy manejando el caso en que la reserva la haga un usuario cualquiera o el administrador. si es administrador el campo responsable aparecera en blanco y editable para que el coloque el login de la persona a la cual le esta realizando la reserva. En caso que no sea administrador se colocara en responsable el login de el usuario. Nota:En la base de datos hay dos campos que son idUserReserve que es quien hace la reserva y el campo idUserResponsable es quien se reponsabiliza por la reserva, en caso de usuario es lo mismo, en caso de administrador son cosas diferentes.
		if(!m_reserve)
		{
			static_cast<QLineEdit*>(m_inputs[tr("responsable")])->setText(m_responsable);
			static_cast<QLineEdit*>(m_inputs[tr("responsable")])->setReadOnly ( true );
		}
	}
	
	if(list==2 && !m_inserter)
	{
		dDebug() << "ENTRE A LLAMAR A FILL";
		fill();
	}

	list++;
}


void ADReserveFForm::fill()
{
// 	if(adAudiovisual->isValid())
// 	{
// 		tipoResC->setCurrentIndex(tiposRC->findText(adAudiovisual->type())); 
// 		static_cast<QLineEdit*>(m_inputs[tr("marca")])->setText(adAudiovisual->marksEquipment());
// 		estadoC->setCurrentIndex(estadoC->findText(adAudiovisual->estate()));
// 		static_cast<QLineEdit*>(m_inputs[tr("numero de inventario")])->setText(adAudiovisual->numberInventory());
// 		static_cast<QLineEdit*>(m_inputs[tr("asignado al espacio")])->setText(adAudiovisual->codeSpace());
// 
// 		tipoResC->currentText(),
// 		recursosC->currentText(),
// 		static_cast<QLineEdit*>(m_inputs[tr("responsable")])->text(),
// 		static_cast<QLineEdit*>(m_inputs[tr("login")])->text()
		
// 	}
}

void ADReserveFForm::changeResource( QString typeR)
{
	QString table;
	if((tipoRecC->currentText()).toLower() ==("espacios"))
	{
		table="adspace";
	}
	else
	{
		table="adaudiovisual";
	}
	
	
	if( typeR !=( "ESCOJA UN RECURSO" ) )
	{
		nameResources.clear();
		resourcesNameC->clear();
		emit requestTypeResources(table, typeR);
	}
}


// Este metodo se encarga de tomar la informacion del comboBox de los nombres de los espacios, en el momento en es modificado, y manda a pedir todas las reservas que hayan sobre ese espacio.
void ADReserveFForm::changeNameSpace(const QString& name)
{
	QString table;
	QString resource;
	
	if((tipoRecC->currentText()).toLower() ==("espacios"))
	{
		table="adspace";
	}
	else
	{
		table="adaudiovisual";
	}

	if( name !=( "Escoja uno de los Recursos" ) )
	{
		resource = nameResources.key(name);
		
		dDebug() << "changeNameSpace changeNameSpace";
		dDebug() << "TABLE >>>> " << table << "   RESOURCE >>>> " << resource;
		emit consultSchedule(table, resource );
		horario->clear();
	}
}

// Cuando el metodo anterior haya mandado a pedir las reservas de un espacio, este metodo se encargara de recibirlos y luego reenviarselos a horario que es la tabla donde se muestra la informacion de los horarios, y luego ejecuta el metodo fill (horario) pasandole por parametro el tipo de reserva para llenar la tabla con las reservas de este tipo.
void ADReserveFForm::requestSchedule( const QList<XMLResults>& results )
{
	horario->receiveSchedule( results );
	horario->fill();
}


void ADReserveFForm::insertListNameResources(const QList<XMLResults>& results)
{
	QList<XMLResults>::const_iterator it= results.begin();
	QStringList resources;
	int n=0;
	
	while( it != results.end() )
	{
		if(tipoRecC->currentText().toLower() ==("espacios"))
		{
			nameResources.insert( ((*it)["idresource"]) , ((*it)["nameresource"]));
		}
		else if(tipoRecC->currentText().toLower() ==("audiovisual"))
		{
			nameResources.insert( ((*it)["idresource"]), ""+recursosC->currentText()+" "+QString::number(n));
			n++;
		}
		++it;
	}

	resources = QStringList(nameResources.values());
	resources.insert(0,"Escoja uno de los Recursos");
	resourcesNameC->addItems( resources );
}

/*
void ADReserveFForm::permisos(const QString &login, const bool permiso)
{
	m_responsable=login;
	m_reserve=permiso;
}*/


void ADReserveFForm::setup()
{
	QWidget * base = new QWidget();
	QVBoxLayout *vBLayout = new QVBoxLayout(base);
	QGroupBox *container = new QGroupBox("Informacion");
	vBLayout->addWidget(container, Qt::AlignVCenter);
	horario = new ADSchedule();
	vBLayout->addWidget(horario);

	QLineEdit *edits;
	QStringList titles, tipoRec;

	QGridLayout *layout = new QGridLayout;
	container->setLayout(layout);
	
	tipoRec << tr("Espacios") << tr("Audiovisual");
	tipoRecC = new QComboBox;
	tipoRecC->addItems(tipoRec);
	connect(tipoRecC, SIGNAL(activated (int) ), this , SLOT(changeTypeResource ( int )));

	recursosC = new QComboBox;
	connect(recursosC, SIGNAL(activated (QString) ), this , SLOT(changeResource ( QString )));
	
	resourcesNameC = new QComboBox;
	connect(resourcesNameC, SIGNAL(activated (QString) ), this , SLOT(changeNameSpace ( QString )));

	horario->makeTable( true );
	
	titles << tr("Tipo Recurso") << tr("Recurso") << tr("Nombre Recurso")<< tr("Responsable") << tr("Motivo");

	layout->addWidget(new QLabel(titles[0]),0,0);
	layout->addWidget(tipoRecC,0,1);
	m_inputs.insert(titles[0].toLower () , tipoRecC);

	layout->addWidget(new QLabel(titles[1]),1,0);
	layout->addWidget(recursosC,1,1);
	m_inputs.insert(titles[1].toLower () , recursosC);
	
	layout->addWidget(new QLabel(titles[2]),2,0);
	layout->addWidget(resourcesNameC ,2,1);
	m_inputs.insert(titles[2].toLower () , resourcesNameC);
	
	layout->addWidget(new QLabel(titles[3]),3,0);
	edits = new QLineEdit();
	layout->addWidget(edits, 3, 1);
	m_inputs.insert( titles[3].toLower () , edits );
	
	layout->addWidget(new QLabel(titles[4]),4,0);
	areaTexto = new QTextEdit;
	layout->addWidget(areaTexto,4,1);
	
	setForm(base);
// 	connect(this, SIGNAL(requestDone()),this, SLOT(emitInsertReserve()));
}


void ADReserveFForm::emitInsertReserve()
{
	QString table;
	dDebug() << "EMITINSERTRESERVE===EMITINSERTRESERVE";
	listSchedules = horario->buildSchedule();
	QList<QMap<QString, QString> >::const_iterator it = listSchedules.begin();
	
	
	if( tipoRecC->currentText() ==("Espacios") )
	{
		table="adspacereserve";
	}
	else
	{
		table="adavreserve";
	}


	if(m_inserter && valite())
	{
		while( it != listSchedules.end())
		{
			emit requestInsertReserve(
				table,
				(*it)["typereserve"],
				m_responsable,
				static_cast<QLineEdit*>(m_inputs[tr("responsable")])->text(),
				nameResources.key(static_cast<QComboBox*>(m_inputs[tr("espacio")])->currentText()),
				(*it)["day"],
				(*it)["beginhour"],
				(*it)["endhour"],
				(*it)["begindate"],
				(*it)["enddate"],
				true,
				destinationReserve
				
			);
			it++;
		}
	}
// 	else
// 	{
// 		emit requestUpdateReserve(
// 			tipoResC->currentText(),
// 			recursosC->currentText(),
// 			static_cast<QLineEdit*>(m_inputs[tr("responsable")])->text()
// 		);
// 	}
	
}

bool ADReserveFForm::valite()
{
	bool isValid = true;
	QList<QMap<QString, QString> >::const_iterator it = listSchedules.begin();
	while( it != listSchedules.end())
	{
		if( (*it)["typereserve"].isEmpty() || m_responsable.isEmpty() || (static_cast<QLineEdit*> (m_inputs[tr("responsable")])->text()).isEmpty() || (nameResources.key(static_cast<QComboBox*> (m_inputs[tr("espacio")])->currentText())).isEmpty() || ((*it)["day"]).isEmpty() || ((*it)["beginhour"]).isEmpty() || ((*it)["endhour"]).isEmpty() || ((*it)["begindate"]).isEmpty() || ((*it)["enddate"]).isEmpty())
		{
			QMessageBox::information ( 0 , "ERROR", "Uno de los campos del formato\nes incorrecto", 0);
			isValid=false;
		}
		it++;
	}
	
	return isValid;
}
