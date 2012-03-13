/*
** $Id: widgetuploads.cpp 936 2012-02-19 00:22:43Z smokexyz $
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

#include "widgetuploads.h"
#include "ui_widgetuploads.h"

#include "quazaasettings.h"
#include "skinsettings.h"

#ifdef _DEBUG
#include "debug_new.h"
#endif

WidgetUploads::WidgetUploads(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::WidgetUploads)
{
	ui->setupUi(this);
	restoreState(quazaaSettings.WinMain.UploadsToolbar);
	setSkin();
}

WidgetUploads::~WidgetUploads()
{
	delete ui;
}

void WidgetUploads::changeEvent(QEvent* e)
{
	QMainWindow::changeEvent(e);
	switch(e->type())
	{
		case QEvent::LanguageChange:
			ui->retranslateUi(this);
			break;
		default:
			break;
	}
}

void WidgetUploads::saveWidget()
{
	quazaaSettings.WinMain.UploadsToolbar = saveState();
}

void WidgetUploads::setSkin()
{
	ui->treeViewUploads->setStyleSheet(skinSettings.listViews);
}
