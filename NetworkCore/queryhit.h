/*
** queryhit.h
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

#ifndef QUERYHIT_H
#define QUERYHIT_H

#include "types.h"
#include <QList>
#include <QSharedPointer>

#include "Hashes/sha1.h"

class G2Packet;
class CQuery;

struct QueryHitInfo
{
	CEndPoint	    m_oNodeAddress;
	CEndPoint		m_oSenderAddress;
	QUuid           m_oGUID;
	QUuid           m_oNodeGUID;
	QList<CEndPoint>    m_lNeighbouringHubs;
	quint8          m_nHops;
	QString			m_sVendor;

	QueryHitInfo()
	{
		m_sVendor = "";
	}

	~QueryHitInfo()
	{
		systemLog.postLog(LogSeverity::Debug, "Hit info deleted");
		//qDebug("hit info deleted");
	}
};

struct QueuedQueryHit
{
	QueryHitInfo* m_pInfo;
	G2Packet*     m_pPacket;
};

class CQueryHit
{
public:
	CQueryHit*      m_pNext;

	QSharedPointer<QueryHitInfo>   m_pHitInfo;

	CSHA1           m_oSha1;
	QString         m_sDescriptiveName;
	QString         m_sURL;
	QString         m_sMetadata;
	quint64         m_nObjectSize;
	quint32         m_nCachedSources;
	bool            m_bIsPartial;
	quint64         m_nPartialBytesAvailable;
	QString         m_sPreviewURL;

	bool            m_bIsP2PIMCapable;

public:
	CQueryHit();
	~CQueryHit();

	static QueryHitInfo* ReadInfo(G2Packet* pPacket, CEndPoint* pSender = 0);
	static CQueryHit*     ReadPacket(G2Packet* pPacket, QueryHitInfo* pHitInfo);

	void Delete();
	void ResolveURLs();
	bool IsValid(CQuery* pQuery = 0);
};

typedef class QSharedPointer<CQueryHit> QueryHitSharedPtr;

#endif // QUERYHIT_H
