

#include "histogram.h"

#include <QPainter>

#include <cmath>

#include <ddebug.h>

/**
 * @class Bar es una barra de histograma, contiene su color, titulo y valor
*/
/**
 * Constructor por defecto
*/
Bar::Bar()
{
	m_value = 0;
	m_color = Qt::red;
}

/**
 * Constructor
*/
Bar::Bar( int value, const QString& label, const QColor& color): m_value(value), m_label(label), m_color(color)
{
	
}
/**
 * Constructor por copia
*/
Bar::Bar(const Bar& copy): m_value(copy.m_value),m_label(copy.m_label), m_color(copy.m_color) 
{
}
/**
 * Destructor
 */
Bar::~Bar()
{
	
}
/**
 * Retorna el valor de la barra
 */
int  Bar::value()
{
	return m_value;
}
/**
 * Multiplica el valor de la barra por n
*/
double Bar::operator*(double n)
{
	return m_value * n;
}
/**
 * Aumenta el valor de la barra en uno
 * 
*/
int Bar::operator++(int)
{
	return m_value++;
}
/**
 * Retorna el titulo de la barra
*/
QString Bar::label() const
{
	return m_label;
}
/**
 * Retorna el colo de la barra
*/
QColor Bar::color() const
{
	return m_color;
}

/**
 * constructor
 */

Histogram::Histogram(const QRect & boundingRect, QObject *parent):QObject(parent), m_maxValue(0), m_boundingRect(boundingRect)
{
	QSize hint = sizeHint();
	if(boundingRect.size().height() < hint.height()  && boundingRect.size().width() < hint.width())
	{
		m_boundingRect.setSize(hint);
	}
}

/**
 * constructor
 */
Histogram::Histogram(const QRect & boundingRect, const QVector<Bar> & values, QObject *parent ):QObject(parent), m_bars(values)
{
	
	QSize hint = sizeHint();
	if(boundingRect.size().height() < hint.height()  && boundingRect.size().width() < hint.width())
	{
		m_boundingRect.setSize(hint);
	}
	calculateMaxValue();
}
/**
 * Destructor
 */
Histogram::~Histogram()
{
	
}
/**
 * dibuja el histograma
 */
void Histogram::draw(QPainter *painter)
{
// 	painter->setRenderHint( QPainter::Antialiasing /*| QPainter::TextAntialiasing*/, true );
	painter->setRenderHint( QPainter::TextAntialiasing, true );
	double s = 1;
	if(m_maxValue != 0)
	{
		s = (double)(m_boundingRect.height()-20) /((double)(m_maxValue));
	}
	
	QFontMetrics fm = painter->fontMetrics ();
	
	int marginX = fm.width( QString::number(m_boundingRect.height()*s )) + 5;
	int marginY = 20;
	painter->save();
	painter->setPen( QPen(QColor("gray")));
	
	int spaceX = m_boundingRect.width()/(m_bars.count()+2);
	if(spaceX <  fm.boundingRect("1").height() +3)
	{
		spaceX =  fm.boundingRect("1").height()+3;
	}
	
	for(int i = marginX; i < m_boundingRect.width(); i+= spaceX)
	{
		painter->drawLine( QPoint(i, 0), QPoint(i, m_boundingRect.height()) );
	}
	
	for(int j = 0; j < ((m_boundingRect.height()*s)*2)-marginY; j+=(20*s))
	{
		painter->drawLine(QPointF(m_boundingRect.width(), m_boundingRect.height()-marginY- j), QPointF(0, m_boundingRect.height()-marginY- j));
	}

	int count = 20;
	
	for(int j = (20)*s; j < ((m_boundingRect.height()*s)*2)-marginY; j+=(20*s))
	{
		painter->drawText( QPointF(0, m_boundingRect.height()-marginY- j),
			    QString::number(count) );
		
		count += 20;
		
	}
	
	painter->restore();
	
	if(!m_bars.isEmpty())
	{
		count = 0;
		painter->setBrush(QBrush(Qt::red));
		foreach(Bar value, m_bars)
		{
			
			QRectF r = QRectF(QPointF(marginX + (count*spaceX), m_boundingRect.height()-((value*s)+marginY)), QSizeF( spaceX, value * s));
			painter->setBrush(QBrush(value.color()));
			painter->drawRect( r );
			painter->save();
			
			QString lb = "";
			if(!value.label().isEmpty() || !value.label().isNull())
			{
				lb = "  Label: " + value.label() + " Value: "+ QString::number( value.value());
			}
			else
			{
				lb = " Value: "+ QString::number( value.value());
			}
			
			if(r.topLeft().y()+fm.width(lb)+ marginY < m_boundingRect.height())
			{
				painter->translate(r.topLeft() + QPointF(((r.width() - fm.boundingRect("1").height())/2), 0));
			}
			else
			{
				painter->translate(r.topLeft() + QPointF((r.width() - fm.boundingRect("1").height())/2, -fm.width(lb) ));
			}
			
			painter->rotate(90);
			painter->drawText ( QPointF( 0, 0),  lb );
			painter->restore();
			count++;
		}
	}

	painter->save();
	painter->setPen( QPen(QColor("navy")));
	painter->drawLine(QPoint(m_boundingRect.width(), m_boundingRect.height()-marginY), QPoint(0, m_boundingRect.height()-marginY));
	painter->drawLine(QPoint(marginX , m_boundingRect.height() ), QPoint(marginX, 0));
	painter->restore();
}
/**
 * Calcula el valor maximo de las barras
 */
