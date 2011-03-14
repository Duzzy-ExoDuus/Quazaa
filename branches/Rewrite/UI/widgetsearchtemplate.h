/*
** widgetsearchtemplate.h
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

#ifndef WIDGETSEARCHTEMPLATE_H
#define WIDGETSEARCHTEMPLATE_H

#include <QtGui/QDialog>
#include "searchtreemodel.h"

namespace Ui
{
	class WidgetSearchTemplate;
}

class CManagedSearch;
class CQuery;
//class CQueryHit;
#include "NetworkCore/queryhit.h"
#include "types.h"

namespace SearchState
{
	enum SearchState { Stopped, Searching, Paused };
};

class WidgetSearchTemplate : public QWidget
{
	Q_OBJECT

public:
	WidgetSearchTemplate(QString searchString = "", QWidget* parent = 0);
	~WidgetSearchTemplate();
	SearchTreeModel* searchModel;
	CManagedSearch* m_pSearch;

	void StartSearch(CQuery* pQuery);
	void PauseSearch();
	void StopSearch();
	void ClearSearch();

	int nHubs;
	int nLeaves;
	int nHits;
	int nFiles;

	QString sSearchString;
	SearchState::SearchState searchState;

	//void GetStats(quint32& nHubs, quint32& nLeaves, quint32& nHits);

signals:
	void statsUpdated(WidgetSearchTemplate* thisSearch);

protected:
	void changeEvent(QEvent* e);

private:
	Ui::WidgetSearchTemplate* m_ui;

protected slots:
	void OnStatsUpdated();
};

#endif // WIDGETSEARCHTEMPLATE_H
