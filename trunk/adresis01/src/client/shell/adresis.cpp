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

	connect ( m_connector, SIGNAL(requestShowListAudiovisual(const QList<XMLResults>&)), this, SIGNAL (requestShowListAudioVisualAD( const QList<XMLResults>&)) );
	
	connect ( m_connector, SIGNAL(requestListTypes(const QList<XMLResults>&)), this, SIGNAL (requestListTypesAD( const QList<XMLResults>&)) );

	connect ( m_connector, SIGNAL(requestSchedule (const QList<XMLResults>&)), this, SIGNAL (requestScheduleAD( const QList<XMLResults>&)) );
	
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
	m_user.setValues(values);
 	Logic::TypeModule module;

////AQUI ES DONDE MANEJO QUE SE ENVIE A CREAR LAS FORMAS DE LAS CUALES EL USUARIO REALMENTE TIENE PERMISOS.
////LO ESTOY HACIENDO POR MEDIO DE ESTE FOR, EL CUAL VA HASTA 4, PARA REPRESENTAR LOS 4  MODULOS QUE HAY (USER, SPACE,
////AUDIOVISUAL Y RESERVES). ESTA NO DEBERIA DE SER LA FORMA EN QUE DEBERIA DE HACERSE YA QUE YO NO DEBERAIA DE COLOCAR EL 4 ASI
//// NO MAS PORQUE SI, DEBERIA DE HABER ALGUNA FORMA DE TOMAR EL TAMAÑO DE MODULOS QUE HAY, PERO ES Q REALMENTE NO SE COMO SE
//// DEBERIA DE HACER. POR AHORA LO VOY A DEJAR ASI.

	if(m_user.permissions()[Logic::administrador])
	{
		for(int i=0; i < 5; i++)
		{
			module = Logic::TypeModule(i);
			
			switch(module)
			{
				case Logic::users:
				{
					requestCreateModules(module);
					break;
				}
				case Logic::spaces:
				{
					requestCreateModules(module);
					break;
				}
				case Logic::audiovisuals:
				{
					requestCreateModules(module);
					break;
				}
				case Logic::reserves:
				{
					requestCreateModules(module);
					break;
				}
			}
		}
	}
	else
	{
		requestCreateModules(Logic::reserves);
	}
	
}





void Adresis::getInfoModule(Logic::TypeModule module )
{
	D_FUNCINFO;
	if(m_user.permissions()[Logic::administrador])
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
				ADSelectPackage selectSpace(QStringList()<< "adspacereserve"<< "adspace", QStringList() <<"idreserve as id" <<"typereserve as type"<<"namespace as name");
				selectSpace.setWhere( "adspacereserve.idresource = adspace.codespace and isactive=true");
				m_connector->sendQuery(Logic::fillReserveModule, selectSpace);
				
				ADSelectPackage selectAV(QStringList()<< "adavreserve"<< "adaudiovisual", QStringList() <<"idreserve as id" << "typereserve as type"<<"typeav as name");
				selectAV.setWhere( "adavreserve.idresource = adaudiovisual.numberinventoryav and isactive=true" );
				m_connector->sendQuery(Logic::fillReserveModule, selectAV);
			}
		}
	}


	else
	{
		ADSelectPackage selectSpace(QStringList()<< "adspacereserve"<< "adspace", QStringList() <<"idreserve as id" <<"typereserve as type"<<"namespace as name");
		selectSpace.setWhere( "adspacereserve.idresource = adspace.codespace and isactive=true");
		m_connector->sendQuery(Logic::fillReserveModule, selectSpace);

		ADSelectPackage selectAV(QStringList()<< "adavreserve"<< "adaudiovisual", QStringList() <<"idreserve as id" << "typereserve as type"<<"typeav as name");
		selectAV.setWhere( "adavreserve.idresource = adaudiovisual.numberinventoryav and isactive=true" );
		m_connector->sendQuery(Logic::fillReserveModule, selectAV);
	}
}


void Adresis::addUser(const QString& name, const QString& code,const QString& login,const QString& passwd,QMap<Logic::TypeUser, bool> permissions )
{
	dDebug() << "Adresis::addUser(const QString& name, const QString& code,const QString& login,const QString& passwd,QMap<Logic::TypeModule, bool> permissions )";
	ADUser newUser(name,  code, login,passwd, permissions);
	ADInsertPackage insert = newUser.insertPackage();
	m_connector->sendPackage( insert );
	getInfoModule(Logic::users);
}

