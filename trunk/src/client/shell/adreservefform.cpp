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
	m_inserter = true;
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
}


void ADReserveFForm::changeTypeResource(int opcion)
{
// 	opcion = 0 -> Espacios
//	opcion = 1 -> audiovisual
	resourceC->clear();
	ADEvent listTypes(ADEvent::Client, Logic::ReservesF, Logic::GetTypes , QVariant::fromValue(this));
	emit sendEvent( &listTypes );
	requestLogin();
}


void ADReserveFForm:: receiveEvent( ADEvent * e)
{
	if(e->action() == Logic::GetTypes)
	{
		typeSpaces.clear();
		typeAudiovisual.clear();
		foreach(QVariant variant, ((e->data().toList())[0]).toList() )
		{
			typeSpaces << variant.toString();
		}
		
		foreach(QVariant variant, ((e->data().toList())[1]).toList() )
		{
			typeAudiovisual << variant.toString();
		}
		
		insertListTypes();
	}
	
	else if(e->action() == Logic::Info)
	{
		if( (((e->data()).toList())[0]).toString() == "nameResources")
		{
			dDebug() << "Tipo " << (e->data()).type();
			QStringList id = ((((e->data()).toList())[1]).toMap()).keys();
			QList<QVariant> name = ((((e->data()).toList())[1]).toMap()).values();
			
			dDebug() <<"QMAP IDS";
			for(int i = 0; i < name.count(); i++)
			{
				dDebug() << id.at(i) <<"  "<< (name.at(i)).toString();
				nameResources.insert( id.at(i), ((name.at(i)).toString()));
			}
			
			insertListNameResources();
		}
		else if( (((e->data()).toList())[0]).toString() == "reservesResource" )
		{
			dDebug() <<"Recibi  ReservesResource";
			QList<ADReserve *> listReserves;
			foreach( QVariant variant, (((e->data()).toList())[1]).toList() )
			{
				ADReserve *reserve = qVariantValue<ADReserve *>(variant);
				listReserves << reserve;
			}
			dDebug() << "Tamaño de la lista de reservas " << listReserves.count();
			receiveReserves(listReserves);
		}
		
		else if( (((e->data()).toList())[0]).toString() == "infoUser" )
		{
			dDebug() <<"Recibi  InfoUser";
			m_responsable = (((e->data()).toList())[1]).toString();
			
		}
	}
	
	else if(e->action() == Logic::Dates)
	{
		m_dateSemestral.insert("dateI", ((e->data()).toList())[0].toString() );
		m_dateSemestral.insert("dateF", ((e->data()).toList())[1].toString() );
		dDebug() << "FECHAS => " << ((e->data()).toList())[0].toString() << "  " << ((e->data()).toList())[1].toString();
	}
}


void ADReserveFForm::insertListTypes()
{
	int opcion = typeResourceC->currentIndex();
	resourceC->insertItem(0,"ESCOJA UN RECURSO");
	
	if(opcion == 0)
	{
		resourceC->insertItems(1,typeSpaces);
	}
	else
	{
		resourceC->insertItems(1,typeAudiovisual);
	}
}



void ADReserveFForm::insertListNameResources()
{
	QStringList resources = QStringList(nameResources.values());
	resources.insert(0,"Escoja uno de los Recursos");
	resourcesNameC->addItems( resources );
}



void ADReserveFForm::requestDatesSemestral()
{
	ADEvent dates(ADEvent::Client, Logic::ReservesF, Logic::Info , QList<QVariant>() << QVariant("datesSemestral") << QVariant::fromValue(this));
	emit sendEvent( &dates );
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
// 		resourceC->currentText(),
// 		static_cast<QLineEdit*>(m_inputs[tr("responsable")])->text(),
// 		static_cast<QLineEdit*>(m_inputs[tr("login")])->text()
		
// 	}
}

