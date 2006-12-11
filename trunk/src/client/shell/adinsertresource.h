/***************************************************************************
 *   Copyright (C) 2006 by Hector Fabio                                    *
 *   hectorcaz@gmail.com                                                   *
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
#ifndef ADINSERTRESOURCE_H
#define ADINSERTRESOURCE_H

#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QStringList>
#include <QLineEdit>
#include "global.h"

/**
	@author Hector Fabio Cruz Mosquera,0329876 <hectorcaz@gmail.com>
*/
class ADInsertResource : public QDialog
{
	Q_OBJECT
	public:
		ADInsertResource(QWidget *parent=0);
		~ADInsertResource();

	private:
		QComboBox *typeResource;
		QComboBox *resource;
		QLineEdit *edit;
		QPushButton *addButton, *delButton, *okButton, *cancelButton;
		QStringList recursosEsp, recursosAud;
		bool valite();
		int list;
	
	private slots:
		void ok();
		void cancel();
		void changeTypeResource(int opcion);
		void insertResource();
		void deleteResource();
		void changeNameResource(const QString& resource);
		
	
	public slots:
		void insertListTypes(const QList<XMLResults>& results);
		
	signals:
		void addDelResource(const QString &opcion, const QString &table, const QString &resource);
};

#endif