void Adresis::modifyUser(const QString& name, const QString& code,const QString& login, const QString& passwd, QMap<Logic::TypeUser, bool> permissions )
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

void Adresis::addReserve( const QString& table, const QString& typeR, const QString& userReserve, const QString& userResponsable, const QString& idRecurso, const QString& day, const QString& beginhour, const QString& endhour, const QString& begindate, const QString& enddate, const bool& isactive, const QString& destinationReserve)
{

	dDebug() << "Adresis::addReserve";
	ADReserve newReserve(typeR, userReserve, userResponsable, idRecurso, day, beginhour, endhour, begindate, enddate, isactive, destinationReserve);
	
	ADInsertPackage insert = newReserve.insertPackage( table );
	m_connector->sendPackage( insert );
	getInfoModule(Logic::reserves);

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
		case Logic::reserves:
		{
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
		case Logic::reserves:
		{
		
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

	columns << "typeav" << "numberinventoryav" << "marksequipmentav" << "estateav" << "codespace";
	where = "codespace = '"+code+"'";
	table = "adaudiovisual";
	
	type = Logic::queryListAudiovisual;
	ADSelectPackage query(QStringList() << table, columns, true );
	query.setWhere(where);
	m_connector->sendQuery(type, query);


}

void Adresis::consultListTypes( const QString &typeL)
{
	QStringList columns;
	QString table;
	Logic::TypeQuery type;
	
	columns << "type";
	
	table = typeL;
	type = Logic::querytypes;
	ADSelectPackage query(QStringList() << table, columns, true );
	m_connector->sendQuery(type, query);
}

void Adresis::requestNameResourcesAD(const QString table, const QString typeResource)
{
	QStringList columns;
	QString where;
	Logic::TypeQuery type;
	
	if(table.operator==("adspace"))
	{
		columns << "codespace as idresource" <<"namespace as nameresource";
		where = "typespace = '"+typeResource+"'";
	}
	else if( table.operator==("adaudiovisual"))
	{
		columns << "numberinventoryav as idresource";
		where = "typeav = '"+typeResource+"'";
	}
	
	type = Logic::querytypes;
	ADSelectPackage query(QStringList() << table, columns, true );
	query.setWhere(where);
	m_connector->sendQuery(type, query);
}


void Adresis::consultScheduleAD( const QString& table, const QString& name )
{
	QStringList columns;
	QStringList tables;
	QString where;
	Logic::TypeQuery type;
	
	dDebug() << "CONSULTSCHEDULEAD CONSULTSCHEDULEAD CONSULTSCHEDULEAD";
// 	select typereserve, nameuser, day, beginhour, endhour, begindate, enddate from adspacereserve, adspace, aduser where namespace= 'salon ingenieria 1'  and adspacereserve.idspace = adspace.codespace and iduser=loginuser;
	
	columns << "typereserve" << "nameuser" << "day" <<"beginhour" << "endhour" << "begindate" << "enddate";
	
	if(table.operator==("adspace"))
	{
		tables << "adspace" << "adspacereserve" << "aduser";
		where = "codespace= '"+name+"' and adspacereserve.idresource = adspace.codespace and iduserresponsable=loginuser";
	}
	else if( table.operator==("adaudiovisual") )
	{
		tables << "adaudiovisual" << "adavreserve" << "aduser";
		where = "numberinventoryav= '"+name+"' and adavreserve.idresource = adaudiovisual.numberinventoryav and iduserresponsable=loginuser";
	}
	
	type = Logic::querySchedule;
	ADSelectPackage query(tables, columns, true );
	query.setWhere(where);
	m_connector->sendQuery(type, query);
}


void Adresis::consultInfoUser()
{
	emit requestInfoUser(m_user.login(), m_user.permissions()[Logic::administrador]);
}


void Adresis::addDelResourceAD( const QString &opcion, const QString &table ,const QString &resource)
{
	
	if( opcion.operator==("add"))
	{
		ADInsertPackage addDel = ADInsertPackage(table, QStringList() << "type", QStringList() << "'"+resource+"'");
		m_connector->sendPackage(addDel);
	}
	
	else if( opcion.operator==("del") )
	{
		ADDeletePackage addDel(table);
		QString where = "type = '"+resource+"'";
		addDel.setWhere( where );
		m_connector->sendPackage(addDel);
	}
}