void ADReserveFForm::changeResource( QString typeR)
{
	QString modulo;
	requestDatesSemestral();
	if((typeResourceC->currentText()).toLower() ==("espacios"))
	{
		modulo = "space";
	}
	else
	{
		modulo = "audiovisual";
	}
		
	if( typeR !=( "ESCOJA UN RECURSO" ) )
	{
		nameResources.clear();
		resourcesNameC->clear();
		
		ADEvent listName(ADEvent::Client, Logic::ReservesF, Logic::Info , QList<QVariant>() << QVariant("nameResources") << QVariant::fromValue(this) << QVariant(modulo) << QVariant(typeR));
		emit sendEvent( &listName );
	}
}


// Este metodo se encarga de tomar la informacion del comboBox de los nombres de los espacios, en el momento que es modificado, y manda a pedir todas las reservas que hayan sobre este Recurso
void ADReserveFForm::changeNameResource(const QString& name)
{
	QString resource;
	dDebug() << "CHANGENAMERESOURCE ===> " << name;
	if( name !=( "Escoja uno de los Recursos" ) )
	{
		horario->clear();
		resource = nameResources.key(name);
		ADEvent listName(ADEvent::Client, Logic::ReservesF, Logic::Info , QList<QVariant>() << QVariant("reservesResource") << QVariant::fromValue(this) << QVariant(resource));
		emit sendEvent( &listName );
		
	}
}

// Cuando el metodo anterior haya mandado a pedir las reservas de un espacio, este metodo se encargara de recibirlos y luego reenviarselos a horario que es la tabla donde se muestra la informacion de los horarios, y luego ejecuta el metodo fill ( De la clase horario ) pasandole por parametro el tipo de reserva para llenar la tabla con las reservas de este tipo.
void ADReserveFForm::receiveReserves( const QList<ADReserve *>& results )
{
	dDebug() << "receiveReserves " <<  resourcesNameC->currentText();
	horario->assignTypeReserve("semestral");
	horario->receiveReserves( results );
	horario->fill();
}



void ADReserveFForm::requestLogin()
{
	dDebug() << "ADRESERVEFORM REQUESTLOGIN";
	ADEvent infoU(ADEvent::Client, Logic::ReservesF, Logic::Info, QList<QVariant>() << QVariant("infoUser")<<QVariant::fromValue(this));
	emit sendEvent( &infoU );
	dDebug() << "ADRESERVEFORM REQUESTLOGIN";

}


void ADReserveFForm::setup()
{
	QWidget * base = new QWidget();
	QVBoxLayout *vBLayout = new QVBoxLayout(base);
	
	QGroupBox *container = new QGroupBox("Informacion");
	QGridLayout *layout = new QGridLayout;
	container->setLayout(layout);
	vBLayout->addWidget(container, Qt::AlignVCenter);
	
	horario = new ADSchedule();
	vBLayout->addWidget(horario);

	QLineEdit *edits;
	QStringList titles, tipoRec;

	tipoRec << tr("Espacios") << tr("Audiovisual");
	typeResourceC = new QComboBox;
	typeResourceC->addItems(tipoRec);
	connect(typeResourceC, SIGNAL(activated (int) ), this , SLOT(changeTypeResource ( int )));

	resourceC = new QComboBox;
	connect(resourceC, SIGNAL(activated (QString) ), this , SLOT(changeResource ( QString )));
	
	resourcesNameC = new QComboBox;
	connect(resourcesNameC, SIGNAL(activated (QString) ), this , SLOT(changeNameResource ( QString )));

	horario->makeTable( true );
	titles << tr("Tipo Recurso") << tr("Recurso") << tr("Nombre Recurso")<< tr("Responsable") << tr("Motivo");

	layout->addWidget(new QLabel(titles[0]),0,0);
	layout->addWidget(typeResourceC,0,1);
	m_inputs.insert(titles[0].toLower () , typeResourceC);

	layout->addWidget(new QLabel(titles[1]),1,0);
	layout->addWidget(resourceC,1,1);
	m_inputs.insert(titles[1].toLower () , resourceC);
	
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
	m_inputs.insert( titles[4].toLower () , areaTexto );
	
	setForm(base);
	connect(this, SIGNAL(requestDone()),this, SLOT(emitInsertReserve()));
}


