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

#ifndef ADVIEWHTML_H
#define ADVIEWHTML_H

#include <QTextBrowser>
#include <QFrame>
#include <QGroupBox>
/**
	@author Jorge Cuadrado <kuadrosxx@gmail.com>
*/
class ADViewHtml : public QFrame
{
	Q_OBJECT
	public:
		ADViewHtml(QWidget *parent = 0);
		~ADViewHtml();
		
		void setHtml(const QString& html );
		
	private:
		QTextBrowser *m_view;
		QGroupBox *m_panel;
		
	public slots:
		void print();
		void exportToPdf();
	
	signals:
		void requestClose();
};

#endif
