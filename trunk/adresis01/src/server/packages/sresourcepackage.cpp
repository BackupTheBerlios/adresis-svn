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

#include "sresourcepackage.h"
#include <QFile>
#include <QString>
#include <QDir>

#include <dglobal.h>
#include <ddebug.h>

SResourcePackage::SResourcePackage() : QDomDocument()
{
	QDomElement root = createElement("Resources");
	appendChild(root);
	
	// Leemos los datos desde un archivo de indices
	QFile rscIndex(HOME+"/data/index.rsc");
	
	if( rscIndex.open( QIODevice::ReadOnly | QIODevice::Text))
	{
		QDomDocument doc;
		if ( doc.setContent( &rscIndex ) )
		{
			QDomElement docElem = doc.documentElement();
			
			QDomNode n = docElem.firstChild();
			
			QDomElement resource;
			
			while(!n.isNull())
			{
				QDomElement e = n.toElement();
				if(!e.isNull())
				{
					dDebug() << e.tagName();
					
					QString path = e.attribute("path");
						
					QString name = e.attribute("name");
						
					if( name.isEmpty() )
					{
						QFileInfo finfo(path);
						name = finfo.fileName();
					}
					
					if ( e.tagName() == "Image" )
					{
						resource = createElement("Image");
						resource.setAttribute("filename", name);
						
						addResource( resource, path );
						
						documentElement().appendChild(resource);
					}
					else if ( e.tagName() == "Svg" )
					{
						resource = createElement("Svg");
						resource.setAttribute("filename", name);
						
						addResource( resource, path );
						
						documentElement().appendChild(resource);
					}
					else if ( e.tagName() == "Sourd" )
					{
						resource = createElement("Sourd");
						resource.setAttribute("filename", name);
						
						addResource( resource, path );
						
						documentElement().appendChild(resource);
					}
					
					
				}
				n = n.nextSibling();
			}

		}
		else
		{
			dError() << "Invalid resource index file: " << rscIndex.fileName();
		}
		
		rscIndex.close();
	}
}


SResourcePackage::~SResourcePackage()
{
}

void SResourcePackage::addResource(QDomElement &element, const QString &path)
{
	QFileInfo finfo(path);
	
	QString realPath = path;
	
	if ( finfo.isRelative() )
	{
		realPath = HOME+"/data/"+path;
	}
	
	QFile file(realPath);
	
	
	if( file.open( QIODevice::ReadOnly ))
	{
		QDomCDATASection cdata = createCDATASection( QString::fromLocal8Bit(file.readAll()));
		
		element.appendChild(cdata);
		
		file.close();
	}
}



