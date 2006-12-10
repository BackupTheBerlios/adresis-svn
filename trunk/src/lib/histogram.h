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

#ifndef HISTOGRAM_H
#define HISTOGRAM_H


#include<QVector>
#include<QDomElement>
#include<QDomDocument>
#include<QMap>
#include <QXmlDefaultHandler>
#include <QString>
#include <QApplication>
#include <QColor>
#include <QRect>
#include <QFontMetrics>
/**
 * @author Jorge Cuadrado <kuadrosxx@gmail.com>
 * @class Bar es una barra de histograma, contiene su color, titulo y valor
 */
class Bar
{
	public:
		Bar();
		Bar( int value , const QString& label = QString::null, const QColor& color = Qt::red);
		
		Bar(const Bar& copy);
		~Bar();
		int value();
		double operator*(double mult);
		int operator++(int);
		QString label() const;
		QColor color() const;
		
		
	private:
		int m_value;
		QString m_label;
		QColor m_color;
		
};



/**
 * @author Jorge Cuadrado <kuadrosxx@gmail.com>
 * 
 * @class Histogram es un Histograma, provee una de herramienta dibujar histogramas
 */
class Histogram: public QObject
{
	Q_OBJECT
	public:
		Histogram(const QRect & boundingRect = QRect(), QObject *parent = 0);
		Histogram(const QRect & boundingRect, const QVector<Bar> & values, QObject *parent = 0);
		
		~Histogram();
		void draw(QPainter *painter);
		
		void setBars(QVector<Bar> bars);
		void setValues(const QMap<QString, int> & values);
		
		
		QSize sizeHint(const QFontMetrics & fm = QApplication::fontMetrics());
		QString toString() const;
		
	private:
		void calculateMaxValue();
		QVector<Bar> m_bars;
		int m_maxValue;
		QRect m_boundingRect;
		
		
	signals:
		void requestUpdate();
};


/**
 * @author Jorge Cuadrado <kuadrosxx@gmail.com>
 */
class HistogramFactory : public QXmlDefaultHandler
{
	public:
		HistogramFactory();
		~HistogramFactory();
		
		bool startElement(const QString& , const QString& , const QString& qname, const QXmlAttributes& atts);
		bool endElement( const QString& ns, const QString& localname, const QString& qname);
		bool characters ( const QString & ch );
		bool error ( const QXmlParseException & exception );
		bool fatalError ( const QXmlParseException & exception );
		
		Histogram *build(const QString &document);
		
	private:
		QString m_qname;
		bool m_isParsing;
		bool m_readCharacters;
		Histogram * m_histogram;
		QVector<Bar> m_bars;
		QString m_root;
};
#endif
