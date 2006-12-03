/***************************************************************************
 *   Copyright (C) 2006 by Jorge Cuadrado   *
 *   kuadrosx@zi0n   *
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
#ifndef ADCANCELLATIONDETAILS_H
#define ADCANCELLATIONDETAILS_H

#include <QObject>
#include <qdialog.h>
#include <QPushButton>
#include <QWidget>
#include <QMessageBox>
#include <QTextEdit>
		
/**
	@author Hector Fabio Cruz Mosquera,0329876 <hecfacru@s4pc18>
*/
class ADCancellationDetails : public QDialog
{
	Q_OBJECT
	public:
		ADCancellationDetails();
		~ADCancellationDetails();
		
	signals:
		 void text(QString);
				
	private:
		void setup();
		QTextEdit *areaTexto;
		QPushButton *okButton;
				
	private slots:
		void ok();
		
};

#endif