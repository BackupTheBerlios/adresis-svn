/***************************************************************************
 *   Copyright (C) 2006 by Jorge Cuadrado   *
 *   kuadrosx@gmail.com   *
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
#ifndef ADRESIS_H
#define ADRESIS_H

#include <QObject>

#include "adconnector.h"
#include "global.h"
#include "aduser.h"
#include "adaudiovisual.h"
#include "adspace.h"


/**
* @author Jorge Cuadrado <kuadrosxx@gmail.com>
*/
class Adresis : public QObject
{
	Q_OBJECT;
	public:
		Adresis(QObject * parent=0);
		~Adresis();
		
	public slots:
		void addUser(const QString& name, const QString& code,const QString& login,const QString& passwd,QMap<Logic::TypeModule, bool> permissions );
		
		void modifyUser(const QString& name, const QString& code,const QString& login,const QString& passwd,QMap<Logic::TypeModule, bool> permissions );
		
		
		//cambio
		void modifyAudiovisual(const QString& typeav, const QString& marksEquipmentav,const QString& estateav,const QString& numberinventoryav, const QString& codeSpace);
		//cambio
		
		void addSpace(const QString& codeSpace, const QString& typeSpace,const bool & coolAirSpace,const QString& capacitySpace, const QString& nameSpace);

		void addAudiovisual(const QString& typeav, const QString& marksEquipmentav, const QString& estateav, const QString& numberinventoryav, const QString& codeSpace);
		
		void modifySpace(const QString& codeSpace, const QString& typeSpace,const bool & coolAirSpace,const QString& capacitySpace, const QString& nameSpace);
		
		
	public slots:
		void connectToHost( const QString & hostName, quint16 port);
		void login(const QString &user, const QString &passwd);
		void autenticated(const XMLResults& values);
		void getInfoModule(Logic::TypeModule module);
		void execDelete(Logic::TypeModule, const QString& key);
		void getObject(Logic::TypeModule, const QString& key);
		void createUser(const XMLResults &result);
		void createSpace(const XMLResults &result);
		void createAudiovisual(const XMLResults& result);
		
		void consultListAudiovisual(const QString &code);
// 		void requestListAudioVisualSpaces(const QList<XMLResults>& result);

				
	signals:
		void requestShowMessage(Msg::Type type, const QString& message );
		void requestCreateModules();
		void requestFillModule(Logic::TypeModule, const QList<XMLResults>&);
		void showUser( const ADUser &);
		void showSpace( const ADSpace &);
		void showAudiovisual( const ADAudioVisual & );
		
		void requestShowListAudioVisualAD(const QList<XMLResults>& result);
		
		
	private:
		ADConnector *m_connector;
		ADUser m_user;
		
};

#endif
