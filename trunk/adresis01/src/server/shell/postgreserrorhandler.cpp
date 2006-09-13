/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@gmail.com                                                       *
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

#include "postgreserrorhandler.h"

#include <QObject>
#include <QRegExp>
#include <QStringList>
#include <ddebug.h>
PostgresErrorHandler::PostgresErrorHandler()
{
}

PostgresErrorHandler::~PostgresErrorHandler()
{
}

SErrorPackage PostgresErrorHandler::handle(const QSqlError &error)
{
	
	dDebug() << error.databaseText();
	
	QString mensaje;
// 	if("ERROR:  llave duplicada viola restricci�n unique \"aduser_pke\"" ==error.databaseText())
// 	{
// 	   	mensaje = "El usuario que usted ha intentado ingresar ya existe";
// 	}
// 	
// 	
// 	if("ERROR:  llave duplicada viola restricci�n unique \"adspace_pkey\""==error.databaseText())
// 	{
// 	   	mensaje = "El espacio que usted ha intentado ingresar ya existe";
// 	}
// 	
// 	
// 	if("ERROR:  llave duplicada viola restricci�n unique \"adaudiovisual_pkey\""==error.databaseText())
// 	{
// 		mensaje = "La ayuda audiovisual que usted ha intentado ingresar ya existe";
// 	}
// 	
// 	if("ERROR:  inserci�n o actualizaci�n en la tabla \"adaudiovisual\" viola la llave for �nea \"$1\""==error.databaseText())
// 	{
// 		mensaje = "El espacio especificado no existe";
// 	}
// 	
// 	
// 	if("ERROR:  inserci�n o actualizaci�n en la tabla \"adscheduleav\" viola la llave for�nea \"$2\""==error.databaseText())
// 	{
// 		mensaje = "La ayuda audiovisual no existe";
// 	}
// 	
// 	if("ERROR:  llave duplicada viola restricci�n unique \"adscheduleav_pkey\""==error.databaseText())
// 	{
// 		mensaje = "El horario que usted ha intentado ingresar ya existe";
// 	}
// 
// 
// 	if("ERROR:  el nuevo registro para la relaci�n \"adscheduleav\" viola la restricci�n check \"$1\""==error.databaseText())
// 	{
// 		mensaje = "La fecha no es congruente";
// 	}
// 	
// 	
// 	
// 	if("ERROR:  inserci�n o actualizaci�n en la tabla \"adschedulespace\" viola la llave for�nea \"$2\""==error.databaseText())
// 	{
// 		mensaje = "El espacio no existe";
// 	}
// 	
// 	
// 	if("ERROR:  llave duplicada viola restricci�n unique \"adschedulespace_pkey\""==error.databaseText())
// 	{
// 		mensaje = "El horario que usted ha intentado ingresar ya existe";
// 	}
// 	
// 	
// 	if("ERROR:  el nuevo registro para la relaci�n \"adschedulespace\" viola la restricci�n check \"$1\""==error.databaseText())
// 	{
// 		mensaje = "La fecha no es congruente";
// 	}
// 	
// 	
// 	
// 	
// 	if("ERROR:  inserci�n o actualizaci�n en la tabla \"adspacereserve\" viola la llave for�nea \"$1\""==error.databaseText())
// 	{
// 		mensaje = "El usuario no existe";
// 	}
// 	
// 	if("ERROR:  inserci�n o actualizaci�n en la tabla \"adspacereserve\" viola la llave for�nea \"$2\""==error.databaseText())
// 	{
// 		mensaje = "El espacio no existe";
// 	}
// 	
// 	
// 	if("ERROR:  inserci�n o actualizaci�n en la tabla \"adspacereserve\" viola la llave for�nea \"$3\""==error.databaseText())
// 	{
// 		mensaje = "El horario asociado al espacio no existe";
// 	}
// 	
// 	
// 	if("ERROR:  llave duplicada viola restricci�n unique \"adspacereserve_pkey\""==error.databaseText())
// 	{
// 		mensaje = "La reserva ya existe";
// 	}
// 	
// 	if("ERROR:  inserci�n o actualizaci�n en la tabla \"adavreserve\" viola la llave for�nea \"$1\""==error.databaseText())
// 	{
// 		mensaje = "El usuario no existe";
// 	}
// 	
// 	
// 	if("ERROR:  inserci�n o actualizaci�n en la tabla \"adavreserve\" viola la llave for�nea \"$2\""==error.databaseText())
// 	{
// 		mensaje = "La ayuda audiovisual no existe";
// 	}
// 	
// 	
// 	if("ERROR:  inserci�n o actualizaci�n en la tabla \"adavreserve\" viola la llave for�nea \"$3\""==error.databaseText())
// 	{
// 		mensaje = "El horario asociado a la ayuda audiovisual no existe";
// 	}
// 
// 	
// 	if("ERROR:  llave duplicada viola restricci�n unique \"adavreserve_pkey\""==error.databaseText())
// 	{
// 		mensaje = "La reserva ya existe";
// 	}
// 	
// 	QRegExp rg;
// 	
// 	
// 	rg.setPattern ( "ERROR:  la sintaxis de entrada no es v�lida para integer: \"*\"" );
// 	if(rg.exactMatch ( error.databaseText() ))
// 	{
// 	QStringList list = error.databaseText().split( '\"' );
// 		mensaje = "El valor "+list[1]+" debe ser numerico";
// 	}
// 	
	
	// TODO: Analizar los textos!
	
	
	SErrorPackage package(error.number(), QObject::tr("PostgresErrorHandler %1").arg(error.databaseText()));
// 	SErrorPackage package(error.number(), QObject::tr("PostgresErrorHandler %1").arg(mensaje));
	
	
	
	return package;
}


