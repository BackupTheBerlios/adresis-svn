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
#include "adresis.h"
#include "addeletepackage.h"
#include "adselectpackage.h"
#include <ddebug.h>
Adresis::Adresis(QObject * parent)
	: QObject(parent)
{
	m_connector = new ADConnector(this);
	connect ( m_connector, SIGNAL(message(Msg::Type , const QString &)), this, SIGNAL(requestShowMessage( Msg::Type, const QString& )));
	connect ( m_connector, SIGNAL(userAutenticated(const XMLResults&)) , this, SLOT(autenticated(const XMLResults&) ));
	
	connect ( m_connector, SIGNAL(fillModule(Logic::TypeModule, const QList<XMLResults>&)), this, SIGNAL(requestFillModule(Logic::TypeModule, const QList<XMLResults>&)) );
	
	connect( m_connector, SIGNAL(requestShowUser(const XMLResults& )), this, SLOT(createUser(const XMLResults& )));
	
	connect( m_connector, SIGNAL(requestShowSpace(const XMLResults& )), this, SLOT(createSpace(const XMLResults& )));
	
	connect( m_connector,SIGNAL(requestShowAudiovisual(const XMLResults& )),this,SLOT(createAudiovisual(const XMLResults& )));

	connect ( m_connector, SIGNAL(requestListAudiovisual(const QList<XMLResults>&)), this, SIGNAL (requestListAudioVisual( const QList<XMLResults>&)) );
	
	
	DINIT;
}

Adresis::~Adresis()
{
	DEND;
}

void Adresis::createUser(const XMLResults& result)
{
	D_FUNCINFO;
	ADUser user;
	user.setValues(result);
	emit showUser(user);
}

void Adresis::createSpace(const XMLResults& result)
{
	D_FUNCINFO;
	ADSpace space;
	space.setValues(result);
	emit showSpace(space);
}

void Adresis::createAudiovisual(const XMLResults& result)
{
	D_FUNCINFO;
	ADAudioVisual audiovisual;
	audiovisual.setValues(result);
	emit showAudiovisual(audiovisual);
}

void Adresis::connectToHost( const QString & hostName, quint16 port)
{
	m_connector->connectToHost( hostName, port);
}

//slots
void Adresis::login(const QString &user, const QString &passwd)
{
	m_connector->login(user, passwd);
	ADSelectPackage u(QStringList()<< "aduser", QStringList() << "nameuser" << "codeuser" << "loginuser"<< "passwduser" << "permissionsuser");
	u.setWhere( "loginuser='"+  user + "'" );
	m_connector->sendQuery(Logic::userAuthenticated, u); 
}

void Adresis::autenticated(const XMLResults & values)
{
	m_user.setValues( values);
	requestCreateModules();
}

void Adresis::getInfoModule(Logic::TypeModule module )
{
	D_FUNCINFO;
	if(m_user.permissions()[module])
	{
		switch(module)
		{
			case Logic::users:
			{
				ADSelectPackage select(QStringList()<< "aduser", QStringList() << "nameuser"<< "loginuser" );
				m_connector->sendQuery(Logic::fillUserModule, select);
				break;
			}
			case Logic::spaces:
			{
				ADSelectPackage select(QStringList()<< "adspace", QStringList() << "codespace" <<"typespace" << "namespace" );
				m_connector->sendQuery(Logic::fillSpaceModule, select);
				break;
			}

			case Logic::audiovisuals:
			{
				ADSelectPackage select(QStringList()<< "adaudiovisual", QStringList() << "typeAV"<<"numberinventoryAV");
				m_connector->sendQuery(Logic::fillAudiovisualModule, select);
				break;
			}
			
			case Logic::reserves:
			{
// 				ADSelectPackage select(QStringList()<< "adaudiovisual", QStringList() << "type"<<"inventoryNumber");
// 				m_connector->sendQuery(Logic::fillAudioVisualModule, select);
			}
		}
	}
	else
	{
		emit requestShowMessage(Msg::Error, "Error, no tiene permisos sobre este modulo");
	}
}

void Adresis::addUser(const QString& name, const QString& code,const QString& login,const QString& passwd,QMap<Logic::TypeModule, bool> permissions )
{
	dDebug() << "Adresis::addUser(const QString& name, const QString& code,const QString& login,const QString& passwd,QMap<Logic::TypeModule, bool> permissions )";
	ADUser newUser(name,  code, login,passwd, permissions);
	ADInsertPackage insert = newUser.insertPackage();
	m_connector->sendPackage( insert );
	getInfoModule(Logic::users);
}

void Adresis::modifyUser(const QString& name, const QString& code,const QString& login, const QString& passwd, QMap<Logic::TypeModule, bool> permissions )
{
	D_FUNCINFO;
	ADUser newUser(name, code, login,passwd, permissions);
	ADUpdatePackage update = newUser.updatePackage();
	QString where = "loginuser = '" + login + "'";
	update.setWhere(where);
	m_connector->sendPackage( update );
	getInfoModule(Logic::users);
}



