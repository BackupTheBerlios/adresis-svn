/***************************************************************************
 *   Copyright (C) 2004 by Alexander Dymo                                  *
 *   adymo@kdevelop.org                                                    *
 *   David Cuadrado (C) 2005 						   *
 *   krawek@gmail.com							   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QPixmap>

static char * new_xpm[] = {
"12 16 6 1",
" 	c #040404",
".	c None",
"X	c white",
"o	c #808304",
"O	c black",
"+	c #f3f7f3",
"       .....",
" XXXXX  ....",
" XXXXX X ...",
" XXXXX XX ..",
" XooXX    O.",
" X+XXX+XXXO.",
" XXXXXXXXXO.",
" XoooXooXXO.",
" XXXXXXXXXO.",
" XXXXXXXXXO.",
" XoXXoooXXO.",
" XXXXXXXXXO.",
"OXXXXXXXXXO.",
"OXXXXXXXXXO.",
"OOOOOOOOOOO.",
"............"};


#include "button.h"

#include <QPainter>
#include <QToolTip>
#include <QStyle>
#include <QApplication>
#include <QStyleOption>
#include <QStylePainter>

#include "buttonbar.h"

namespace Ideal {
	
	Button::Button(ButtonBar *parent, const QString text, const QIcon &icon,
		       const QString &description)
	: QPushButton(icon, text, parent), m_buttonBar(parent), m_description(description),
	m_place(parent->place()), m_realText(text), m_realIconSet(icon), m_isSensible(false), m_haveIcon(true)
	{
		hide();
		setFlat(true);
		setCheckable(true);
		setFocusPolicy(Qt::NoFocus);
		setDescription(m_description);
		setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
		resize(sizeHint());
		fixDimensions(Ideal::Bottom);
    
		setToolTip(m_realText);
		
		m_animation = new Animation(this);
		
		connect( m_animation->timer, SIGNAL( timeout() ), this, SLOT( animate() ) );
	}

	Button::~Button()
	{
		delete m_animation;
	}
	
	void Button::enterEvent( QEvent* )
	{
		if ( m_isSensible && !isChecked() && ! isDown())
		{
			m_isSensible = false;
			
			setChecked(true);
			setDown(true);
			emit click();
			
			QTimer::singleShot(300, this, SLOT(toggleSensibility()));
		}
		
		m_animation->isEnter = true;
		m_animation->count = 1;

		m_animation->start();
	}

	void Button::leaveEvent( QEvent* )
	{
		if ( m_animation->count == 0 )
		{
			m_animation->count = 1;
		}
		m_animation->isEnter = false;
		m_animation->timer->start();
	}
	
	void Button::animate()
	{
		if ( m_animation->isEnter ) 
		{
			m_animation->count += 1;
			update( );
			if ( m_animation->count > m_animation->MAXCOUNT )
			{
				m_animation->timer->stop();
			}
		} else 
		{
			m_animation->count -= 1;
			update( );
			if ( m_animation->count < 1 )
			{
				m_animation->timer->stop();
			}
		}
	}

	void Button::setDescription(const QString &description)
	{
		m_description = description;
		setToolTip(m_description);
	}

	QString Button::description() const
	{
		return m_description;
	}
	
	QStyleOptionButton Button::styleOption() const
	{
		QStyleOptionButton opt;
		opt.init(this);

		if (isEnabled())
			opt.state |= QStyle::State_Enabled;
		if (hasFocus())
			opt.state |= QStyle::State_HasFocus;
		if (isDown())
			opt.state |= QStyle::State_Sunken;
// 		if (isOn())
// 			opt.state |= QStyle::State_On;
		if (! isFlat() && ! isDown())
			opt.state |= QStyle::State_Raised;
		opt.features = isDefault() ? QStyleOptionButton::DefaultButton : QStyleOptionButton::None;

		opt.text = text();

		if ( m_haveIcon )
		{
			opt.icon = m_realIconSet;
			opt.iconSize = QSize(16,16);
		}
		
		QRect r = rect();
		if (m_place == Ideal::Left || m_place == Ideal::Right)
			r.setSize(QSize(r.height(), r.width()));

		opt.rect = r;

		return opt;
	}

	void Button::paintEvent(QPaintEvent *)
	{
		QPainter p(this);
		
		/////
		QColor fillColor, textColor;
		
		if ( isDown() || isChecked() )
		{
			fillColor = m_animation->blendColors( palette().highlight().color(), palette().background().color(), static_cast<int>( m_animation->count * 3.5 ) );
			textColor = m_animation->blendColors( palette().highlightedText().color(), palette().text().color(), static_cast<int>( m_animation->count * 4.5 ) );
		}
		else
		{
			fillColor = m_animation->blendColors( palette().background().color(), palette().highlight().color(), static_cast<int>( m_animation->count * 3.5 ) );
			textColor = m_animation->blendColors( palette().text().color(), palette().highlightedText().color(), static_cast<int>( m_animation->count * 4.5 ) );
		}
		////
		
		QStyleOptionButton opt = styleOption();
		QRect r = opt.rect;

		QPixmap pm(r.width(), r.height());
		pm.fill(fillColor);
		p.setPen( QPen(palette().mid(), 1) );
		
		opt.palette.setColor(QPalette::Button, fillColor);
		opt.palette.setColor(QPalette::ButtonText, textColor);

		QStylePainter p2(&pm, this);
		p2.drawControl(QStyle::CE_PushButton, opt);
		
		
		switch (m_place)
		{
			case Ideal::Left:
				p.rotate(-90);
				p.drawPixmap(1-pm.width(), 0, pm);
				break;
			case Ideal::Right:
				p.rotate(90);
				p.drawPixmap(0, 1-pm.height(), pm);
				break;
			default:
				p.drawPixmap(0, 0, pm);
				break;
		}
		
		p.end();
	}
	
	ButtonMode Button::mode()
	{
		return m_buttonBar->mode();
	}

	void Button::setPlace(Ideal::Place place)
	{
		Place oldPlace = m_place;
		m_place = place;
		fixDimensions(oldPlace);
	}

	void Button::fixDimensions(Place oldPlace)
	{
		switch (m_place)
		{
			case Ideal::Left:
			case Ideal::Right:
				if ((oldPlace == Ideal::Bottom) || (oldPlace == Ideal::Top))
				{
					setFixedWidth(height());
					setMinimumHeight(sizeHint().width());
					setMaximumHeight(32767);
				}
				break;
			case Ideal::Top:
			case Ideal::Bottom:
				if ((oldPlace == Ideal::Left) || (oldPlace == Ideal::Right))
				{
					setFixedHeight(width());
					setMinimumWidth(sizeHint().height());
					setMaximumWidth(32767);
				}
				break;
		}
	}

	QSize Button::sizeHint() const
	{
		return sizeHint(text());
	}

	QSize Button::sizeHint(const QString &text) const
	{
// 		constPolish();
		QStyleOptionButton option = styleOption();
		int w = option.iconSize.width(), h = option.iconSize.height();

		if ( !icon().isNull() && m_buttonBar->mode() != Text ) 
		{
			int iw = option.iconSize.width();
			int ih = option.iconSize.height();
			w += iw;
			h = qMax( h, ih );
		}
		if (m_buttonBar->mode() != Icons) 
		{
			QString s( text );
			bool empty = s.isEmpty();
			if ( empty )
				s = QLatin1String("XXXX");
			QFontMetrics fm = fontMetrics();
			QSize sz = fm.size( Qt::TextShowMnemonic, s );
			if(!empty || !w)
				w += sz.width();
			if(!empty || !h)
				h = qMax(h, sz.height());
		}

		return (style()->sizeFromContents(QStyle::CT_ToolButton, &option, QSize(w, h), this).expandedTo(QApplication::globalStrut()));
	}

	void Ideal::Button::updateSize()
	{
		switch (m_place)
		{
			case Ideal::Left:
			case Ideal::Right:
				setMinimumHeight(sizeHint().width());
				resize(sizeHint().height(), sizeHint().width());
				break;
			case Ideal::Top:
			case Ideal::Bottom:
				resize(sizeHint().width(), sizeHint().height());
				break;
		}
	}

	QString Button::realText() const
	{
		return m_realText;
	}

	void Button::setMode(Ideal::ButtonMode mode)
	{
		switch (mode)
		{
			case Text:
				disableIconSet();
				enableText();
				break;
			case IconsAndText:
				enableIconSet();
				enableText();
				break;
			case Icons:
				disableText();
				enableIconSet();
				break;
		}
		updateSize();
	}

	void Button::enableIconSet()
	{
		if (!icon().isNull() )
		{
			if (m_realIconSet.isNull())
			{
           		 	m_realIconSet = QIcon((const char**)new_xpm);
			}
			setIcon(m_realIconSet);
		}
		
		m_haveIcon = true;
	}

	void Button::disableIconSet()
	{
		m_haveIcon = false;
		setIcon(QIcon());
	}

	void Button::disableText()
	{
		if (text().length() > 0)
		{
			setText("");
		}
	}

	void Button::enableText()
	{
		setText(m_realText);
	}
	
	void Button::toggleSensibility()
	{
		m_isSensible = !m_isSensible;
	}
}

