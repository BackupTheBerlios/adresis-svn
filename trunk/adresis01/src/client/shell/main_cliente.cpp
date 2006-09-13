#include <QApplication>
#include <QMessageBox>

#include <dconfig.h>
#include <dapplicationproperties.h> // dAppProp
#include <ddebug.h>
#include <dapplication.h>
#include "admainwindow.h"
#include "adcapplication.h"


int main(int argc, char **argv)
{
	ADCApplication app(argc, argv);
	
	app.setStyle("plastique");
	
// 	app.setPalette(app.style()->standardPalette());
	app.setApplicationName("adresis_client");
	
	DCONFIG->beginGroup("General");
	
	dAppProp->setHomeDir( DCONFIG->value("Home", "/").toString() );
	dAppProp->setCacheDir( DCONFIG->value("Repository", "/").toString() );
	
	QString home = dAppProp->homeDir();
	if ( home == "/" || home.isEmpty() )
	{
		if (! app.firstRun() )
		{
			QMessageBox::critical(0, app.applicationName(), QObject::tr("Please configure the application first!"));
			
			return 0;
		}
		else
		{
			dAppProp->setHomeDir( DCONFIG->value("Home", "/").toString() );
			dAppProp->setCacheDir( DCONFIG->value("Repository", "/").toString() );
		}
	}
	
	dAppProp->setThemeDir(dAppProp->homeDir()+"/data/themes/default");
	
	ADMainWindow mainWindow;
// 	mainWindow.show();
	mainWindow.showMaximized();
	
	return app.exec();
}

