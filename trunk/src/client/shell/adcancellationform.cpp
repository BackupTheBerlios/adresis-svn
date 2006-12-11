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
#include "adcancellationform.h"
#include <QGridLayout> 


ADCancellationForm::ADCancellationForm(QWidget * parent)
	:ADFormBase("<h1><b>Cancellations</b><h1>" , parent)

{
	setWindowTitle("Ingrese el motivo de la cancelaci�");
	setup();
}

ADCancellationForm::ADCancellationForm(ADCancellation * cancellation, QWidget * parent)
{
	m_cancel = cancellation;
	setup();
	static_cast<QLineEdit*>(m_inputs[tr("id cancelacion")])->setText( cancellation->idReserveCancellation() );
	static_cast<QLineEdit*>(m_inputs[tr("usuario")])->setText( cancellation->idUserCancellation() );
	static_cast<QLineEdit*>(m_inputs[tr("hora cancelacion")])->setText( cancellation->dateTimeCancellation().time().toString("hh:mm") );
	static_cast<QLineEdit*>(m_inputs[tr("fecha cancelacion")])->setText( cancellation->dateTimeCancellation().date().toString("dd/MM/yyyy") );
	areaTexto-> setDocument( new QTextDocument(cancellation->razonCancellation()) );
}


ADCancellationForm::~ADCancellationForm()
{
}

void ADCancellationForm::setReadOnly( bool rOL)
{
	areaTexto-> setReadOnly ( rOL );
}

void ADCancellationForm::setup()
{
	QWidget * base = new QWidget();
	QVBoxLayout *vBLayout = new QVBoxLayout(base);
	
	QGroupBox *container = new QGroupBox("Cancelacion de Reserva");
	QGridLayout *layout = new QGridLayout;
	container->setLayout(layout);
	vBLayout->addWidget(container, Qt::AlignVCenter);
	
	QLineEdit *edits;
	QStringList titles;

	titles << tr("Id Cancelacion") << tr("Usuario") << tr("Hora Cancelacion")<< tr("Fecha Cancelacion") << tr("Motivo Cancelacion");

	layout->addWidget(new QLabel(titles[0]),0,0);
	edits = new QLineEdit;
	edits->setReadOnly(true);
	layout->addWidget(edits,0,1);
	m_inputs.insert(titles[0].toLower() , edits);

	layout->addWidget(new QLabel(titles[1]),1,0);
	edits = new QLineEdit;
	edits->setReadOnly(true);
	layout->addWidget(edits,1,1);
	m_inputs.insert(titles[1].toLower() , edits);
	
	layout->addWidget(new QLabel(titles[2]),2,0);
	edits = new QLineEdit;
	edits->setReadOnly(true);
	layout->addWidget(edits,2,1);
	m_inputs.insert(titles[2].toLower() , edits);
	
	layout->addWidget(new QLabel(titles[3]),3,0);
	edits = new QLineEdit;
	edits->setReadOnly(true);
	layout->addWidget(edits,3,1);
	m_inputs.insert(titles[3].toLower() , edits);
	
	layout->addWidget(new QLabel(titles[4]),4,0);
	areaTexto = new QTextEdit;
	areaTexto-> setReadOnly ( true );
	layout->addWidget(areaTexto,4,1);
	
	
	setForm(base);
	connect(this, SIGNAL(requestDone()),this, SLOT(valite()));
	connect(this, SIGNAL(requestClose()), this, SLOT(close()));
}

void ADCancellationForm::valite()
{
	if((areaTexto->document()->toPlainText()).isEmpty() )
	{
		QMessageBox::information ( 0 , "Message", "El motivo de cancelacion no deberia estar vacio", 0);
	}
	else
	{
		m_cancel->setRazonCancellation( areaTexto->document()->toPlainText() );
		ADEvent event(ADEvent::Client, Logic::ReservesF, Logic::Del, QVariant::fromValue(m_cancel) );
		emit sendEvent( &event );
		close();
	}
}