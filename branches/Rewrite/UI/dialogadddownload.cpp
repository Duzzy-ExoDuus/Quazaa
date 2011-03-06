/*
** dialogadddownload.cpp
**
** Copyright � Quazaa Development Team, 2009-2011.
** This file is part of QUAZAA (quazaa.sourceforge.net)
**
** Quazaa is free software; this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
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

#include "dialogadddownload.h"
#include "ui_dialogadddownload.h"

DialogAddDownload::DialogAddDownload(QWidget* parent) :
	QDialog(parent),
	m_ui(new Ui::DialogAddDownload)
{
	m_ui->setupUi(this);
}

DialogAddDownload::~DialogAddDownload()
{
	delete m_ui;
}

void DialogAddDownload::changeEvent(QEvent* e)
{
	QDialog::changeEvent(e);
	switch(e->type())
	{
		case QEvent::LanguageChange:
			m_ui->retranslateUi(this);
			break;
		default:
			break;
	}
}

void DialogAddDownload::on_pushButtonDownload_clicked()
{
	emit closed();
	close();
}

void DialogAddDownload::on_pushButtonCancel_clicked()
{
	emit closed();
	close();
}
