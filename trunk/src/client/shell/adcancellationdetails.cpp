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
#include "adcancellationdetails.h"
#include <QGridLayout>

ADCancellationDetails::ADCancellationDetails()
 : QDialog()
{
	setup();
}


ADCancellationDetails::~ADCancellationDetails()
{
}

void ADCancellationDetails::setup()
{
	QVBoxLayout *vBLayout = new QVBoxLayout;
	areaTexto= new QTextEdit;
	vBLayout->addWidget(areaTexto);
	
	okButton = new QPushButton("OK");
	connect( okButton, SIGNAL( clicked() ), this, SLOT( ok() ) );
	vBLayout->addWidget(okButton);
	
	setLayout(vBLayout);
	setModal(true);
}

void ADCancellationDetails::ok()
{
	if((areaTexto->document()->toPlainText()).isEmpty() )
	{
		QMessageBox::information ( 0 , "Message", "El motivo no deberia estar vacio", 0);
	}
	else
	{
		emit text( areaTexto->document()->toPlainText() );
		accept();
	}
}

