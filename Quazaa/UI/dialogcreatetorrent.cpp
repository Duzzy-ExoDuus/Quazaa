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

#include "dialogcreatetorrent.h"
#include "ui_dialogcreatetorrent.h"
#include "skinsettings.h"

#include "debug_new.h"

DialogCreateTorrent::DialogCreateTorrent(QWidget* parent) :
	QDialog(parent),
	ui(new Ui::DialogCreateTorrent)
{
	ui->setupUi(this);
	ui->comboBoxSeedDHT->setView(new QListView());
	setSkin();
}

DialogCreateTorrent::~DialogCreateTorrent()
{
	delete ui;
}

void DialogCreateTorrent::changeEvent(QEvent* e)
{
	switch(e->type())
	{
		case QEvent::LanguageChange:
			ui->retranslateUi(this);
			break;
		default:
			break;
	}
}

void DialogCreateTorrent::on_pushButtonCancel_clicked()
{
	emit closed();
	close();
}

void DialogCreateTorrent::on_pushButtonSave_clicked()
{
	emit closed();
	close();
}

void DialogCreateTorrent::setSkin()
{

}
