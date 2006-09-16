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

#include "ssuccesspackage.h"

#include <QDir>

#include "dglobal.h"
#include "ddebug.h"

SSuccessPackage::SSuccessPackage(const QString& msg): QDomDocument()
{
	QDomElement root = createElement("Success");
	appendChild(root);
	
	QDomElement smsg = createElement("Message");
	smsg.setAttribute( "value", msg);
	
	root.appendChild(smsg);
	
	// Leemos los formularios desde un archivo de indices
	QFile formsIndex(DATA_DIR+"/forms/index.drc");
	
	if( formsIndex.open( QIODevice::ReadOnly | QIODevice::Text))
	{
		QDomDocument doc;
		if ( doc.setContent( &formsIndex ) )
		{
			QDomElement docElem = doc.documentElement();
			
			QDomNode n = docElem.firstChild();
			
			QDomElement module;
			
			while(!n.isNull()) 
			{
				QDomElement e = n.toElement();
				if(!e.isNull()) 
				{
					if ( e.tagName() == "Module" )
					{
						QString name = e.attribute("key");
						module = createElement("Module");
						module.setAttribute("key", name);
						module.setAttribute("text", e.attribute("text"));
						
						parseModule( e, module );
						
						documentElement().appendChild(module);
					}
				}
				n = n.nextSibling();
			}

		}
		else
		{
			dError() << "Invalid resource index file: " << formsIndex.fileName();
		}
		
		formsIndex.close();
	}
}

void SSuccessPackage::parseModule(QDomElement &element, QDomElement &module)
{
	QDomNode n = element.firstChild();
	
	while(!n.isNull()) 
	{
		QDomElement e = n.toElement();
		
		if ( e.tagName() == "FormFile" )
		{
			addForm( module, e.attribute( "id").toInt(), DATA_DIR+"/forms/"+e.attribute( "path") );
		}
		
		n = n.nextSibling();
	}
}


SSuccessPackage::~SSuccessPackage()
{
}

void SSuccessPackage::addForm(QDomElement &module, int id, const QString &formPath)
{
	dDebug() << "Adding form from: " << formPath;
	QFile ffile(formPath);
	
	if ( ffile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QDomElement formDef = createElement("FormDef");
		formDef.setAttribute( "id", id);
		
		QDomCDATASection cdata = createCDATASection( QString::fromLocal8Bit(ffile.readAll()));
		
		formDef.appendChild(cdata);
		
		module.appendChild(formDef);
		
		ffile.close();
	}
}


