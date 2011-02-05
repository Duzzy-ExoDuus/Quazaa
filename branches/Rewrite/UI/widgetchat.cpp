//
// widgetchat.cpp
//
// Copyright © Quazaa Development Team, 2009-2010.
// This file is part of QUAZAA (quazaa.sourceforge.net)
//
// Quazaa is free software; you can redistribute it
// and/or modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3 of
// the License, or (at your option) any later version.
//
// Quazaa is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Quazaa; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#include "widgetchat.h"
#include "ui_widgetchat.h"

#include "quazaasettings.h"
 
#include "systemlog.h"

WidgetChat::WidgetChat(QWidget* parent) :
	QWidget(parent),
	ui(new Ui::WidgetChat)
{
	ui->setupUi(this);
	panelChatMiddle = new WidgetChatMiddle();
	ui->splitterChat->restoreState(quazaaSettings.WinMain.ChatSplitter);
	ui->verticalLayoutChatMiddle->addWidget(panelChatMiddle);
	ui->toolButtonChatFriendsHeader->setChecked(quazaaSettings.WinMain.ChatFriendsTaskVisible);
	ui->toolButtonChatRoomsHeader->setChecked(quazaaSettings.WinMain.ChatRoomsTaskVisible);
	ui->listViewChatRooms->setModel(panelChatMiddle->channelListModel);
	connect(panelChatMiddle, SIGNAL(roomChanged(WidgetChatRoom*)), this, SLOT(updateUserList(WidgetChatRoom*)));
	connect(this, SIGNAL(changeRoom(int)), panelChatMiddle, SLOT(changeRoom(int)));
	connect(panelChatMiddle, SIGNAL(updateUserCount(int,int)), this, SLOT(updateUserCount(int,int)));
}

WidgetChat::~WidgetChat()
{
	delete ui;
}

void WidgetChat::changeEvent(QEvent* e)
{
	QWidget::changeEvent(e);
	switch(e->type())
	{
		case QEvent::LanguageChange:
			ui->retranslateUi(this);
			break;
		default:
			break;
	}
}

void WidgetChat::saveWidget()
{
	quazaaSettings.WinMain.ChatSplitter = ui->splitterChat->saveState();
	quazaaSettings.WinMain.ChatFriendsTaskVisible = ui->toolButtonChatFriendsHeader->isChecked();
	quazaaSettings.WinMain.ChatRoomsTaskVisible = ui->toolButtonChatRoomsHeader->isChecked();
	panelChatMiddle->saveWidget();
}

void WidgetChat::on_splitterChat_customContextMenuRequested(QPoint pos)
{
	Q_UNUSED(pos);

	if(ui->splitterChat->handle(1)->underMouse())
	{
		if(ui->splitterChat->sizes()[0] > 0)
		{
			quazaaSettings.WinMain.ChatSplitterRestoreLeft = ui->splitterChat->sizes()[0];
			quazaaSettings.WinMain.ChatSplitterRestoreMiddle = ui->splitterChat->sizes()[1];
			QList<int> newSizes;
			newSizes.append(0);
			newSizes.append(ui->splitterChat->sizes()[0] + ui->splitterChat->sizes()[1]);
			newSizes.append(ui->splitterChat->sizes()[2]);
			ui->splitterChat->setSizes(newSizes);
		}
		else
		{
			QList<int> sizesList;
			sizesList.append(quazaaSettings.WinMain.ChatSplitterRestoreLeft);
			sizesList.append(quazaaSettings.WinMain.ChatSplitterRestoreMiddle);
			sizesList.append(ui->splitterChat->sizes()[2]);
			ui->splitterChat->setSizes(sizesList);
		}
	}

	if(ui->splitterChat->handle(2)->underMouse())
	{
		if(ui->splitterChat->sizes()[2] > 0)
		{
			quazaaSettings.WinMain.ChatSplitterRestoreMiddle = ui->splitterChat->sizes()[1];
			quazaaSettings.WinMain.ChatSplitterRestoreRight = ui->splitterChat->sizes()[2];
			QList<int> newSizes;
			newSizes.append(ui->splitterChat->sizes()[0]);
			newSizes.append(ui->splitterChat->sizes()[1] + ui->splitterChat->sizes()[2]);
			newSizes.append(0);
			ui->splitterChat->setSizes(newSizes);
		}
		else
		{
			QList<int> sizesList;
			sizesList.append(ui->splitterChat->sizes()[0]);
			sizesList.append(quazaaSettings.WinMain.ChatSplitterRestoreMiddle);
			sizesList.append(quazaaSettings.WinMain.ChatSplitterRestoreRight);
			ui->splitterChat->setSizes(sizesList);
		}
	}
}

void WidgetChat::updateUserList(WidgetChatRoom* currentTab)
{
	//qDebug() << "Signal successful. In WidgetChat::updateUserList()";
	ui->listViewChatUsers->setModel(currentTab->userList);
	updateUserCount(currentTab->operators, currentTab->users);
}

void WidgetChat::on_listViewChatRooms_pressed(QModelIndex index)
{
	switch (QApplication::mouseButtons ())
	{
	case Qt::LeftButton:
		emit changeRoom(index.row());
		break;
	case Qt::RightButton:
		break;
	default:
		break;
	}
}


void WidgetChat::updateUserCount(int operators, int users)
{
	ui->labelChatUsersCount->setText(tr("%1 Operators, %2 Total").arg(operators).arg(users));
}
