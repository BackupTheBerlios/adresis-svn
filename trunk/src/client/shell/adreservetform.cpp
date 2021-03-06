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
#include "adreservetform.h"
#include <QLineEdit>
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>

#include <QGridLayout>
#include <QCheckBox>
#include <ddebug.h>
#include <QMessageBox>


ADReserveTForm::ADReserveTForm(QWidget *parent)
	: ADFormBase("<h1><b>Audiovisuales</b><h1>" , parent)
{
	m_inserter = true;
	setup();
}

ADReserveTForm::~ADReserveTForm()
{
}


ADReserveTForm::ADReserveTForm( ADReserve * reserve, QList<QString> infoResource, QList<ADReserve *> reservas, QWidget * parent)
{
	D_FUNCINFO;
	m_inserter = false;
	m_reserve = new ADReserve(reserve->idReserve(), reserve->typeReserve(), reserve->iduserreserve(), reserve->iduserresponsable(), reserve->idaudiovisual(), reserve->idspace(), reserve->day(), reserve->beginDateTime(), reserve->endDateTime(), reserve->isActive(), reserve->destinationreserve());
	setup();
	
	QString modulo, key;
	if( m_reserve->idspace() != "null" )
	{
		typeResourceC->setCurrentIndex(typeResourceC->findText("Espacios"));
		modulo = "space";
		key = m_reserve->idspace();
	}
	else
	{
		typeResourceC->setCurrentIndex(typeResourceC->findText("Audiovisual"));
		modulo = "audiovisual";
		key = m_reserve->idaudiovisual();
	}
	
	typeResourceC->setEnabled(false);
	resourceC->addItem(infoResource[0]);
	resourceC->setEnabled(false);
	resourcesNameC->addItem(infoResource[1]);
	resourcesNameC->setEnabled(false);
	
	loginC->addItem(m_reserve->iduserresponsable());
	loginC->setEnabled( false );
	
	m_userReserve = (m_reserve->iduserreserve());
	
	areaTexto-> setDocument( new QTextDocument(m_reserve->destinationreserve()) );
	areaTexto-> setReadOnly ( true );
	
	
	horario->assignTypeReserve("temporal", m_inserter);
	horario->receiveReserves( reservas );
	horario->modifyMonthReserve(reserve->beginDateTime().date().month()-1);
}


void ADReserveTForm::changeTypeResource(int opcion)
{
// 	opcion = 0 -> Espacios
//	opcion = 1 -> audiovisual
	resourceC->clear();
	ADEvent listTypes(ADEvent::Client, Logic::ReservesT, Logic::GetTypes , QVariant::fromValue(this));
	emit sendEvent( &listTypes );
	requestLogin();
}



void ADReserveTForm:: receiveEvent( ADEvent * e)
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
			QStringList id = ((((e->data()).toList())[1]).toMap()).keys();
			QList<QVariant> name = ((((e->data()).toList())[1]).toMap()).values();
			
			for(int i = 0; i < name.count(); i++)
			{
				dDebug() << id.at(i) <<"  "<< (name.at(i)).toString();
				nameResources.insert( id.at(i), ((name.at(i)).toString()));
			}
			
			insertListNameResources();
		}
		else if( (((e->data()).toList())[0]).toString() == "reservesResource" )
		{
			QList<ADReserve *> listReserves;
			foreach( QVariant variant, (((e->data()).toList())[1]).toList() )
			{
				ADReserve *reserve = qVariantValue<ADReserve *>(variant);
				listReserves << reserve;
			}
			receiveReserves(listReserves);
		}
		
		else if( (((e->data()).toList())[0]).toString() == "infoUser" )
		{
			m_userReserve = (((e->data()).toList())[1]).toString();
			loginC->addItem(m_userReserve);
		}
	}
	
}



void ADReserveTForm::insertListTypes()
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



void ADReserveTForm::insertListNameResources()
{
	QStringList resources = QStringList(nameResources.values());
	resources.insert(0,"Escoja uno de los Recursos");
	resourcesNameC->addItems( resources );
}


