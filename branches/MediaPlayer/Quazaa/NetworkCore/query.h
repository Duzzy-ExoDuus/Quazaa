/*
** query.h
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

#ifndef QUERY_H
#define QUERY_H

#include "types.h"

class G2Packet;
class CQuery;
class CHash;

typedef QSharedPointer<CQuery> CQueryPtr;

class CQuery
{
public:
	QUuid           m_oGUID;
	QList<CHash>	m_lHashes;
	QString         m_sMetadata;
	quint64         m_nMinimumSize;
	quint64         m_nMaximumSize;
	QString         m_sDescriptiveName;
	CEndPoint		m_oEndpoint;
	quint32			m_nQueryKey;

	QString			m_sG2PositiveWords;
	QString			m_sG2NegativeWords;
	QList<quint32>	m_lHashedKeywords;
public:
	CQuery();
	QString DescriptiveName()
	{
		return m_sDescriptiveName;
	}

	void SetGUID(QUuid& guid);

	void AddURN(const CHash& pHash);
	void SetDescriptiveName(QString sDN);
	void SetSizeRestriction(quint64 nMin, quint64 nMax);
	void SetMetadata(QString sMeta);

	void BuildG2Keywords(QString strPhrase);
	bool CheckValid();

	G2Packet* ToG2Packet(CEndPoint* pAddr = 0, quint32 nKey = 0);

	static CQueryPtr FromPacket(G2Packet* pPacket, CEndPoint* pEndpoint = 0);

private:
	bool FromG2Packet(G2Packet* pPacket, CEndPoint* pEndpoint);
};

#endif // QUERY_H
