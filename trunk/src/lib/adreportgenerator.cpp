/***************************************************************************
 *   Copyright (C) 2006 by Jorge Cuadrado                                  *
 *   kuadrosxx@gmail.com                                                   *
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
#include "adreportgenerator.h"

#include <QtGui>
#include <QObject>


#include "adreserve.h"
#include "ddebug.h"

ADReportGenerator::ADReportGenerator()
{
}


ADReportGenerator::~ADReportGenerator()
{
}

QTextDocument * ADReportGenerator::generateSchedule(int week, const QList<QVariant>& reserves  )
{
	QTextDocument * document =new QTextDocument();
	QTextCursor cursor( document );
	
	QTextTableFormat tableFormat;
	tableFormat.setAlignment(Qt::AlignHCenter);
	tableFormat.setBackground(QColor("#e0e0e0"));
	tableFormat.setCellPadding(2);
	tableFormat.setCellSpacing(4);
	QVector<QTextLength> constraints;
	constraints << QTextLength(QTextLength::PercentageLength, 12)
			<< QTextLength(QTextLength::PercentageLength, 12)
			<< QTextLength(QTextLength::PercentageLength, 12)
			<< QTextLength(QTextLength::PercentageLength, 12)
			<< QTextLength(QTextLength::PercentageLength, 12)
			<< QTextLength(QTextLength::PercentageLength, 12)
			<< QTextLength(QTextLength::PercentageLength, 12)
			<< QTextLength(QTextLength::PercentageLength, 12);
	tableFormat.setColumnWidthConstraints(constraints);

	QTextTable *table = cursor.insertTable(1, 8, tableFormat);
	QTextFrame *frame = cursor.currentFrame();
	QTextFrameFormat frameFormat = frame->frameFormat();
	frameFormat.setBorder(1);
	frame->setFrameFormat(frameFormat);

	QTextCharFormat format = cursor.charFormat();

	QTextCharFormat boldFormat = format;
	boldFormat.setFontWeight(QFont::Bold);

	QTextCharFormat highlightedFormat = boldFormat;
	QTime time( 7, 0);

	
	for (int weekDay = 1; weekDay <= 8; ++weekDay) 
	{
		QTextTableCell cell = table->cellAt(0, weekDay-1);
		QTextCursor cellCursor = cell.firstCursorPosition();
		if(weekDay == 1)
		{
			cellCursor.insertText(QString("%1").arg("Hora"), boldFormat);
		}
		else
		{
			cellCursor.insertText(QString("%1").arg(QDate::longDayName(weekDay-1)), boldFormat);
		}
	}
	
	for(int i = 0; i < 29; i++)
	{
		table->insertRows(table->rows(), 1);
		QTextTableCell cell = table->cellAt(i+1, 0);
		QTextCursor cellCursor = cell.firstCursorPosition();
		cellCursor.insertText( time.toString ("hh:mm") );
		time = time.addSecs(1800);
	}
	
	
	foreach(QVariant r,  reserves)
	{
		ADReserve *reserve = qvariant_cast<ADReserve *>(r);
		if(reserve)
		{
			QDateTime begin = reserve->beginDateTime();
			QDateTime end = reserve->endDateTime();
			int year = begin.date().year();
			QTextCharFormat f;
			f.setUnderlineColor ( Qt::red );
			
			QStringList days;
			for(int i = 1; i <= 7; i++)
			{
				days << QDate::longDayName(i);
			}
			if( begin.date().weekNumber( &year ) == week )
			{
				for(int i = (((begin.time().hour()+(begin.time().minute()/60)-7)*2)+1);i < (((end.time().hour()+(end.time().minute()/60)-7)*2)+1); i++)
				{
					QTextTableCell cell = table->cellAt(i , days.indexOf( reserve->day() )+1);
					QTextCursor cellCursor = cell.firstCursorPosition();
					cellCursor.insertText( "reservado" );
				}
			}
		}
	}
	return document;
}


QTextDocument *ADReportGenerator::generateListReserves(const QList<QVariant>& reserves, bool isSpace  )
{
	QTextDocument * document =new QTextDocument();
	QTextCursor cursor( document );
	return document;
}
