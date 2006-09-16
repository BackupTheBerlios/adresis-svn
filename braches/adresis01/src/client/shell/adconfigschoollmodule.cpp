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
#include "adconfigschoollmodule.h"
#include <QPixmap>
#include <QIcon>
#include <QComboBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QDate>
#include <QFileDialog>
#include <ddebug.h>
#include <QGroupBox>
#include <QMessageBox>
#include <dconfig.h>
#include "global.h"


ADConfigSchoollModule::ADConfigSchoollModule(QWidget *parent)
 : QDialog(parent)
{
	setup();
}


ADConfigSchoollModule::~ADConfigSchoollModule()
{
}


void ADConfigSchoollModule::setup()
{
	DCONFIG->beginGroup("ConfigurationOfSchooll");
	setWindowTitle("Configure Schooll");
	QVBoxLayout *vBLayout = new QVBoxLayout;
	QGroupBox *container = new QGroupBox("Configurar Escuela");
	vBLayout->addWidget(container, Qt::AlignVCenter);
	
	QGridLayout *layout = new QGridLayout;
	container->setLayout(layout);
	
	QHBoxLayout *layoutH;
	
	QComboBox *comboBox;
	
	QPushButton *browse = new QPushButton("Browse");
	connect(browse, SIGNAL(clicked()), this, SLOT(browse()));
	
	QStringList titles;
	titles << "Nombre De La Escuela" << "Logo De La Escuela" << "Fecha Inicio Semestre Actual" << "Fecha Finalizacion Semestre Actual";
	
	QLineEdit *edits;
	
	layout->addWidget(new QLabel(titles[0]), 0,0);
	edits = new QLineEdit();
	edits->setText(qvariant_cast<QString>(DCONFIG->value("NameSchooll")));
	layout->addWidget(edits, 0,1);
	m_inputs.insert("name", edits);
	
	layoutH = new QHBoxLayout();
	edits = new QLineEdit();
	edits->setText(qvariant_cast<QString>(DCONFIG->value("LogoPath")));
	edits->setReadOnly(true);
	m_inputs.insert("logo",edits);
	layoutH->addWidget(edits);
	layoutH->addWidget(browse);
	icono = new QLabel();
	icono->setMinimumSize(64,64);
	icono->setMaximumSize(64,64);
	icono->setAlignment(Qt::AlignCenter);
	icono->setFrameShape(QFrame::Box);
	showImage( qvariant_cast<QString>(DCONFIG->value("LogoPath")));
	layoutH->addWidget(icono);
	layout->addWidget(new QLabel(titles[1]), 1,0);
	layout->addLayout(layoutH,1,1);
	
	year = (QDate::currentDate()).year();
	layoutH = new QHBoxLayout();
	
	comboBox = new QComboBox();
	m_inputs.insert("dayi",comboBox);
	fillComboDay("dayi", 0);
	comboBox->setCurrentIndex(comboBox->findText(qvariant_cast<QString>(DCONFIG->value("dateIniSem")).section("/",0,0)));
	layoutH->addWidget(comboBox);
	
	comboBox = new QComboBox();
	for(int month = 1; month <= 12; ++month)
	{
		comboBox->addItem( QDate::longMonthName(month) );
	}
	comboBox->setCurrentIndex( (qvariant_cast<QString>(DCONFIG->value("dateIniSem"))).section("/",1,1).toInt() );
	connect( comboBox, SIGNAL( activated( int )), this, SLOT( callMonthI( int )));
	m_inputs.insert("monthi",comboBox);
	layoutH->addWidget(comboBox);
	
	comboBox = new QComboBox();
	comboBox->setEditable(false);
	comboBox->addItem( QString::number(year));
	m_inputs.insert("yeari",comboBox);
	layoutH->addWidget(comboBox);
		
	layout->addWidget(new QLabel(titles[2]), 2,0);
	layout->addLayout(layoutH,2,1);
	
	
	layoutH = new QHBoxLayout();
	
	comboBox = new QComboBox();
	m_inputs.insert("dayf",comboBox);
	fillComboDay("dayf", 0);
	dDebug() << (qvariant_cast<QString>(DCONFIG->value("dateFinSem" )).section("/",0,0));
	comboBox->setCurrentIndex(comboBox->findText(qvariant_cast<QString>(DCONFIG->value("dateFinSem")).section("/",0,0)));
	layoutH->addWidget(comboBox);
	
	comboBox = new QComboBox();
	for (int month = 1; month <= 12; ++month)
	{
            comboBox->addItem(QDate::longMonthName(month));
	}
	dDebug() << (qvariant_cast<QString>(DCONFIG->value("dateFinSem"))).section("/",1,1).toInt();
	comboBox->setCurrentIndex((qvariant_cast<QString>(DCONFIG->value("dateFinSem"))).section("/",1,1).toInt());
	connect( comboBox, SIGNAL( activated( int )), this, SLOT( callMonthF( int )));
	m_inputs.insert("monthf",comboBox);
	layoutH->addWidget(comboBox);
	
	comboBox = new QComboBox();
	comboBox->setEditable(false);
	comboBox->addItem( QString::number(year) );
	m_inputs.insert("yearf",comboBox);
	layoutH->addWidget(comboBox);
		
	layout->addWidget(new QLabel(titles[3]), 3,0);
	layout->addLayout(layoutH,3,1);
	
	layoutH = new QHBoxLayout();
	okButton = new QPushButton("OK");
	connect( okButton, SIGNAL( clicked() ), this, SLOT( ok() ) );
	cancelButton = new QPushButton("CANCEL");
	connect( cancelButton, SIGNAL( clicked() ), this, SLOT( cancel() ) );
	layoutH->addWidget(okButton);
	layoutH->addWidget(cancelButton);
	layout->addLayout(layoutH,4,1);
	setLayout(vBLayout);
	setModal(true);
}

