/***************************************************************************
		*   Copyright (C) 2006 by Jorge Cuadrado   *
		*   kuadrosxx@gmail.com   *
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
#ifndef ADCANCELLATIONFORM_H
#define ADCANCELLATIONFORM_H

#include <adformbase.h>
#include <QWidget>
#include <QMap>
#include <QTextEdit>
#include <QLineEdit>
#include "adcancellation.h"
#include "global.h"
#include <QMessageBox>

/**
 * @author Hector Fabio Cruz Mosquera,0329876 <hecfacru@s4pc18>
 */
class ADCancellationForm : public ADFormBase
{
	Q_OBJECT
	public:
		ADCancellationForm( bool sem=true, QWidget *parent = 0);
		ADCancellationForm(ADCancellation *cancellation, bool sem=true ,QWidget *parent = 0);
		~ADCancellationForm();
		void setReadOnly(bool rOL);
		
	signals:
		void sendEvent( ADEvent *);	
		void requestClose();
		
	private slots:
		void valite();
		void modifyContend( Logic::Module, const QList< QVariant >&);
		
	private:
		void setup();
		QMap<QString, QWidget*> m_inputs;
		QTextEdit *areaTexto;
		ADCancellation *m_cancel;
		bool semestral;
};

#endif
