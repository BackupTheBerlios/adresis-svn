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

ADAudiovisualForm::ADAudiovisualForm(const ADAudioVisual & audiovisual, QWidget * parent) : ADFormBase("<h1><b>Audiovisuals</b><h1>", parent)
{
	D_FUNCINFO;
	setup();
	m_inserter = false;
	if(audiovisual.isValid())
	{
		static_cast<QLineEdit*>(m_inputs[tr("tipo")])->setText(audiovisual.type());
		static_cast<QLineEdit*>(m_inputs[tr("marca")])->setText(audiovisual.marksEquipment());
		estadoC->currentText();
		static_cast<QLineEdit*>(m_inputs[tr("numero de inventario")])->setText(audiovisual.numberInventory());
		static_cast<QLineEdit*>(m_inputs[tr("asignado al espacio")])->setText(audiovisual.codeSpace());
		
	}
}

ADAudiovisualForm::~ADAudiovisualForm()
{
}

void ADAudiovisualForm::setup()
{
	QWidget * base = new QWidget();
	QVBoxLayout *vBLayout = new QVBoxLayout(base);
	QGroupBox *container = new QGroupBox("Informacion");
	vBLayout->addWidget(container, Qt::AlignVCenter);
	
	QLabel *label;
	
	QGridLayout *layout = new QGridLayout;
	container->setLayout(layout);
	
	label = new QLabel("NOTA: SI ESTA AYUDA NO ESTA ASIGNADO A NINGUN ESPACIO, DEJE EN BLANCO EL CAMPO \"ASIGNADO AL ESPACIO\"");
	vBLayout->addWidget(label);	

	QLineEdit *edits;
	QStringList titles, titles2, estados;
	
	estados << tr("bueno") << tr("malo");
	estadoC = new QComboBox;
	estadoC->addItems(estados);
	
	titles << tr("Tipo") << tr("Marca") << tr("Estado") << tr("Numero de inventario") << tr("Asignado al Espacio");

	for(int i = 0; i < titles.count(); i++)
	{
		if(i==2)
		{
			layout->addWidget(new QLabel("Estado"),2,0);
			layout->addWidget(estadoC,2,1);
			m_inputs.insert(titles[i].toLower () , edits);
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
			static_cast<QLineEdit*>(m_inputs[tr("tipo")])->text(),
			static_cast<QLineEdit*>(m_inputs[tr("marca")])->text(),
			estadoC->currentText(),
			static_cast<QLineEdit*>(m_inputs[tr("numero de inventario")])->text(),
			spaceAsign
		);
	}
	else
	{
		emit requestUpdateAudiovisual(
			static_cast<QLineEdit*>(m_inputs[tr("tipo")])->text(),
			static_cast<QLineEdit*>(m_inputs[tr("marca")])->text(),
			estadoC->currentText(),
			static_cast<QLineEdit*>(m_inputs[tr("numero de inventario")])->text(),
			spaceAsign
		);
	}
	
}