void ADReserveTForm::requestReserves( QString name)
{
	
}


void ADReserveTForm::changeResource( QString typeR)
{
	if( typeR !=( "ESCOJA UN RECURSO" ) )
	{
		QString modulo;
		if((typeResourceC->currentText()).toLower() ==("espacios"))
		{
			modulo = "space";
		}
		else
		{
			modulo = "audiovisual";
		}
		nameResources.clear();
		resourcesNameC->clear();
		
		ADEvent listName(ADEvent::Client, Logic::ReservesT, Logic::Info , QList<QVariant>() << QVariant("nameResources") << QVariant::fromValue(this) << QVariant(modulo) << QVariant(typeR));
		emit sendEvent( &listName );
		
	}
}


// Este metodo se encarga de tomar la informacion del comboBox de los nombres de los espacios, en el momento que es modificado, y manda a pedir todas las reservas que hayan sobre este Recurso
void ADReserveTForm::changeNameResource(const QString& name)
{
	QString resource;
	if( name !=( "Escoja uno de los Recursos" ) )
	{
		horario->clear();
		resource = nameResources.key(name);
		ADEvent listName(ADEvent::Client, Logic::ReservesT, Logic::Info , QList<QVariant>() << QVariant("reservesResource") << QVariant::fromValue(this) << QVariant(resource));
		emit sendEvent( &listName );
	}
}

// Cuando el metodo anterior haya mandado a pedir las reservas de un espacio, este metodo se encargara de recibirlos y luego reenviarselos a horario que es la tabla donde se muestra la informacion de los horarios, y luego ejecuta el metodo fill ( De la clase horario ) pasandole por parametro el tipo de reserva para llenar la tabla con las reservas de este tipo.
void ADReserveTForm::receiveReserves( const QList<ADReserve *>& results )
{
	horario->assignTypeReserve("temporal", m_inserter);
	horario->receiveReserves( results );
	horario->fill();
}



void ADReserveTForm::requestLogin()
{
	ADEvent infoU(ADEvent::Client, Logic::ReservesT, Logic::Info, QList<QVariant>() << QVariant("infoUser")<<QVariant::fromValue(this));
	emit sendEvent( &infoU );
}


void ADReserveTForm::setup()
{
	QWidget * base = new QWidget();
	QVBoxLayout *vBLayout = new QVBoxLayout(base);
	
	QGroupBox *container = new QGroupBox("Informacion");
	QGridLayout *layout = new QGridLayout;
	container->setLayout(layout);
	vBLayout->addWidget(container, Qt::AlignVCenter);
	
	horario = new ADSchedule();
	vBLayout->addWidget(horario);
	
	QStringList titles, tipoRec;

	tipoRec << tr("Espacios") << tr("Audiovisual");
	typeResourceC = new QComboBox;
	typeResourceC->addItems(tipoRec);
	connect(typeResourceC, SIGNAL(activated (int) ), this , SLOT(changeTypeResource ( int )));

	resourceC = new QComboBox;
	connect(resourceC, SIGNAL(activated (QString) ), this , SLOT(changeResource ( QString )));
	
	resourcesNameC = new QComboBox;
	connect(resourcesNameC, SIGNAL(activated (QString) ), this , SLOT(changeNameResource ( QString )));
	
	loginC = new QComboBox;
	loginC->setEnabled( false );
	
	horario->makeTable( false );
	titles << tr("Tipo Recurso") << tr("Recurso") << tr("Nombre Recurso")<< tr("Responsable") << tr("Motivo");

	layout->addWidget(new QLabel(titles[0]),0,0);
	layout->addWidget(typeResourceC,0,1);

	layout->addWidget(new QLabel(titles[1]),1,0);
	layout->addWidget(resourceC,1,1);
	
	layout->addWidget(new QLabel(titles[2]),2,0);
	layout->addWidget(resourcesNameC ,2,1);
	
	layout->addWidget(new QLabel(titles[3]),3,0);
	layout->addWidget(loginC, 3, 1);
	
	layout->addWidget(new QLabel(titles[4]),4,0);
	areaTexto = new QTextEdit;
	layout->addWidget(areaTexto,4,1);
	
	setForm(base);
	connect(this, SIGNAL(requestDone()),this, SLOT(emitEvent()));
}


