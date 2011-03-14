/*
** dialogabout.cpp
**
** Copyright  Quazaa Development Team, 2009-2011.
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

#include "dialogabout.h"
#include "ui_dialogabout.h"
#include "quazaaglobals.h"
#include <QDesktopServices>
#include <QMessageBox>
#include <QUrl>

DialogAbout::DialogAbout(QWidget* parent) :
	QDialog(parent),
	m_ui(new Ui::DialogAbout)
{
	m_ui->setupUi(this);
        m_ui->labelVersion->setText(m_ui->labelVersion->text().replace("&lt;version&gt;", QuazaaGlobals::APPLICATION_VERSION_STRING()));
}

DialogAbout::~DialogAbout()
{
	delete m_ui;
}

void DialogAbout::changeEvent(QEvent* e)
{
	switch(e->type())
	{
		case QEvent::LanguageChange:
			m_ui->retranslateUi(this);
			break;
		default:
			break;
	}
}

void DialogAbout::on_pushButtonOK_clicked()
{
	emit closed();
	close();
}

void DialogAbout::on_labelCopyright_linkActivated(QString link)
{
	QDesktopServices::openUrl(QUrl(link, QUrl::TolerantMode));
}

void DialogAbout::on_textBrowserCredits_anchorClicked(QUrl link)
{
	QDesktopServices::openUrl(link);
}
