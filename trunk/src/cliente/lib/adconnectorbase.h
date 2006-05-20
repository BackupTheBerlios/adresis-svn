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

#ifndef CCONNECTORBASE_H
#define CCONNECTORBASE_H

#include <qtcpsocket.h>
#include <QStringList>
#include <QXmlSimpleReader>

#include "global.h"

class ADPackageParser;

/**
 * Maneja las conexiones al servidor, asi mismo tambien maneja los errores de conexion
 * @author David Cuadrado <krawek@gmail.com>
*/
class ADConnectorBase : public QTcpSocket
{
	Q_OBJECT;
	public:
		ADConnectorBase(QObject * parent = 0);
		~ADConnectorBase();
		virtual void login(const QString &user, const QString &passwd) = 0;
		
	public slots:
		virtual void sendToServer(const QString &text);
		
	protected slots:
		virtual void readFromServer() = 0;
		virtual void handleError(QAbstractSocket::SocketError error) = 0;
		
		void flushQueue();
		
	private:
		QStringList m_queue;
};

#endif
