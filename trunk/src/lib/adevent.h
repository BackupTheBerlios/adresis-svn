/***************************************************************************
 *   Copyright (C) 2006 by Jorge Cuadrado   *
 *   kuadrosx@zi0n   *
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
 
#ifndef ADEVENT_H
#define ADEVENT_H

#include <QVariant>
#include <global.h>
/**
	@author Hector Fabio Cruz Mosquera,0329876 <hectorcaz@gmail.com>
*/
class ADEvent : public QObject
{
	Q_OBJECT;
	public:
		enum Source{Client = 0, Server };
		ADEvent();
		ADEvent(Source source, Logic::Module module, Logic::Action action, const QVariant & data);
		~ADEvent();
		int module() const;
		int action() const;
		int source() const;
		
		
		void setSource(Source source);
		void setModule(Logic::Module module);
		void setAction(Logic::Action action);
		
		QString toString() const;
		QVariant data() const;
		
		bool isValid() const;
		
	private:
		Source m_source;
		Logic::Module m_module;
		Logic::Action m_action;
		QVariant m_data;
		
		bool m_valid;
		
	public slots:
		void setData(const QVariant & data );
		

};

#endif
