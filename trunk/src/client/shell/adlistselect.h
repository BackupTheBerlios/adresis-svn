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
#ifndef ADLISTSELECT_H
#define ADLISTSELECT_H

#include <QWidget>
#include <QMap>
#include <QListWidget>
#include <QToolButton>

/**
		@author Hector Fabio Cruz Mosquera,0329876
 */
		class ADListSelect : public QWidget
{
	Q_OBJECT
	public:
		ADListSelect(QWidget *parent=0);
		~ADListSelect();
		void addListToLeft(const QStringList &list);
		void addListToRight(const QStringList &list);
		QStringList takeList(const QString& listWidget);
		
		
	signals:
		void listChanged(const QString &lista, int pos);

	public slots:
		void addItemToLeft();
		void addItemToRight();

		
	private:
		QListWidget *listWidgetLeft;
		QListWidget *listWidgetRight;
		QToolButton *qTButtonLeft;
		QToolButton *qTButtonRight;
		QMap<QString, QWidget*> m_inputs;
		bool check(const QString& lista, const QListWidgetItem *item);
		
		
};
#endif
