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
// 	if("ERROR:  llave duplicada viola restricción unique \"aduser_pke\"" ==error.databaseText())
// 	{
// 	   	mensaje = "El usuario que usted ha intentado ingresar ya existe";
// 	}
// 	
// 	
// 	if("ERROR:  llave duplicada viola restricción unique \"adspace_pkey\""==error.databaseText())
// 	{
// 	   	mensaje = "El espacio que usted ha intentado ingresar ya existe";
// 	}
// 	
// 	
// 	if("ERROR:  llave duplicada viola restricción unique \"adaudiovisual_pkey\""==error.databaseText())
// 	{
// 		mensaje = "La ayuda audiovisual que usted ha intentado ingresar ya existe";
// 	}
// 	
// 	if("ERROR:  inserción o actualización en la tabla \"adaudiovisual\" viola la llave for ánea \"$1\""==error.databaseText())
// 	{
// 		mensaje = "El espacio especificado no existe";
// 	}
// 	
// 	
// 	if("ERROR:  inserción o actualización en la tabla \"adscheduleav\" viola la llave foránea \"$2\""==error.databaseText())
// 	{
// 		mensaje = "La ayuda audiovisual no existe";
// 	}
// 	
// 	if("ERROR:  llave duplicada viola restricción unique \"adscheduleav_pkey\""==error.databaseText())
// 	{
// 		mensaje = "El horario que usted ha intentado ingresar ya existe";
// 	}
// 
// 
// 	if("ERROR:  el nuevo registro para la relación \"adscheduleav\" viola la restricción check \"$1\""==error.databaseText())
// 	{
// 		mensaje = "La fecha no es congruente";
// 	}
// 	
// 	
// 	
// 	if("ERROR:  inserción o actualización en la tabla \"adschedulespace\" viola la llave foránea \"$2\""==error.databaseText())
// 	{
// 		mensaje = "El espacio no existe";
// 	}
// 	
// 	
// 	if("ERROR:  llave duplicada viola restricción unique \"adschedulespace_pkey\""==error.databaseText())
// 	{
// 		mensaje = "El horario que usted ha intentado ingresar ya existe";
// 	}
// 	
// 	
// 	if("ERROR:  el nuevo registro para la relación \"adschedulespace\" viola la restricción check \"$1\""==error.databaseText())
// 	{
// 		mensaje = "La fecha no es congruente";
// 	}
// 	
// 	
// 	
// 	
// 	if("ERROR:  inserción o actualización en la tabla \"adspacereserve\" viola la llave foránea \"$1\""==error.databaseText())
// 	{
// 		mensaje = "El usuario no existe";
// 	}
// 	
// 	if("ERROR:  inserción o actualización en la tabla \"adspacereserve\" viola la llave foránea \"$2\""==error.databaseText())
// 	{
// 		mensaje = "El espacio no existe";
// 	}
// 	
// 	
// 	if("ERROR:  inserción o actualización en la tabla \"adspacereserve\" viola la llave foránea \"$3\""==error.databaseText())
// 	{
// 		mensaje = "El horario asociado al espacio no existe";
// 	}
// 	
// 	
// 	if("ERROR:  llave duplicada viola restricción unique \"adspacereserve_pkey\""==error.databaseText())
// 	{
// 		mensaje = "La reserva ya existe";
// 	}
// 	
// 	if("ERROR:  inserción o actualización en la tabla \"adavreserve\" viola la llave foránea \"$1\""==error.databaseText())
// 	{
// 		mensaje = "El usuario no existe";
// 	}
// 	
// 	
// 	if("ERROR:  inserción o actualización en la tabla \"adavreserve\" viola la llave foránea \"$2\""==error.databaseText())
// 	{
// 		mensaje = "La ayuda audiovisual no existe";
// 	}
// 	
// 	
// 	if("ERROR:  inserción o actualización en la tabla \"adavreserve\" viola la llave foránea \"$3\""==error.databaseText())
// 	{
// 		mensaje = "El horario asociado a la ayuda audiovisual no existe";
// 	}
// 
// 	
// 	if("ERROR:  llave duplicada viola restricción unique \"adavreserve_pkey\""==error.databaseText())
// 	{
// 		mensaje = "La reserva ya existe";
// 	}
// 	
// 	QRegExp rg;
// 	
// 	
// 	rg.setPattern ( "ERROR:  la sintaxis de entrada no es válida para integer: \"*\"" );
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