void ADReserveTForm::emitEvent()
{
	listSchedules = horario->returnSchedule();
	QList<QMap<QString, QString> >::const_iterator it = listSchedules.begin();
	QString space, audiovisual;

	if(typeResourceC->currentText() == "Espacios")
	{
		space = nameResources.key(resourcesNameC->currentText());
		audiovisual = "null";
	}
	else
	{
		space = "null";
		audiovisual = nameResources.key(resourcesNameC->currentText());
	}
	
	if(valite())
	{
		while( it != listSchedules.end())
		{
			QDate di = (QDate::fromString((*it)["begindate"],"d/M/yyyy"));
			QDate df = (QDate::fromString((*it)["enddate"],"d/M/yyyy"));
			QTime hi = (QTime::fromString((*it)["beginhour"],"hh:mm"));
			QTime hf = (QTime::fromString((*it)["endhour"],"hh:mm"));
			QDateTime beginDateTime = QDateTime( di,hi );
			QDateTime endDateTime = QDateTime( df, hf ) ;
			
			
			if(m_inserter)
			{
				m_reserve = new ADReserve( 
					"",
					(*it)["typereserve"],
					m_userReserve,
					loginC-> currentText(),
					audiovisual,
					space,
					(*it)["day"],
					beginDateTime,
					endDateTime,
					true,
					(areaTexto->document())->toPlainText()
				);
				
				
				ADEvent insertReserve( ADEvent::Client, Logic::ReservesT, Logic::Add, QVariant::fromValue(m_reserve));
				emit sendEvent(&insertReserve);	
				clearFields();
			}
			
			
			
			else if(m_inserter == false && valite())
			{
				m_reserve = new ADReserve(
					m_reserve->idReserve(),
					m_reserve->typeReserve(),
					m_reserve->iduserreserve(),
					m_reserve->iduserresponsable(),
					m_reserve->idaudiovisual(),
					m_reserve->idspace(),
					(*it)["day"],
					beginDateTime,
					endDateTime,
					m_reserve->isActive(),
					m_reserve->destinationreserve());
					
				ADEvent insertReserve( ADEvent::Client, Logic::ReservesT, Logic::Update , QVariant::fromValue(m_reserve));
				emit sendEvent(&insertReserve);	
				emit requestClose();
			}
			it++;
		}
	}	
}



bool ADReserveTForm::valite()
{
	bool isValid = true;
	QList<QMap<QString, QString> >::const_iterator it = listSchedules.begin();
	if(listSchedules.count() == 0)
	{
		QMessageBox::information ( 0 , "ERROR", "No se ha seleccionado ningun horario", 0);
		isValid=false;
	}
	
	while( it != listSchedules.end())
	{
		if( (*it)["typereserve"].isEmpty() || m_userReserve.isEmpty() || (loginC-> currentText()).isEmpty() || ((*it)["day"]).isEmpty() || ((*it)["beginhour"]).isEmpty() || ((*it)["endhour"]).isEmpty() || (areaTexto->document())->toPlainText().isEmpty())
		{
			QMessageBox::information ( 0 , "ERROR", "Uno de los campos del formato\nes incorrecto", 0);
			isValid=false;
		}
		it++;
	}
	
	return isValid;
}

void ADReserveTForm::clearFields()
{
	typeResourceC->setCurrentIndex(0);
	resourceC->clear ();
	resourcesNameC->clear ();
	loginC->clear ();
	areaTexto->clear ();
	horario->clear();
}


void ADReserveTForm::modifyContend( Logic::Module, const QList< QVariant >&)
{
	
}
