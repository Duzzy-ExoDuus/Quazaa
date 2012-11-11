/*
** widgetchatmiddle.h
**
** Copyright © Quazaa Development Team, 2009-2012.
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

#ifndef WIDGETCHATCENTER_H
#define WIDGETCHATCENTER_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QToolButton>
#include <QTextDocument>

#include "maintabwidget.h"

struct ConnectionInfo;

namespace Ui
{
	class WidgetIrcMain;
}

class WidgetIrcMain : public QMainWindow
{
	Q_OBJECT
public:
	WidgetIrcMain(QWidget* parent = 0);
	~WidgetIrcMain();

protected:
	void changeEvent(QEvent* e);

private:
	Ui::WidgetIrcMain* ui;
	MainTabWidget* tabWidgetMain;

signals:
	void msgTabChanged(SessionTabWidget*);
	void disconnected();
	void showFriends();

public slots:
	void saveWidget();
	void connectTo(const QString& host = QString(), quint16 port = 6667,
				   const QString& nick = QString(), const QString& password = QString());
	void connectTo(const ConnectionInfo &connection);
	void connectToImpl(const ConnectionInfo& connection);
	SessionTabWidget* currentSession();

private slots:
	void on_actionChatSettings_triggered();
	void on_actionEditMyProfile_triggered();
	void initialize();
	void disconnect();
	void tabActivated(int index);
	void createWelcomeView();
	void createTabbedView();
	void onNewTabRequested();
	void setSkin();
	void msgTabChanged(int index);

};

#endif // WIDGETCHATCENTER_H