void Adresis::addAudiovisual(const QString& typeav, const QString& marksEquipmentav,const QString& estateav,const QString& numberinventoryav, const QString& codeSpace)
{
	dDebug() << "Adresis::addAudiovisual(const QString& typeav, const QString& marksEquipmentav,const QString& estateav,const QString& numberinventoryav, const QString& codeSpace)";
	ADAudioVisual newAudiovisual(typeav, marksEquipmentav, estateav,  numberinventoryav, codeSpace);
	ADInsertPackage insert = newAudiovisual.insertPackage();
	m_connector->sendPackage( insert );
	getInfoModule(Logic::audiovisuals);
}



void Adresis::modifyAudiovisual(const QString& typeav, const QString& marksEquipmentav,const QString& estateav,const QString& numberinventoryav, const QString& codeSpace)
{
	D_FUNCINFO;
	ADAudioVisual newAudiovisual(typeav, marksEquipmentav, estateav,  numberinventoryav, codeSpace);
	ADUpdatePackage update = newAudiovisual.updatePackage();
	QString where = "numberinventoryav = '" + numberinventoryav + "'";
	update.setWhere(where);
	m_connector->sendPackage(update);
	getInfoModule(Logic::audiovisuals);
}

void Adresis::addSpace(const QString& codeSpace, const QString& typeSpace,const bool & coolAirSpace,const QString& capacitySpace, const QString& nameSpace)
{
	dDebug() << "Adresis::addSpace(const QString& codeSpace, const QString& typeSpace,const QString& coolAirSpace,const QString& capacitySpace, const QString& nameSpace)";
	ADSpace newSpace(codeSpace, typeSpace, coolAirSpace, capacitySpace, nameSpace);
	
	ADInsertPackage insert = newSpace.insertPackage();
	m_connector->sendPackage( insert );
	getInfoModule(Logic::spaces);
}


void Adresis::modifySpace(const QString& codeSpace, const QString& typeSpace,const bool & coolAirSpace,const QString& capacitySpace, const QString& nameSpace)
{
	D_FUNCINFO;
	ADSpace newSpace(codeSpace, typeSpace, coolAirSpace, capacitySpace, nameSpace); //FIXME
	ADUpdatePackage update = newSpace.updatePackage();
	QString where = "codespace = '" + codeSpace + "'";
	update.setWhere(where);
	m_connector->sendPackage( update );
	getInfoModule(Logic::spaces);
}


void Adresis::execDelete(Logic::TypeModule module, const QString& key)
{
	QString where;
	QString table;
	switch(module)
	{
		case Logic::users:
		{
			where = "loginuser = '" + key + "'";
			table = "aduser";
			break;
		}
		
		case Logic::spaces:
		{
			where = "codespace = '"+ key +"'";
			table = "adspace";
			break;
		}

		case Logic::audiovisuals:
		{
			where = "numberinventoryav = '"+ key +"'";
			table = "adaudiovisual";
			break;
		}
	}
	ADDeletePackage del(table);
	del.setWhere(where);
	m_connector->sendPackage(del);
	dDebug() << module;
	getInfoModule(module);
}

void Adresis::getObject(Logic::TypeModule module, const QString& key)
{
	QStringList columns;
	QString table;
	QString where;
	Logic::TypeQuery type;
	
	switch(module)
	{
		case Logic::users:
		{
			columns << "nameuser" << "codeuser" << "loginuser"<< "passwduser" << "permissionsuser";
			where = "loginuser = '" + key + "'";
			table = "aduser";
			type = Logic::queryUser;
			break;
		}
		case Logic::audiovisuals:
		{
			columns << "typeav" << "marksequipmentav" << "estateav"<< "numberinventoryav" << "codespace";
			where = "numberinventoryav = '" + key + "'";
			table = "adaudiovisual";
			type = Logic::queryAudiovisual;

			break;
		}
		case Logic::spaces:
		{
			columns << "codespace" << "typespace" << "coolairspace" << "capacityspace" << "namespace";
			where = "codespace = '" + key + "'";
			table = "adspace";
			type = Logic::querySpace;
			break;
		}
	}
	ADSelectPackage query(QStringList() << table, columns, true );
	query.setWhere(where);
	m_connector->sendQuery(type, query);
}




void Adresis::consultListAudiovisual(const QString &code)
{
	QStringList columns;
	QString table;
	QString where;
	Logic::TypeQuery type;	

	columns << "typeav" << "numberinventoryav";
	where = "codespace = '"+code+"'";
	table = "adaudiovisual";
	
	type = Logic::queryListAudiovisual;
	ADSelectPackage query(QStringList() << table, columns, true );
	query.setWhere(where);
	m_connector->sendQuery(type, query);

	dDebug() << "Ya Mande la consulta";

}

// void Adresis::requestListAudioVisualSpaces(const QList<XMLResults> & result)
// {
// 	dDebug() << "";
// 	dDebug() << "Ya se los voy a enviar a Main Window";
// 	dDebug() << "";
// 	emit requestListAudioVisual(result);
// 	dDebug() << "Como que Ya se las envie a Main Window";
// 	dDebug() << "";
// }
