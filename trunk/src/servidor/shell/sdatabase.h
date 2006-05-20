/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@gmail.com                                           	   *
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

#ifndef SDATABASE_H
#define SDATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlField>

#include <QObject>

#include "adquery.h"
#include "sresultset.h"

class SDatabase;

/**
 * @short S Database Manager (aka SDBM )
 * Esta clase gestiona las conexiones a la base de datos
 * 
 * Esta clase usa el patron singleton, accesible mediante la macro SDBM, que sustituye a SDatabase::instance()
 * @author David Cuadrado \<krawek@gmail.com\>
*/
class SDatabase : public QObject, public QSqlDatabase
{
	Q_OBJECT;
	protected:
		
    		/**
    		 * Constructor
    		 * @param parent 
    		 * @return 
    		 */
    		SDatabase(QObject *parent = 0);
		
    		/**
    		 * Destructor
    		 * @return 
    		 */
    		~SDatabase();
		
	public:
		static SDatabase *instance();
		
		/**
		 * Coloca el nombre de la base de datos
		 * @param dbname 
		 */
		void setDatabaseName ( const QString & dbname );
		/**
		 * Coloca el usuario de la base de datos
		 * @param login 
		 */
		void setUserName ( const QString & login );
		/**
		 * Coloca el password de la base de datos.
		 * @param password 
		 */
		void setPassword ( const QString & password );
		/**
		 * Coloca el hostname de la base de datos.
		 * @param host 
		 */
		void setHostName ( const QString & host );
		/**
		 * Pone el puerto.
		 * @param p 
		 */
		void setPort ( int p );
		
		/**
		 * Configura la conexion, y la deja lista para ser utilizada. 
		 * @param dbname 
		 * @param login 
		 * @param password 
		 * @param host 
		 */
		void setupConnection(const QString & dbname, const QString & login, const QString & password, const QString & host );
		
		
		SResultSet execQuery(const ADQuery *query );

	signals:
		/**
		 *  Marca el progreso de una consulta
		 * @param p 
		 */
		void progress(int p);
		
		/**
		 * Este slot se emite cuando se ha realizado una consulta y envia verdadero en caso de que la consulta sea exitosa y falso en cualquier otro caso
		 */
		void executed(bool good);
		
		
	public slots:
		
		/**
		 * Funcion de bajo nivel para realizar consultas
		 */
		SResultSet execRawQuery(const QString &sql);
		
	private:
		static SDatabase *m_instance;
};

#define SDBM SDatabase::instance()

#endif
