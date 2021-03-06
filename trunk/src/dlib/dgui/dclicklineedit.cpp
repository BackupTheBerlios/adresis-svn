/*
   This file is part of libkdepim.
   Copyright (c) 2004 Daniel Molkentin <molkentin@kde.org>
   based on code by Cornelius Schumacher <schumacher@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/


#include "dclicklineedit.h"

#include <QPainter>


DClickLineEdit::DClickLineEdit( const QString &msg, QWidget *parent) :
        QLineEdit( parent)
{
	mDrawClickMsg = true;
	setClickMessage( msg );
	
	setFocusPolicy ( Qt::ClickFocus );
	
#if QT_VERSION >= 0x0402000
	setStyleSheet(QString(":enabled { padding-right: %1; }").arg(8));
#endif
}

void DClickLineEdit::setClickMessage( const QString &msg )
{
	mClickMessage = msg;
	repaint();
}


void DClickLineEdit::setText( const QString &txt )
{
	mDrawClickMsg = txt.isEmpty();
	repaint();
	QLineEdit::setText( txt );
}

void DClickLineEdit::paintEvent( QPaintEvent *e )
{
	QLineEdit::paintEvent(e);
	
	QPainter p(this);
	if ( mDrawClickMsg == true && !hasFocus() )
	{
		QPen tmp = p.pen();
		p.setPen( palette().color( QPalette::Disabled, QPalette::Text ) );
		QRect cr = contentsRect();
		
		cr.adjust(3, 0, 0 ,0);
		p.drawText( cr, Qt::AlignVCenter, mClickMessage );
		p.setPen( tmp );
	}
}


void DClickLineEdit::focusInEvent( QFocusEvent *ev )
{
	if ( mDrawClickMsg == true ) 
	{
		mDrawClickMsg = false;
		repaint();
	}
	QLineEdit::focusInEvent( ev );
}


void DClickLineEdit::focusOutEvent( QFocusEvent *ev )
{
	if ( text().isEmpty() ) 
	{
		mDrawClickMsg = true;
		repaint();
	}
	QLineEdit::focusOutEvent( ev );
}

