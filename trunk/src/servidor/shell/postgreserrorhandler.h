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

#ifndef POSTGRESERRORHANDLER_H
#define POSTGRESERRORHANDLER_H

#include "serrorpackage.h"
#include <QSqlError>

/**
 * Esta clase analiza los errores enviados por postgres y los traduce a un lenguaje que el usuario de la aplicación pueda entender.
 * @author David Cuadrado <krawek@gmail.com>
*/
class PostgresErrorHandler
{
	public:
		PostgresErrorHandler();
		~PostgresErrorHandler();
		
		/**
		 * Recibe la cadena de error del servidor postgres y retorna un paquete de error que será enviado al usuario.
		 * @param error 
		 * @return 
		 */
		static SErrorPackage handle(const QSqlError &error);
};

#endif
