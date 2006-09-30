/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
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

#ifndef CONFIGWIZARD_H
#define CONFIGWIZARD_H

#include <QString>

#include "dwizard.h"

class CWWelcomePage;
class CWSecondPage;

class QLineEdit;

/**
 * @brief la clase ADCFirstRunDialog provee el dialogo para configurar los directorios principales de la aplicacion.
 * El home que es el directorio donde esta instalada la aplicacion y el repositorio es usado para multiples propositos
 * @author David Cuadrado
 */
class ADCFirstRunDialog : public DWizard
{
	Q_OBJECT
	public:
    		/**
	 	 * Crea el dialogo configuracion.
		 */
		ADCFirstRunDialog();
    		/**
		 * destuctor
		 */
		~ADCFirstRunDialog();
		
		/**
		 * Pone los directorios por defecto en el dialogo.
		 * @param home 
		 * @param repos 
		 */
		void setInitialData(const QString &home, const QString &repos);
		
		/**
		 * @return la ruta del directorio home
		 */
		QString home();
		
		/**
		 * @return la ruta del directorio repository
		 */
		QString repository();
		
		CWWelcomePage *welcomePage();

	private:
		CWWelcomePage *m_welcomePage;
		CWSecondPage *m_secondPage;
		
		QString kthome;
		QString ktrepos;
};

class CWWelcomePage : public DWizardPage
{
	Q_OBJECT
	public:
		CWWelcomePage(QWidget *parent = 0);
		~CWWelcomePage();
		
		void setMessage(const QString &msg);
		
		bool isComplete() const { return true; } ;
		void reset() { };
		
	private:
		QLabel *m_message;
};

class CWSecondPage : public DWizardPage
{
	Q_OBJECT
	public:
		CWSecondPage(QWidget *parent = 0);
		~CWSecondPage();
		void setData(const QString &home, const QString &repos);
		QString home();
		QString repository();
		
		bool isComplete()const;
		void reset();
		
	private slots:
		void verify(const QString &);
		
	private:
		QLineEdit *m_kthome;
		QLineEdit *m_ktrepos;
};

#endif

