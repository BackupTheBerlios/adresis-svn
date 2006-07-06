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

#include "adconnectorbase.h"

#include <QDataStream>
#include <ddebug.h>

#include "global.h"

ADConnectorBase::ADConnectorBase(QObject * parent) : QTcpSocket(parent)
{
	connect(this, SIGNAL(readyRead()), this, SLOT(readFromServer()));
	connect(this, SIGNAL(error ( QAbstractSocket::SocketError)), this, SLOT(handleError(QAbstractSocket::SocketError)));
	
	connect(this, SIGNAL(connected()), this, SLOT(flushQueue()));
}


ADConnectorBase::~ADConnectorBase()
{
}

void ADConnectorBase::sendToServer(const QString &text)
{
	dDebug() << "Sending: " << text;
	dDebug() << "state()" << state();

	dDebug() << "Estado" << QAbstractSocket::ConnectedState;

	if ( state() == QAbstractSocket::ConnectedState )
	{
		QTextStream out(this);
		
		dDebug() << "ESTOY EN EL IF";

		QString toSend(text);
		toSend.remove('\n');
		out << text+"%%" << endl;
		flush();
	}
	else
	{
		dDebug() << "ESTOY EN EL ELSE";
		m_queue << text;
	}
}

void ADConnectorBase::flushQueue()
{
	D_FUNCINFO;
	while ( m_queue.count() > 0 )
	{
		sendToServer( m_queue.takeFirst() );
	}
	m_queue.clear();
}
