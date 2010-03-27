# -------------------------------------------------
# Project created by QtCreator 2009-12-02T17:28:49
# -------------------------------------------------
QT += network \
	sql \
	script \
	svg \
	webkit \
	xml \
	xmlpatterns \
	phonon
TARGET = Quazaa
CONFIG(debug, debug|release) {
	mac:TARGET = $$join(TARGET,,,_debug)
	win32:TARGET = $$join(TARGET,,,d)
}
INCLUDEPATH += vlcmediaplayer
win32:LIBS += -L./bin # if you are at windows os
LIBS += -lvlc
TEMPLATE = app
SOURCES += main.cpp \
	mainwindow.cpp \
	dialoglanguage.cpp \
	quazaasettings.cpp \
	dialogsplash.cpp \
	widgetsearchtemplate.cpp \
	dialogwizard.cpp \
	dialogtransferprogresstooltip.cpp \
	dialogtorrentproperties.cpp \
	dialogselectvisualisation.cpp \
	dialogsecuritysubscriptions.cpp \
	dialogscheduler.cpp \
	dialogprofile.cpp \
	dialogpreviewprepare.cpp \
	dialogopentorrent.cpp \
	dialoghashprogress.cpp \
	dialoggplview.cpp \
	dialogfiltersearch.cpp \
	dialogeditshares.cpp \
	dialogdownloadsimport.cpp \
	dialogdownloadproperties.cpp \
	dialogdownloadmonitor.cpp \
	dialogcreatetorrent.cpp \
	dialogclosetype.cpp \
	dialogaddsecuritysubscription.cpp \
	dialogaddrule.cpp \
	dialogadddownload.cpp \
	dialogabout.cpp \
	dialogsettings.cpp \
	commonfunctions.cpp \
	dialoglibrarysearch.cpp \
	QSkinDialog/qskinsettings.cpp \
	QSkinDialog/qskindialog.cpp \
	QSkinDialog/dialogskinpreview.cpp \
	qtsingleapplication/src/qtsingleapplication.cpp \
	qtsingleapplication/src/qtsinglecoreapplication.cpp \
	qtsingleapplication/src/qtlockedfile_win.cpp \
	qtsingleapplication/src/qtlockedfile_unix.cpp \
	qtsingleapplication/src/qtlockedfile.cpp \
	qtsingleapplication/src/qtlocalpeer.cpp \
	phononmediaplayer.cpp \
	vlcmediaplayer.cpp \
	quazaairc.cpp
HEADERS += mainwindow.h \
	dialoglanguage.h \
	quazaasettings.h \
	quazaaglobals.h \
	dialogsplash.h \
	widgetsearchtemplate.h \
	dialogwizard.h \
	dialogtransferprogresstooltip.h \
	dialogtorrentproperties.h \
	dialogsettings.h \
	dialogselectvisualisation.h \
	dialogsecuritysubscriptions.h \
	dialogscheduler.h \
	dialogprofile.h \
	dialogpreviewprepare.h \
	dialogopentorrent.h \
	dialoghashprogress.h \
	dialoggplview.h \
	dialogfiltersearch.h \
	dialogeditshares.h \
	dialogdownloadsimport.h \
	dialogdownloadproperties.h \
	dialogdownloadmonitor.h \
	dialogcreatetorrent.h \
	dialogclosetype.h \
	dialogaddsecuritysubscription.h \
	dialogaddrule.h \
	dialogadddownload.h \
	dialogabout.h \
	commonfunctions.h \
	dialoglibrarysearch.h \
	QSkinDialog/qskinsettings.h \
	QSkinDialog/qskindialog.h \
	QSkinDialog/dialogskinpreview.h \
	qtsingleapplication/src/qtsingleapplication.h \
	qtsingleapplication/src/qtlockedfile.h \
	qtsingleapplication/src/qtlocalpeer.h \
	qtsingleapplication/src/qtsinglecoreapplication.h \
	phononmediaplayer.h \
	vlcmediaplayer.h \
	quazaairc.h
FORMS += mainwindow.ui \
	dialoglanguage.ui \
	dialogsplash.ui \
	widgetsearchtemplate.ui \
	dialogwizard.ui \
	dialogtransferprogresstooltip.ui \
	dialogtorrentproperties.ui \
	dialogsettings.ui \
	dialogselectvisualisation.ui \
	dialogsecuritysubscriptions.ui \
	dialogscheduler.ui \
	dialogprofile.ui \
	dialogpreviewprepare.ui \
	dialogopentorrent.ui \
	dialoghashprogress.ui \
	dialoggplview.ui \
	dialogfiltersearch.ui \
	dialogeditshares.ui \
	dialogdownloadsimport.ui \
	dialogdownloadproperties.ui \
	dialogdownloadmonitor.ui \
	dialogcreatetorrent.ui \
	dialogclosetype.ui \
	dialogaddsecuritysubscription.ui \
	dialogaddrule.ui \
	dialogadddownload.ui \
	dialogabout.ui \
	dialoglibrarysearch.ui \
	mediasettings.ui \
	QSkinDialog/qskindialog.ui \
	QSkinDialog/dialogskinpreview.ui
TRANSLATIONS = quazaa_af.ts \
	quazaa_ar.ts \
	quazaa_ca.ts \
	quazaa_chs.ts \
	quazaa_cz.ts \
	quazaa_de.ts \
	quazaa_default_en.ts \
	quazaa_ee.ts \
	quazaa_es.ts \
	quazaa_es-mx.ts \
	quazaa_fi.ts \
	quazaa_fr.ts \
	quazaa_gr.ts \
	quazaa_heb.ts \
	quazaa_hr.ts \
	quazaa_hu.ts \
	quazaa_it.ts \
	quazaa_ja.ts \
	quazaa_lt.ts \
	quazaa_nl.ts \
	quazaa_no.ts \
	quazaa_pt.ts \
	quazaa_pt-br.ts \
	quazaa_ru.ts \
	quazaa_sl-si.ts \
	quazaa_sq.ts \
	quazaa_sr.ts \
	quazaa_sv.ts \
	quazaa_tr.ts \
	quazaa_tw.ts
RESOURCES += Resource.qrc
RC_FILE = Quazaa.rc
OTHER_FILES += LICENSE.GPL3
DESTDIR = ./bin