void Histogram::calculateMaxValue()
{
	m_maxValue = 0;
	foreach(Bar value, m_bars)
	{
		if(m_maxValue < value.value())
		{
			m_maxValue = value.value();
		}
	}
}

/**
 * asigna un valor a las barras del histograma
*/
void  Histogram::setBars(QVector<Bar> bars)
{
	m_bars = bars;
	calculateMaxValue();
	QSize hint = sizeHint();
	dDebug()  << hint;
	if(m_boundingRect.size().height() < hint.height()  && m_boundingRect.size().width() < hint.width())
	{
		m_boundingRect.setSize(hint);
	}
}

/**
 * Asigna los valore de las barras, donde la llave del map es el titulo y el entero es el valor de la barra
*/
void Histogram::setValues( const QMap<QString, int> & values )
{
	m_bars.clear();
	QMap<QString, int>::const_iterator it = values.begin();
	while(it != values.end())
	{
		m_bars << Bar(it.value(), it.key(), Qt::red);
		++it;
	}
	calculateMaxValue();
	emit requestUpdate();
}

/**
 * Retorna el tamaño requerido para el histograma 
*/
QSize Histogram::sizeHint(const QFontMetrics & fm)
{
	calculateMaxValue();
	double s = 1;
	if(m_maxValue != 0)
	{
		s = (double)(m_boundingRect.height()-20) /((double)(m_maxValue));
	}
	int spaceX = m_boundingRect.width()/(m_bars.count()+2);
	if(spaceX <  fm.boundingRect("1").height() +3)
	{
		int spaceX =  fm.boundingRect("1").height();
	}
	return QSize(((m_bars.count()+1)*spaceX)+50, m_maxValue*s);
}

/**
 * Retorna un string con el histograma en formato xml
*/
QString Histogram::toString() const
{
	QDomDocument doc;
	QDomElement root = doc.createElement ( "Histogram" );
	doc.appendChild(root);
	
	QDomElement listBarsE = doc.createElement( "Bars" );
	listBarsE.setAttribute("size", m_bars.count());
	root.appendChild(listBarsE);
	
	foreach(Bar bar, m_bars)
	{
		QDomElement barE = doc.createElement( "Bar" );
		barE.setAttribute("color", bar.color().name());
		barE.setAttribute("label", bar.label());
		barE.setAttribute("value", bar.value());
		listBarsE.appendChild(barE);
	}
	return doc.toString();
}

///--------------------------
HistogramFactory::HistogramFactory(): QXmlDefaultHandler()
{
}


HistogramFactory::~HistogramFactory()
{

}

bool HistogramFactory::startElement(const QString& , const QString& , const QString& qname, const QXmlAttributes& atts)
{
	if (!m_isParsing)
	{
		m_isParsing = true;
	}
	
	dDebug() << qname;
	if( qname == "Histogram")
	{
		m_histogram = new Histogram(QRect(QPoint(0,0), QSize(500,500)));
	}
	else if( qname == "Bars")
	{
// 		m_bars.resize(atts.value("size").toInt());
	}
	else if( qname == "Bar")
	{
		m_bars << Bar( atts.value("value").toInt(), atts.value("label"), QColor(atts.value("color")) );
	}
	m_qname = qname;
	
	return true;
}

bool HistogramFactory::endElement( const QString& ns, const QString& localname, const QString& qname)
{
	if( qname == "Bars")
	{
		m_histogram->setBars(m_bars);
	}
	return true;
}



bool HistogramFactory::characters ( const QString & ch )
{
	return true;
}

bool HistogramFactory::error ( const QXmlParseException & exception )
{
	dError() << exception.lineNumber() << "x" << exception.columnNumber() << ": " << exception.message();
	
	return true;
}

bool HistogramFactory::fatalError ( const QXmlParseException & exception )
{
 	dFatal() << exception.lineNumber() << "x" << exception.columnNumber() << ": " << exception.message();
	
	return true;
}

Histogram *HistogramFactory::build(const QString &document)
{
	dDebug() << "Builds from: " << document;
	
	QXmlSimpleReader m_reader;
	m_reader.setContentHandler(this);
	m_reader.setErrorHandler(this);
	
	QXmlInputSource xmlsource;
	xmlsource.setData(document);
	
	if( m_reader.parse(&xmlsource) )
	{
		return m_histogram;
	}
	
	return 0;
}