void ADConfigSchoollModule::callMonthI(int monthNo)
{
	fillComboDay("dayi", monthNo);
}

void ADConfigSchoollModule::callMonthF(int monthNo)
{
	fillComboDay("dayf", monthNo);
}


void ADConfigSchoollModule::fillComboDay( QString comboMonth, int monthNo )
{
	QStringList days;
	int numCols;
	
	if(monthNo == 0 || monthNo == 2 || monthNo == 4 || monthNo == 6 || monthNo == 7 || monthNo == 9 || monthNo == 11 )
	{
		numCols=31;
	}
	else if(monthNo == 3 || monthNo == 5 || monthNo == 8 || monthNo == 10)
	{
		numCols=30;
	}
	else if( monthNo == 1 && QDate::isLeapYear( year ) ) //Cond. de años bisiestos
	{
		numCols = 29;
	}
	else
	{
		numCols=28;
	}

	for(int n=1; n <= numCols; n++)
	{
		days << QString::number(n);	
	}
	
	static_cast<QComboBox*>(m_inputs[comboMonth])->clear();
	static_cast<QComboBox*>(m_inputs[comboMonth])->addItems(days);
}

void ADConfigSchoollModule::browse()
{
	QString file = QFileDialog::getOpenFileName(this, tr("Select icon"), QDir::currentPath());
		
	if (!file.isEmpty())
	{
		static_cast<QLineEdit*>(m_inputs[("logo")])->setText(file);
		showImage(file);
	}
}


void ADConfigSchoollModule::showImage( QString file )
{
	QIcon icon = QIcon(file);
	QPixmap pixmap = icon.pixmap(64,64);
	icono->setPixmap(pixmap);
}


void ADConfigSchoollModule::ok()
{
	bool isCorrect= false;
	
	QString dayI = static_cast<QComboBox*>(m_inputs[tr("dayi")])->currentText();
	int monthI = static_cast<QComboBox*>(m_inputs[tr("monthi")])->currentIndex();
	QString yearI = static_cast<QComboBox*>(m_inputs[tr("yeari")])->currentText();
	QString dateIni = ""+ dayI + "/" + QString::number(monthI) + "/" + yearI;
	
	QString dayF = static_cast<QComboBox*>(m_inputs[tr("dayf")])->currentText();
	int monthF = static_cast<QComboBox*>(m_inputs[tr("monthf")])->currentIndex();
	QString yearF = static_cast<QComboBox*>(m_inputs[tr("yearf")])->currentText();
	QString dateFin = ""+ dayF + "/" + QString::number(monthF) + "/" + yearF;
	
	if( (monthI == monthF && dayI.toInt() < dayF.toInt()) || monthI < monthF )
	{
		isCorrect=true;
	}
	
	
	
	if(isCorrect)
	{
		DCONFIG->beginGroup("ConfigurationOfSchooll");
		DCONFIG->setValue("NameSchooll",  static_cast<QLineEdit*>(m_inputs[tr("name")])->text());
		DCONFIG->setValue("LogoPath",  static_cast<QLineEdit*>(m_inputs[tr("logo")])->text());
		DCONFIG->setValue("dateIniSem",  dateIni);
		DCONFIG->setValue("dateFinSem",  dateFin);
		DCONFIG->sync();
		accept();
	}
	else
	{
		QMessageBox::information ( 0 , "Message", "La fecha de inicio debe ser menor a la\nde finalizacion de semestre", 0);
	}
}

void ADConfigSchoollModule::cancel()
{
	reject();
}
