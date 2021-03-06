/***************************************************************************
 *   Copyright (C) 2004 by David Cuadrado                                  *
 *   krawek@toonka.com                                                    *
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

#include <qfile.h>
#include <qtextstream.h>
#include <qtextcodec.h>

#include "ktpreferences.h"


#include "dglobal.h"
#include "ddebug.h"
#include "dapplication.h"

#include <QLabel>
#include <QLineEdit>
#include <QComboBox>

#include "dformfactory.h"


class KTPreferences::GeneralPage : public QWidget
{
	Q_OBJECT
	public:
		GeneralPage();
		~GeneralPage();
		void saveValues();
		
// 		KToon::RenderType m_renderType;
		
	private:
		QLineEdit *m_home, *m_repository, *m_browser;
		QComboBox *m_renderType;
		
		
// 	private slots:
// 		void syncRenderType(int index);
};

KTPreferences::GeneralPage::GeneralPage()
{
	QVBoxLayout *layout = new QVBoxLayout(this);
	DCONFIG->beginGroup("General");
	
	m_home = new QLineEdit;
	QString str = DCONFIG->value("Home").toString();
	if ( !str.isEmpty())
	{
		m_home->setText(str);
	}
	
	m_repository = new QLineEdit;
	str = DCONFIG->value("Cache").toString();
	if ( !str.isEmpty())
	{
		m_repository->setText(str);
	}
	
	m_browser = new QLineEdit;
	str = DCONFIG->value("Browser").toString();
	if ( !str.isEmpty())
	{
		m_browser->setText(str);
	}
	
	QLayout *form = DFormFactory::makeGrid( QStringList() << tr("Home") << tr("Cache") << tr("Browser") , QWidgetList() << m_home << m_repository << m_browser );
	
	layout->addLayout(form);
	
	layout->addStretch(3);
}

KTPreferences::GeneralPage::~GeneralPage()
{
}

void KTPreferences::GeneralPage::saveValues()
{
	DCONFIG->beginGroup("General");
	
	QString str = m_home->text();
	if ( !str.isEmpty() && m_home->isModified ())
	{
		DCONFIG->setValue("Home", str);
	}
	
	str = m_repository->text();
	if ( !str.isEmpty() && m_repository->isModified () )
	{
		DCONFIG->setValue("Cache", str);
	}
	
	str = m_browser->text();
	if ( !str.isEmpty() && m_browser->isModified () )
	{
		DCONFIG->setValue("Browser", str);
	}
}

class KTPreferences::FontPage : public QWidget
{
	public:
		FontPage();
		~FontPage();
		
		QFont currentFont() const;
		
	private:
		DFontChooser *m_fontChooser;
};

KTPreferences::FontPage::FontPage()
{
	m_fontChooser = new DFontChooser(this);
	m_fontChooser->setCurrentFont(font());
}

KTPreferences::FontPage::~FontPage()
{
}


QFont KTPreferences::FontPage::currentFont() const
{
	return m_fontChooser->currentFont();
}


#include "ktpreferences.moc"

//--------------- CONSTRUCTOR --------------------

KTPreferences::KTPreferences( QWidget *parent ) : DConfigurationDialog(parent )
{
	setWindowTitle( tr( "Application KTPreferences" ) );
	
	m_generalPage = new GeneralPage;
	addPage(m_generalPage, tr("General"))->setIcon(QPixmap(THEME_DIR+"/icons/general_config.png"));;
	
	m_themeSelector = new KTThemeSelector;
	addPage(m_themeSelector, tr("Theme preferences"))->setIcon( QPixmap(THEME_DIR+"/icons/theme_config.png") );
	
	
	
	m_fontChooser = new FontPage;
	addPage(m_fontChooser, tr("Font"))->setIcon(QPixmap(THEME_DIR+"/icons/font_config.png"));
	
	resize(400,400);
}

//-------------- DESTRUCTOR -----------------

KTPreferences::~KTPreferences()
{
}

void KTPreferences::ok()
{
	apply();
	DConfigurationDialog::ok();
}

void KTPreferences::apply()
{
	if ( static_cast<KTThemeSelector *>(currentPage()->widget()) ==  m_themeSelector)
	{
		if(m_themeSelector->applyColors() )
		{
			dApp->applyTheme(m_themeSelector->document());
		}
	}
	else if ( static_cast<GeneralPage *>( currentPage()->widget()) == m_generalPage )
	{
		m_generalPage->saveValues();
	}
	else if ( qobject_cast<FontPage *>(currentPage()->widget() ) == m_fontChooser )
	{
		dApp->setFont(m_fontChooser->currentFont());
	}
}



