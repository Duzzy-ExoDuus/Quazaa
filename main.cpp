/*
** main.cpp
**
** Copyright © Quazaa Development Team, 2009-2011.
** This file is part of QUAZAA (quazaa.sourceforge.net)
**
** Quazaa is free software; this file may be used under the terms of the GNU
** General Public License version 3.0 or later as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Quazaa is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
**
** Please review the following information to ensure the GNU General Public
** License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** You should have received a copy of the GNU General Public License version
** 3.0 along with Quazaa; if not, write to the Free Software Foundation,
** Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <QtGui/QApplication>
#include <QSettings>
#include <QtCore>
#include <QObject>

#include "winmain.h"
#include "quazaaglobals.h"
#include "quazaasettings.h"
#include "dialoglanguage.h"
#include "qtsingleapplication/src/QtSingleApplication"

#include "geoiplist.h"
#include "network.h"
#include "queryhashmaster.h"
#include "dialogsplash.h"
#include "dialoglanguage.h"
#include "wizardquickstart.h"
#include "sharemanager.h"
#include "quazaairc.h"
#include "commonfunctions.h"

#ifdef Q_OS_LINUX
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#endif

QuazaaGlobals quazaaGlobals;

int main(int argc, char *argv[])
{
	QtSingleApplication theApp(argc, argv);

	// Check if the application is already running
	if(theApp.sendMessage("Show App"))
	{
		return 0;
	}

	qsrand(time(0));

#ifdef Q_OS_LINUX

	rlimit sLimit;
	memset(&sLimit, 0, sizeof(rlimit));
	getrlimit(RLIMIT_NOFILE, &sLimit);

	sLimit.rlim_cur = sLimit.rlim_max;

	if( setrlimit(RLIMIT_NOFILE, &sLimit) == 0 )
	{
		qDebug() << "Successfully raised resource limits";
	}
	else
	{
		qDebug() << "Cannot set resource limits";
	}

#endif

        theApp.setApplicationName(QuazaaGlobals::APPLICATION_NAME());
        theApp.setApplicationVersion(QuazaaGlobals::APPLICATION_VERSION_STRING());
        theApp.setOrganizationDomain(QuazaaGlobals::APPLICATION_ORGANIZATION_DOMAIN());
        theApp.setOrganizationName(QuazaaGlobals::APPLICATION_ORGANIZATION_NAME());

	//Initialize multilanguage support
	quazaaSettings.loadLanguageSettings();

	if(quazaaSettings.FirstRun())
	{
		DialogLanguage* dlgLanguage = new DialogLanguage();
		dlgLanguage->exec();
	}

        quazaaSettings.translator.load(quazaaSettings.Language.File);
        qApp->installTranslator(&quazaaSettings.translator);

	//Create splash window
	DialogSplash* dlgSplash = new DialogSplash();
	dlgSplash->show();

	dlgSplash->updateProgress(1, QObject::tr("Loading settings..."));
	qApp->processEvents();

	//Initialize Settings
	quazaaSettings.loadSettings();

	//Check if this is Quazaa's first run
	dlgSplash->updateProgress(5, QObject::tr("Checking for first run..."));
	qApp->processEvents();
	if(quazaaSettings.FirstRun())
	{
		dlgSplash->updateProgress(10, QObject::tr("Running first run wizard..."));
		quazaaSettings.saveFirstRun(false);
		quazaaSettings.saveSettings();
		quazaaSettings.saveProfile();

		WizardQuickStart* wzrdQuickStart = new WizardQuickStart();
		wzrdQuickStart->exec();
	}

	//Load profile
	dlgSplash->updateProgress(15, QObject::tr("Loading Profile..."));
	qApp->processEvents();
	quazaaSettings.loadProfile();

	//Load the networks
	dlgSplash->updateProgress(25, QObject::tr("Loading Networks..."));
	qApp->processEvents();

	//initialize geoip list
	GeoIP.loadGeoIP();

	//Load the library
	dlgSplash->updateProgress(50, QObject::tr("Loading Library..."));
	qApp->processEvents();
	QueryHashMaster.Create();
	ShareManager.Start();

	dlgSplash->updateProgress(80, QObject::tr("Loading User Interface..."));
	qApp->processEvents();

	MainWindow = new WinMain();
	if(quazaaSettings.WinMain.Visible)
	{
		MainWindow->show();
	}



	dlgSplash->updateProgress(90, QObject::tr("Loading Tray Icon..."));
	qApp->processEvents();
	MainWindow->loadTrayIcon();

	// Make the main window show if the user tried to open another instance
	QObject::connect(&theApp, SIGNAL(messageReceived(const QString&)),
					 MainWindow, SLOT(show()));
	theApp.setActivationWindow(MainWindow);
	QObject::connect(MainWindow, SIGNAL(Show()), &theApp, SLOT(activateWindow()));

	dlgSplash->updateProgress(100, QObject::tr("Welcome to Quazaa!"));
	qApp->processEvents();
	dlgSplash->close();
	dlgSplash->deleteLater();
	dlgSplash = 0;

	// Start networks if needed
	if(quazaaSettings.Gnutella2.Enable)
	{
		Network.Connect();
	}

	return theApp.exec();
}
