/*
** widgetneighbours.h
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

#ifndef WIDGETNEIGHBORS_H
#define WIDGETNEIGHBORS_H

#include <QtGui>
#include <QAbstractItemModel>

class CNeighboursTableModel;

namespace Ui
{
	class WidgetNeighbours;
}

class WidgetNeighbours : public QMainWindow
{
	Q_OBJECT
public:
	WidgetNeighbours(QWidget* parent = 0);
	~WidgetNeighbours();
	void setModel(QAbstractItemModel* model);
	QWidget* treeView();
	void saveWidget();
	QLabel* labelG2StatsIcon;
	QLabel* labelG2Stats;
	CNeighboursTableModel* neighboursList;

protected:
	void changeEvent(QEvent* e);

private:
	Ui::WidgetNeighbours* ui;
		QMenu *neighboursMenu;

private slots:
	void on_actionNetworkChatWith_triggered();
 void on_tableViewNeighbours_customContextMenuRequested(QPoint pos);
 void on_actionNeighbourDisconnect_triggered();
	void on_actionNeighbourConnectTo_triggered();
	void on_actionSettings_triggered();

	void onTimer();

	void updateG2();
	void updateAres();
	void updateEDonkey();
};

#endif // WIDGETNEIGHBORS_H
