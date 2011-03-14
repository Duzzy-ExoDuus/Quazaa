/*
** widgetchat.h
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

#ifndef WIDGETCHAT_H
#define WIDGETCHAT_H

#include <QWidget>
#include "widgetchatmiddle.h"

namespace Ui
{
	class WidgetChat;
}

class WidgetChat : public QWidget
{
	Q_OBJECT
public:
	WidgetChat(QWidget* parent = 0);
	~WidgetChat();
	WidgetChatMiddle* panelChatMiddle;
	void saveWidget();

signals:
	void changeRoom(int index);

protected:
	void changeEvent(QEvent* e);

private:
	Ui::WidgetChat* ui;

private slots:
	void on_toolButtonChatRoomRemove_clicked();
	void on_listViewChatRooms_pressed(QModelIndex index);
	void on_splitterChat_customContextMenuRequested(QPoint pos);
	void updateUserList(WidgetChatRoom* currentTab);
	void on_toolButtonChatRoomAdd_clicked();
	void updateUserCount(int operators, int users);
};

#endif // WIDGETCHAT_H
