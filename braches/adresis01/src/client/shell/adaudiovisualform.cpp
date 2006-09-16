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
#include "adaudiovisualform.h"

#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>

#include <QGridLayout>
#include <QCheckBox>
#include <ddebug.h>


ADAudiovisualForm::ADAudiovisualForm(QWidget *parent)
	: ADFormBase("<h1><b>Audiovisuals</b><h1>" , parent)
{
	m_inserter = true;
	setup();
}


ADAudiovisualForm::~ADAudiovisualForm()
{
}


// Este metodo  lo cambie debido a que los combobox no estaban tomando los datos reales, en especial el combobox tiposC que es el que me determina que tipo de ayuda es. Entonces de ahora en adelante lo que hago es en admainwindow creo un adspaceform cun una ayudaaudiovisual esa ayuda que me entra como parametro la guardo aqui en adAudiovisual y luego, cuando ya tenga los tipos de ayuda lo que hago es llenar los campos. con la ayuda que tengo actualmente guardada. 
// Esto me toco hacerlo debido a que cuando yo hacia la peticion de la lista de tipos de ayudas siempre me creaba primero la forma y despues me llegaban los tipos, entonces dado esto ahora mas bien lo que hago es cuando ya me hallan llegado la lista de tipos de ayudas ahi si mando a rellenar los campos.

ADAudiovisualForm::ADAudiovisualForm(const ADAudioVisual & audiovisual, QWidget * parent)
{
	D_FUNCINFO;
	setup();
	m_inserter = false;
	adAudiovisual = new ADAudioVisual(audiovisual.type(), audiovisual.marksEquipment(), audiovisual.estate(), audiovisual.numberInventory(), audiovisual.codeSpace());
}



void ADAudiovisualForm::insertListTypes(const QList<XMLResults>& results)
{
	QList<XMLResults>::const_iterator it= results.begin();
	
	while( it != results.end() )
	{
		tipos << ((*it)["type"]);
		++it;
	}

	tiposC->addItems(tipos);

	if(!m_inserter)
	{
		fill();
	}
}


void ADAudiovisualForm::fill()
{
	if(adAudiovisual->isValid())
	{
		tiposC->setCurrentIndex(tiposC->findText(adAudiovisual->type())); 
		static_cast<QLineEdit*>(m_inputs[tr("marca")])->setText(adAudiovisual->marksEquipment());
		estadoC->setCurrentIndex(estadoC->findText(adAudiovisual->estate()));
		static_cast<QLineEdit*>(m_inputs[tr("numero de inventario")])->setText(adAudiovisual->numberInventory());
		static_cast<QLineEdit*>(m_inputs[tr("asignado al espacio")])->setText(adAudiovisual->codeSpace());
		
	}
}


void ADAudiovisualForm::setup()
{
	QWidget * base = new QWidget();
	QVBoxLayout *vBLayout = new QVBoxLayout(base);
	QGroupBox *container = new QGroupBox("Informacion");
	vBLayout->addWidget(container, Qt::AlignVCenter);
	QLabel *label;
	QLineEdit *edits;
	QStringList titles, titles2, estados;
	QGridLayout *layout = new QGridLayout;


	container->setLayout(layout);
	
	label = new QLabel("NOTA: SI ESTA AYUDA NO ESTA ASIGNADO A NINGUN ESPACIO, DEJE EN BLANCO EL CAMPO \"ASIGNADO AL ESPACIO\"");

	vBLayout->addWidget(label);	

// 	addType = new QPushButton("Modifcar tipos de ayudas");
// 	vBLayout->addWidget(addType);

	tiposC = new QComboBox;
	
	estados << tr("bueno") << tr("malo");
	estadoC = new QComboBox;
	estadoC->addItems(estados);
	
	titles << tr("Tipo") << tr("Marca") << tr("Estado") << tr("Numero de inventario") << tr("Asignado al Espacio");

	for(int i = 0; i < titles.count(); i++)
	{
		if(i==0)
		{
			layout->addWidget(new QLabel("Tipo"),0,0);
			layout->addWidget(tiposC,0,1);
		}
		
		else if(i==2)
		{
			layout->addWidget(new QLabel("Estado"),2,0);
			layout->addWidget(estadoC,2,1);
		}
		else
		{
			label = new QLabel(titles[i] );
			layout->addWidget(label, i, 0);
			edits = new QLineEdit();
			layout->addWidget(edits, i, 1);
			m_inputs.insert(titles[i].toLower () , edits);
		}
	}

	setForm(base);
	connect(this, SIGNAL(requestDone()),this, SLOT(emitInsertAudiovisual()));
}



void ADAudiovisualForm::emitInsertAudiovisual()
{

	QString spaceAsign; // Este es el espacio al cual esta asignado. 
	spaceAsign = static_cast<QLineEdit*>(m_inputs[tr("asignado al espacio")])->text();

	// Coloco este condicional por si se da el caso en que la ayuda no este asignada a ningun espacio en tal caso se debe de colocar en la base de datos null. lo que signifca que no esta asignado a ningun espacio.
	if(spaceAsign.isEmpty())
	{
		spaceAsign = "null";
	}



	if(m_inserter)
	{
	
		emit requestInsertAudiovisual(
			tiposC->currentText(),
			static_cast<QLineEdit*>(m_inputs[tr("marca")])->text(),
			estadoC->currentText(),
			static_cast<QLineEdit*>(m_inputs[tr("numero de inventario")])->text(),
			spaceAsign
		);
	}
	else
	{
		emit requestUpdateAudiovisual(
			tiposC->currentText(),
			static_cast<QLineEdit*>(m_inputs[tr("marca")])->text(),
			estadoC->currentText(),
			static_cast<QLineEdit*>(m_inputs[tr("numero de inventario")])->text(),
			spaceAsign
		);
	}
	
}

