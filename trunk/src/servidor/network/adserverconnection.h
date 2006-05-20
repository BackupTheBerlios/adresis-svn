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

#ifndef ADSERVERCONNECTION_H
#define ADSERVERCONNECTION_H

#include <QThread>
#include <QTcpSocket>
#include <QDomDocument>

#include "adserverclient.h"
#include "adspackageparser.h"

#include "adquery.h"

/**
 * Esta clase representa cada conexion de un cliente al servidor, es un hilo.
 * @author David Cuadrado <krawek@gmail.com>
 */
class ADServerConnection : public QThread
{
	Q_OBJECT;

	public:
		ADServerConnection(int socketDescriptor, QObject *parent);
		~ADServerConnection();
		void run();
		
		void close();
		void setLogin(const QString &login);
		
		bool isLogged() const;
		
	private:
		
	public slots:
		void sendToClient(const QString &msg);
		void sendToClient(const QDomDocument &doc);

	signals:
		void error(QTcpSocket::SocketError socketError);
		void requestSendToAll(const QString &msg);
		
		void requestRemoveConnection(ADServerConnection *self);
		
		void requestAuth(ADServerConnection *cnx, const QString &, const QString &);
		
		void requestOperation(ADServerConnection *cnx, const ADQuery *query);
		
	private:
		ADServerClient *m_client;
		
		QXmlSimpleReader m_reader;
		ADSPackageParser *m_parser;
		QString m_login;
		
		bool m_isLogged;
};

#endif
