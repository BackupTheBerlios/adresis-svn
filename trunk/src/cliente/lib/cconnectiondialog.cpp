/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@gmail.com                                                      *
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

#include "cconnectiondialog.h"

#include <QLineEdit>
#include <QVBoxLayout>
#include <QIntValidator>

#include <dformfactory.h>
#include <dconfig.h>

CConnectionDialog::CConnectionDialog(QWidget *parent) : DTabDialog(parent)
{
	setWindowTitle(tr("Connect to server..."));
	setupUserPage();
	setupDBPage();
	
	loadSettings();
}


CConnectionDialog::~CConnectionDialog()
{
}

void CConnectionDialog::setupUserPage()
{
	QWidget *page = new QWidget;
	QVBoxLayout *layout = new QVBoxLayout(page);
	
	m_user = new QLineEdit;
	m_password = new QLineEdit;
	m_password->setEchoMode( QLineEdit::Password);
	
	layout->addLayout(DFormFactory::makeGrid( QStringList() << tr("User") << tr("Password"), QWidgetList() << m_user << m_password ));
	
	addTab( page, tr("User"));
}

void CConnectionDialog::setupDBPage()
{
	QWidget *page = new QWidget;
	QVBoxLayout *layout = new QVBoxLayout(page);
	
	m_database = new QLineEdit;
	m_server = new QLineEdit;
	m_port = new QLineEdit;
	m_port->setValidator( new QIntValidator(1, 65535, this));
	
	layout->addLayout(DFormFactory::makeGrid( QStringList() << tr("Database") << tr("Server") << tr("Port"), QWidgetList() << m_database << m_server << m_port ));
	
	addTab( page,tr("Server") );
	
}

void CConnectionDialog::loadSettings()
{
	DCONFIG->beginGroup("Connection");
	
	m_user->setText( DCONFIG->value("User").toString());
	m_password->setText(DCONFIG->value("Password").toString());
	m_server->setText(DCONFIG->value("Server", "localhost").toString());
	m_port->setText(DCONFIG->value("Port", 31337).toString());
	m_database->setText(DCONFIG->value("Database", "doms").toString());
}

QString CConnectionDialog::user() const
{
	return m_user->text();
}

QString CConnectionDialog::password() const
{
	return m_password->text();
}

QString CConnectionDialog::database() const
{
	return m_database->text();
}

QString CConnectionDialog::server() const
{
	return m_server->text();
}

int CConnectionDialog::port() const
{
	return m_port->text().toInt();
}

void CConnectionDialog::ok()
{
	DCONFIG->beginGroup("Connection");
	
	DCONFIG->setValue("User", user());
	DCONFIG->setValue("Password", password());
	DCONFIG->setValue("Server", server());
	DCONFIG->setValue("Port", port());
	DCONFIG->setValue("Database", database());
	
	DCONFIG->sync();
	
	accept();
}
