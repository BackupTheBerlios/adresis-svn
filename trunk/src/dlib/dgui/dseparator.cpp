/*
 *   Copyright (C) 1997  Michael Roth <mroth@wirlweb.de>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Library General Public License for more details.
 *
 *   You should have received a copy of the GNU Library General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include "dseparator.h"
#include <QStyle>
#include <QStyleOption>
#include <QPainter>

DSeparator::DSeparator(QWidget* parent) : QFrame(parent)
{
   setLineWidth(1);
   setMidLineWidth(0);
   setOrientation( Qt::Horizontal );
}


DSeparator::DSeparator(Qt::Orientation orientation, QWidget* parent)
   : QFrame(parent)
{
   setLineWidth(1);
   setMidLineWidth(0);
   setOrientation( orientation );
}


void DSeparator::setOrientation(Qt::Orientation orientation)
{
   if (orientation == Qt::Vertical) {
      setFrameShape ( QFrame::VLine );
      setFrameShadow( QFrame::Sunken );
      setMinimumSize(2, 0);
   }
   else {
      setFrameShape ( QFrame::HLine );
      setFrameShadow( QFrame::Sunken );
      setMinimumSize(0, 2);
   }
}

Qt::Orientation DSeparator::orientation() const
{
   return ( frameStyle() & VLine ) ? Qt::Vertical : Qt::Horizontal;
}

void DSeparator::virtual_hook( int, void* )
{ /*BASE::virtual_hook( id, data );*/ }
