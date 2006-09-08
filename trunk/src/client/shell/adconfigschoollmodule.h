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
#ifndef ADCONFIGSCHOOLLMODULE_H
#define ADCONFIGSCHOOLLMODULE_H

#include <QMap>
#include <QLabel>
#include <QDialog>
#include <QPushButton>


/**
	@author Hector Fabio Cruz Mosquera,0329876 <hectorcaz@gmail.com>
*/
class ADConfigSchoollModule : public QDialog
{
	Q_OBJECT
	public:
		ADConfigSchoollModule(QWidget *parent = 0);
		~ADConfigSchoollModule();
	
	private:
		void setup();
		void fillComboDay(QString combo, int monthNo);
		QMap<QString, QWidget*> m_inputs;
		QLabel *icono;
		int year;
		QPushButton *okButton,*cancelButton;
		
	private slots:
		void browse();
		void callMonthI(int monthNo);
		void callMonthF(int monthNo);
		void showImage(QString file);
		void ok();
		void cancel();
	
};

#endif
