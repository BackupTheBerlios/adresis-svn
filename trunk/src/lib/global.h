/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado krawek@gmail.com                 *
 *                         Jorge Cuadrado kuadrosxx@gmail.com              *
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

#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <QString>
#include <QHash>
#include <QMap>
#include <QList>

#include <QMetaType>
#include <dglobal.h>


#define SQLSTR(s) QString("\'"+((QString("%1").arg(s)).replace('\'', "\\\'"))+"\'")

namespace Msg
{
	enum Type
	{
		Info,
		Warning,
		Error
	};
	

}
namespace Logic
{
	enum TypeQuery{userAuthenticated=0, fillUserModule, fillSpaceModule, fillAudiovisualModule, fillReserveModule, queryUser, queryAudiovisual, querySpace, queryListAudiovisual, querytypes, querySchedule};
	enum Module{Users=0, Spaces, Audiovisuals, Reserves, Reports};
	enum Action{Add = 0, Del, Update, Find, Info,RSemestral};
	enum TypeUser{administrador=0};
	
}




typedef QHash<QString, QString> XMLResults;
Q_DECLARE_METATYPE( XMLResults * );
typedef QMap <Logic::TypeUser, bool> permissions;
Q_DECLARE_METATYPE( permissions * );


#endif


