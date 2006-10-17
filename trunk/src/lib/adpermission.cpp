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
#include "adpermission.h"

ADPermission::ADPermission(): ADObject()
{
	m_permisos.insert("gestionarUsuario", "0");
	m_permisos.insert("consultarUsuario", "0");
	m_permisos.insert("gestionarEspacio", "0");
	m_permisos.insert("consultarEspacio", "0");
	m_permisos.insert("gestionarAudiovisual", "0");
	m_permisos.insert("consultarAudiovisual", "0");
	m_permisos.insert("gestionarSemestral", "0");
	m_permisos.insert("gestionarTemporal", "0");
	m_permisos.insert("consultarReservas", "0");
	m_permisos.insert("gestionarReportes", "0");
	m_permisos.insert("consultarReportes", "0");
}


ADPermission::~ADPermission()
{
}


void ADPermission::setValues(XMLResults)
{
}

void ADPermission::setValues(QMap<QString, QStringList> vP)
{	
	QStringList acciones = vP.value("action");
	QStringList valores = vP.value("permission");
	
	QStringList::const_iterator it1 = acciones.begin();
	QStringList::const_iterator it2 = valores.begin();
		
	while(it1 != acciones.end())
	{
		m_permisos.insert((*it1),(*it2));
		it1++;
		it2++;
	}
	
	
	QStringList a = m_permisos.keys();
	QStringList v = m_permisos.values();
	for(int i=0;i < a.size();i++)
	{
		dDebug() << "a => "<< a.at(i) <<"  v=>"<< v.at(i);
	}	
}


void ADPermission::setValues(const QXmlAttributes& atts)
{
// 	dDebug() << "ADPermission::setValues(const QXmlAttributes& atts)       " <<atts.value("gestionarUsuario") << "  "<< atts.value("consultarUsuario")<< "  " << atts.value("gestionarEspacio") << " " << atts.value("consultarEspacio") << "  "<<  atts.value("gestionarAudiovisual")<< " " << atts.value("consultarAudiovisual") << "  " << atts.value("gestionarSemestral") << " "<<  atts.value("gestionarTemporal") <<"  " << atts.value("consultarReservas") << " " << atts.value("gestionarReportes") << "" << atts.value("consultarReportes");
			
	m_permisos.insert("gestionarUsuario", atts.value("gestionarUsuario"));
	m_permisos.insert("consultarUsuario", atts.value("consultarUsuario"));
	m_permisos.insert("gestionarEspacio", atts.value("gestionarEspacio"));
	m_permisos.insert("consultarEspacio", atts.value("consultarEspacio"));
	m_permisos.insert("gestionarAudiovisual", atts.value("gestionarAudiovisual"));
	m_permisos.insert("consultarAudiovisual", atts.value("consultarAudiovisual"));
	m_permisos.insert("gestionarSemestral", atts.value("gestionarSemestral"));
	m_permisos.insert("gestionarTemporal", atts.value("gestionarTemporal"));
	m_permisos.insert("consultarReservas", atts.value("consultarReservas"));
	m_permisos.insert("gestionarReportes", atts.value("gestionarReportes"));
	m_permisos.insert("consultarReportes", atts.value("consultarReportes"));

}



bool ADPermission::isValid()  const
{
	return m_valid;
}

QString ADPermission::toXml() const
{
	return "";
}

void ADPermission::fromXml(const QString&)
{
}

QDomElement ADPermission::toXml(QDomDocument &doc) const
{
	QDomElement root = doc.createElement("permissions");
	
	QStringList a = m_permisos.keys();
	QStringList v = m_permisos.values();
	QStringList::const_iterator it1 = a.begin();
	QStringList::const_iterator it2 = v.begin();

	while(it1 != a.end())
	{
		root.setAttribute((*it1),(*it2));
		it1++;
		it2++;
	}
	
	return root;
}




bool ADPermission::value(Logic::Module module, Logic::Action action)
{
	D_FUNCINFO;
	
	dDebug() << "Modulo=> " << module << "   Accion => " << action;
	m_value = false;
	
	if( module==Logic::Users && (action == Logic::Add || action == Logic::Del || action == Logic::Update ))
	{
		if(m_permisos.value("gestionarUsuario") == "1")
		{
			m_value=true;
		}
	}
	
	else if( module == Logic::Users && (action == Logic::Find))
	{
		if(m_permisos.value("consultarUsuario") == "1")
		{
			m_value=true;
		}
	}
	
	else if( module==Logic::Spaces && (action == Logic::Add || action == Logic::Del || action == Logic::Update ))
	{
		if(m_permisos.value("gestionarEspacio") == "1")
		{
			m_value=true;
		}
	}
	else if( module == Logic::Spaces && (action == Logic::Find))
	{
		if(m_permisos.value("consultarEspacio") == "1")
		{
			m_value=true;
		}
	}
	else if( module==Logic::Audiovisuals && (action == Logic::Add || action == Logic::Del || action == Logic::Update ))
	{
		if(m_permisos.value("gestionarAudiovisual") == "1")
		{
			m_value=true;
		}
	}
	else if( module == Logic::Audiovisuals && (action == Logic::Find))
	{
		if(m_permisos.value("consultarAudiovisual") == "1")
		{
			m_value=true;
		}
	}
	
	//COLOCO SOLAMENTE LA ACCION RSEMESTRAL YA QUE SE TOMARA QUE CUANDO SE QUIERA HACER UNA ACCION COMO INSERTA O CUALQUIER OTRA COSA CON UNA RESERVA SEMESTRAL LO QUE SE TIENE QUE VERIFICAR ES SI SE TIENE PERMSOS SOBRE LAS RESERVAS TEMPORALES
	
	else if( module==Logic::Reserves && action == Logic::RSemestral)
	{
		if(m_permisos.value("gestionarSemestral") == "1")
		{
			m_value=true;
		}
	}
	else if( module==Logic::Reserves && (action == Logic::Add || action == Logic::Del || action == Logic::Update ))
	{
		if(m_permisos.value("gestionarTemporal") == "1")
		{
			m_value=true;
		}
	}
	
	else if( module == Logic::Reserves && (action == Logic::Find))
	{
		if(m_permisos.value("consultarReservas") == "1")
		{
			m_value=true;
		}
	}
	
	else if( module==Logic::Reports && (action == Logic::Add || action == Logic::Del || action == Logic::Update ))
	{
		if(m_permisos.value("gestionarReportes") == "1")
		{
			m_value=true;
		}
	}
	
	else if( module == Logic::Reports && (action == Logic::Find))
	{
		if(m_permisos.value("consultarReportes") == "1")
		{
			m_value=true;
		}
	}
	else
	{
		dDebug() << "NO ENCONTRE BAJO EL PERMISO Modulo=> " << module << "   Accion => " << action;
	}
	dDebug() << "Termina " << m_value;
	return m_value;
}