void ADReserveFForm::emitInsertReserve()
{
	dDebug() << "EMITINSERTRESERVE===EMITINSERTRESERVE";
	
	listSchedules = horario->returnSchedule();
	QList<QMap<QString, QString> >::const_iterator it = listSchedules.begin();
	QString space, audiovisual;



	if(m_inserter && valite())
	{
		QDateTime beginDateTime = QDateTime(QDate::fromString(m_dateSemestral.value("dateI") ,"yyyy-MM-dd"), QTime::fromString((*it)["beginhour"],"hh:mm")) ;
		QDateTime endDateTime = QDateTime(QDate::fromString(m_dateSemestral.value("dateF") ,"yyyy-MM-dd"), QTime::fromString((*it)["endhour"],"hh:mm")) ;
		
		dDebug() << "FECHAS SON " << beginDateTime.toString("yyyy/MM/dd hh:mm") << " "<<endDateTime.toString("yyyy/MM/dd hh:mm");
		if(typeResourceC->currentText() == "Espacios")
		{
			space = nameResources.key(resourcesNameC->currentText());
			audiovisual = "";
		}
		else
		{
			space = "";
			audiovisual = nameResources.key(resourcesNameC->currentText());
		}
		
		
		while( it != listSchedules.end())
		{
			ADReserve reserve(
				"",
				(*it)["typereserve"],
				m_responsable,
				static_cast<QLineEdit*>(m_inputs[tr("responsable")])->text(),
				audiovisual,
				space,
				(*it)["day"],
				beginDateTime,
				endDateTime,
				true,
				(static_cast<QTextEdit *>(m_inputs[tr("motivo")])->document())->toPlainText()
			);
			dDebug() << reserve.idReserve() << " " << reserve.typeReserve() << " " << reserve.iduserreserve() << " " << reserve.iduserresponsable() << " " << reserve.idaudiovisual() << " " << reserve.idspace() << " " << reserve.day() << " " << reserve.isActive() << " " << reserve.destinationreserve();
			it++;
			
			ADEvent insertReserve( ADEvent::Client, Logic::ReservesF, Logic::Add, QVariant::fromValue(&reserve));
			dDebug() << "YA CREE EL EVENTO DE INSERCION DE RESERVAS";
			emit sendEvent(&insertReserve);	
			dDebug() << "YA ENVIE EL EVENTO DE INSERCION DE RESERVAS";
		}
	}
// // 	else
// // 	{
// // 		emit requestUpdateReserve(
// // 			tipoResC->currentText(),
// // 			resourceC->currentText(),
// // 			static_cast<QLineEdit*>(m_inputs[tr("responsable")])->text()
// // 		);
// // 	}
	
}



bool ADReserveFForm::valite()
{
	bool isValid = true;
	QList<QMap<QString, QString> >::const_iterator it = listSchedules.begin();
	while( it != listSchedules.end())
	{
		if( (*it)["typereserve"].isEmpty() || m_responsable.isEmpty() || (static_cast<QLineEdit*> (m_inputs[tr("responsable")])->text()).isEmpty() || (nameResources.key(static_cast<QComboBox*> (m_inputs[tr("nombre recurso")])->currentText())).isEmpty() || ((*it)["day"]).isEmpty() || ((*it)["beginhour"]).isEmpty() || ((*it)["endhour"]).isEmpty())
		{
			QMessageBox::information ( 0 , "ERROR", "Uno de los campos del formato\nes incorrecto", 0);
			isValid=false;
		}
		it++;
	}
	
	return isValid;
}
