/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado krawek@gmail.com                 *
 *                         Jorge Cuadrado  kuadrosx@gmail.com              *
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

#ifndef ADCONNECTOR_H
#define ADCONNECTOR_H

#include "adconnectorbase.h"

#include <QStringList>
#include <QXmlSimpleReader>
#include <QQueue>

#include "global.h"

#include "adselectpackage.h"
class ADPackageParser;
/**
 * Maneja las conexiones al servidor, asi mismo tambien maneja los errores de conexion
 * @author Jorge Cuadrado <kuadrosxx@gmail.com>
*/
class ADConnector : public ADConnectorBase
{
	Q_OBJECT;
	public:
		ADConnector(QObject * parent = 0);
		~ADConnector();
		void login(const QString &user, const QString &passwd);
		void sendQuery(Logic::TypeQuery type, const ADSelectPackage& select);
		
	private slots:
		void readFromServer();
		void handleError(QAbstractSocket::SocketError error);
		
	signals:
// 		void readedModuleForms(const ModuleForms &);
		void chatMessage(const QString &login, const QString &msg);
		void message(Msg::Type t, const QString &message);
		void userAutenticated(const XMLResults&);
		void fillModule(Logic::TypeModule, const QList<XMLResults>&);
		
	private:
		QXmlSimpleReader m_reader;
		ADPackageParser *m_parser;
		QString m_readed;
		QQueue<Logic::TypeQuery> m_querys;
};

#endif
