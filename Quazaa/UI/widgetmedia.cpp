/*
** $Id$
**
** Copyright © Quazaa Development Team, 2009-2013.
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

#include "widgetmedia.h"
#include "ui_widgetmedia.h"

#include "quazaasettings.h"
#include "skinsettings.h"

#include "debug_new.h"

#include <QDebug>
#include <QtMultimedia>

#include <QFileDialog>

// The Media Tab's Base Widget

CWidgetMedia::CWidgetMedia(QWidget* parent) :
	QWidget(parent),
	videoWidget(0),
	ui(new Ui::CWidgetMedia)
{
	ui->setupUi(this);

	ui->splitterMedia->restoreState(quazaaSettings.WinMain.MediaSplitter);

	player = new QMediaPlayer(this);
	// owned by PlaylistModel
	playlist = new QMediaPlaylist();
	player->setPlaylist(playlist);

	videoWidget = new VideoWidget(ui->videoContainerWidget);
	player->setVideoOutput(videoWidget);
	videoWidget->raiseControls(); // This corrects the controls being put under the video widget during setVideoWidget

	ui->verticalLayoutMedia->addWidget(videoWidget);


	setSkin();
}

CWidgetMedia::~CWidgetMedia()
{
	delete ui;
}

void CWidgetMedia::changeEvent(QEvent* e)
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

void CWidgetMedia::saveWidget()
{
	quazaaSettings.WinMain.MediaSplitter = ui->splitterMedia->saveState();
}

/*void WidgetMedia::on_actionMediaOpen_triggered()
{
	mediaPlayer->openFile();
}

void WidgetMedia::on_volumeSlider_valueChanged(int value)
{
	quazaaSettings.Media.Volume = value;
}*/

void CWidgetMedia::on_actionMediaRepeat_triggered(bool checked)
{
	quazaaSettings.Media.Repeat = checked;
}

void CWidgetMedia::on_actionMediaShuffle_triggered(bool checked)
{
	quazaaSettings.Media.Shuffle = checked;
}

void CWidgetMedia::on_splitterMedia_customContextMenuRequested(QPoint pos)
{
	Q_UNUSED(pos);

	if(ui->splitterMedia->handle(1)->underMouse())
	{
		if(ui->splitterMedia->sizes()[1] > 0)
		{
			quazaaSettings.WinMain.MediaSplitterRestoreLeft = ui->splitterMedia->sizes()[0];
			quazaaSettings.WinMain.MediaSplitterRestoreRight = ui->splitterMedia->sizes()[1];
			QList<int> newSizes;
			newSizes.append(ui->splitterMedia->sizes()[0] + ui->splitterMedia->sizes()[1]);
			newSizes.append(0);
			ui->splitterMedia->setSizes(newSizes);
		}
		else
		{
			QList<int> sizesList;
			sizesList.append(quazaaSettings.WinMain.MediaSplitterRestoreLeft);
			sizesList.append(quazaaSettings.WinMain.MediaSplitterRestoreRight);
			ui->splitterMedia->setSizes(sizesList);
		}
	}
}

void CWidgetMedia::on_toolButtonMediaPlaylistTaskHeader_clicked()
{
	if(ui->splitterMedia->sizes()[0] > 0)
	{
		quazaaSettings.WinMain.MediaSplitterRestoreLeft = ui->splitterMedia->sizes()[0];
		quazaaSettings.WinMain.MediaSplitterRestoreRight = ui->splitterMedia->sizes()[1];
		QList<int> newSizes;
		newSizes.append(0);
		newSizes.append(ui->splitterMedia->sizes()[0] + ui->splitterMedia->sizes()[1]);
		ui->splitterMedia->setSizes(newSizes);
	}
	else
	{
		QList<int> sizesList;
		sizesList.append(quazaaSettings.WinMain.MediaSplitterRestoreLeft);
		sizesList.append(quazaaSettings.WinMain.MediaSplitterRestoreRight);
		ui->splitterMedia->setSizes(sizesList);
	}
}

void CWidgetMedia::setSkin()
{
	ui->toolButtonMediaPlaylistTaskHeader->setStyleSheet(skinSettings.taskHeader);
	ui->frameMediaPlaylistTask->setStyleSheet(skinSettings.sidebarBackground);
	ui->listViewMediaPlaylist->setStyleSheet(skinSettings.listViews);
}

void CWidgetMedia::on_toolButtonOpen_clicked()
{
	QString file = QFileDialog::getOpenFileName(this, tr("Open File"));

	QUrl url = QUrl::fromLocalFile(file);

	playlist->addMedia(url);

	player->play();
}
