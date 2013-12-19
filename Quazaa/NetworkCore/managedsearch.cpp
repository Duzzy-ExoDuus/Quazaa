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

#include "managedsearch.h"
#include "network.h"
#include "neighbours.h"
#include "g2node.h"
#include "g2packet.h"
#include "query.h"
#include "datagrams.h"
#include "searchmanager.h"
#include "systemlog.h"
#include "Hashes/hash.h"
#include "queryhit.h"

#include <QMutexLocker>

#include "quazaasettings.h"

#include "debug_new.h"

CManagedSearch::CManagedSearch(CQuery* pQuery, QObject* parent) :
	QObject(parent)
{
	m_bActive = false;
	m_bPaused = false;
	m_pQuery = pQuery;
	m_tStarted = common::getDateTimeUTC();
	m_tCleanHostsNext = common::getDateTimeUTC().addSecs(quazaaSettings.Gnutella2.QueryHostThrottle);

	m_oGUID = QUuid::createUuid();
	pQuery->setGUID(m_oGUID);

	m_nHubs = m_nLeaves = m_nHits = 0;

	m_bCanRequestKey = true;
	m_nQueryCount = 0;
	m_nCookie = 0;
	m_nCachedHits = 0;
	m_pCachedHit = 0;
	m_nQueryHitLimit = quazaaSettings.Gnutella.MaxResults;
}

CManagedSearch::~CManagedSearch()
{
	if(m_bActive || m_bPaused)
	{
		stop();
	}

	if(m_pQuery)
	{
		delete m_pQuery;
	}

	if(m_pCachedHit)
	{
		delete m_pCachedHit;
		m_nCachedHits = 0;
		m_pCachedHit = 0;
	}
}

void CManagedSearch::start()
{
	if(!m_bPaused)
	{
		SearchManager.add(this);
	}

	m_bActive = true;
	m_bPaused = false;

	m_nQueryCount = 0;
	m_nQueryHitLimit = m_nHits + quazaaSettings.Gnutella.MaxResults;

	emit stateChanged();
}
void CManagedSearch::pause()
{
	m_bPaused = true;
	m_bActive = false;

	emit stateChanged();
}
void CManagedSearch::stop()
{
	m_bActive = false;
	m_bPaused = false;

	SearchManager.remove(this);

	emit stateChanged();
}

void CManagedSearch::execute(const QDateTime& tNowDT, quint32* pnMaxPackets)
{
	if ( !m_bActive )
	{
		return;
	}

	if ( m_nQueryCount > quazaaSettings.Gnutella2.QueryLimit )
	{
		systemLog.postLog( LogSeverity::Debug, Components::G2, "Pausing search: query limit reached" );
		pause();
		return;
	}

	quint32 nMaxPackets = *pnMaxPackets;

	if ( m_tStarted.secsTo( tNowDT ) < 30 )
	{
		nMaxPackets = qMin( quint32(2), nMaxPackets );
	}

	*pnMaxPackets -= nMaxPackets;

	searchNeighbours( tNowDT );
	searchG2( tNowDT, &nMaxPackets );

	*pnMaxPackets += nMaxPackets;

	m_bCanRequestKey = !m_bCanRequestKey;

	if ( tNowDT > m_tCleanHostsNext )
	{
#ifdef _DEBUG
		quint32 nRemoved = 0;
		quint32 nOldSize = m_lSearchedNodes.size();
#endif

		for ( QHash<QHostAddress, QDateTime>::iterator itHost = m_lSearchedNodes.begin();
			  itHost != m_lSearchedNodes.end(); )
		{
			if ( (*itHost).secsTo( tNowDT ) > quazaaSettings.Gnutella2.RequeryDelay )
			{
#ifdef _DEBUG
				++nRemoved;
#endif
				itHost = m_lSearchedNodes.erase( itHost );
			}
			else
			{
				++itHost;
			}
		}

		m_tCleanHostsNext = tNowDT.addSecs( quazaaSettings.Gnutella2.QueryHostThrottle );

#ifdef _DEBUG
		systemLog.postLog( LogSeverity::Debug, Components::G2,
						   QString( "Clearing don't-try list for search %1, old size: %2, new size: %3, items removed: %4"
									).arg( m_oGUID.toString(), QString::number( nOldSize ),
										   QString::number( m_lSearchedNodes.size() ),
										   QString::number( nRemoved ) ) );
#endif
	}
}

void CManagedSearch::searchNeighbours(const QDateTime& tNowDT)
{
	QMutexLocker l( &Neighbours.m_pSection );

	const quint32 tNow = tNowDT.toTime_t();

	for ( QList<CNeighbour*>::iterator itNode = Neighbours.begin();
		  itNode != Neighbours.end(); ++itNode )
	{
		if ( (*itNode)->m_nProtocol != DiscoveryProtocol::G2 )
		{
			continue;
		}

		CG2Node* pNode = (CG2Node*)(*itNode);

		if ( pNode->m_nState == nsConnected &&
			 tNow - pNode->m_tConnected > 15 &&
			 ( tNow - pNode->m_tLastQuery > quazaaSettings.Gnutella2.QueryHostThrottle &&
			   !m_lSearchedNodes.contains( pNode->m_oAddress ) ) )
		{
			G2Packet* pQuery = m_pQuery->toG2Packet( Network.isFirewalled() ?
														 NULL : &Network.m_oAddress );
			if ( pQuery )
			{
				m_lSearchedNodes[pNode->m_oAddress] = tNowDT;
				pNode->sendPacket( pQuery, true, true );
				pNode->m_tLastQuery = tNow;
			}
		}
	}
}

void CManagedSearch::searchG2(const QDateTime& tNowDT, quint32* pnMaxPackets)
{
	Q_ASSERT( tNowDT.timeSpec() == Qt::UTC );
	const quint32 tNow      = tNowDT.toTime_t();
	CG2Node* pLastNeighbour = NULL;
	SharedG2HostPtr pHost;

#if ENABLE_G2_HOST_CACHE_BENCHMARKING
	QElapsedTimer tHostCacheLock;
	tHostCacheLock.start();
#endif

	qDebug() << "**** [Search] Starting a search. Waiting for the lock.";

	QMutexLocker oHostCacheLock( &hostCache.m_pSection );

	qDebug() << "**** [Search] Lock aquired.";

#if ENABLE_G2_HOST_CACHE_BENCHMARKING
	const qint64 tHCLock = tHostCacheLock.elapsed();
	qint64 tHCWork = 0, tHCWorkStart = 0;
	QElapsedTimer tHostCacheWork;
	tHostCacheWork.start();
#endif

	for ( G2HostCacheIterator itHost = hostCache.m_lHosts.begin();
		  itHost != hostCache.m_lHosts.end(); ++itHost )
	{
		pHost = *itHost;

		if ( !pHost )
			continue;

		qDebug() << "**** [Search] Trying new Host: "
				 << pHost->address().toString().toLocal8Bit().data();

		if ( tNow - pHost->timestamp() > quazaaSettings.Gnutella2.HostCurrent )
			break; // timestamp sorted cache

		if ( !pHost->canQuery( tNow ) )
			continue;

		// don't query already queried hosts
		// this applies to query key requests as well,
		// so we don't waste our resources to request a key that will be useless in this search anyway
		if ( m_lSearchedNodes.contains( pHost->address() ) )
		{
			if ( m_lSearchedNodes[pHost->address()].secsTo( tNowDT ) <
				 (int)(quazaaSettings.Gnutella2.RequeryDelay) )
			{
				continue;
			}
		}

		Neighbours.m_pSection.lock();
		if ( Neighbours.find( (QHostAddress)(pHost->address()) ) )
		{
			// don't udp to neighbours
			Neighbours.m_pSection.unlock();
			continue;
		}
		Neighbours.m_pSection.unlock();

#if ENABLE_G2_HOST_CACHE_BENCHMARKING
		// at this point we've got a valid host
		tHCWork += tHostCacheWork.elapsed() - tHCWorkStart;

		qDebug() << "**** [Search] Host OK! **** Work time: "
				 << QString::number( tHCWork ).toLocal8Bit().data();
#endif

		CEndPoint pReceiver;

		bool bRefreshKey = false;

		if ( !pHost->queryKey() )
		{
			// we don't have a key
		}
		else
		{
			if ( tNow - pHost->keyTime() > quazaaSettings.Gnutella2.QueryKeyTime)
			{
				// query key expired
				pHost->setQueryKey( 0 );
				bRefreshKey = true;
			}
			else if ( !Network.isFirewalled() )
			{
				if ( pHost->keyHost() == Network.m_oAddress )
				{
					pReceiver = Network.m_oAddress;
				}
				else
				{
					pHost->setQueryKey( 0 );
				}
			}
			else
			{
				// we are firewalled, so key must be for one of our connected neighbours
				Neighbours.m_pSection.lock();

				CNeighbour* pNode = Neighbours.find( (QHostAddress)(pHost->keyHost()),
													 DiscoveryProtocol::G2 );

				if ( pNode && static_cast<CG2Node*>(pNode)->m_nState == nsConnected )
				{
					pReceiver = pNode->m_oAddress;
				}
				else
				{
					pHost->setQueryKey( 0 );
				}

				Neighbours.m_pSection.unlock();
			}
		}

		// if we still have a key, send the query
		if ( pHost->queryKey() )
		{
			sendG2Query( pReceiver, pHost, pnMaxPackets, tNowDT );

	}
		else if ( m_bCanRequestKey &&
				  tNow - pHost->keyTime() > quazaaSettings.Gnutella2.QueryHostThrottle )
		{
			// we can request a query key now
			// UDP QKR is sent without ACK request, so this may be a retry

			bool bKeyRequested = false;

			if ( !Network.isFirewalled() )
			{
				// request a key for our address
				requestG2QueryKey( pHost );

				bKeyRequested = true;
			}
			else
			{
				Neighbours.m_pSection.lock();

				// Find best hub for routing
				CG2Node* pHub = findBestHubForRoutingG2( pLastNeighbour );

				if ( pHub )
				{
					pLastNeighbour = pHub;
					if ( !pHub->m_tKeyRequest )
					{
						pHub->m_tKeyRequest = tNow;
					}

					if ( pHub->m_bCachedKeys )
					{
						G2Packet* pQKR = G2Packet::newPacket( "QKR", true );
						pQKR->writePacket( "QNA", (pHost->address().protocol() ? 18 : 6)
										   )->writeHostAddress( pHost->address() );
						if ( bRefreshKey )
							pQKR->writePacket( "REF", 0 );

#if LOG_QUERY_HANDLING
						systemLog.postLog( LogSeverity::Debug,
										   QString( "Requesting query key from %1 through %2"
													).arg( pHost->address().toString()
														   ).arg( pHub->address().toString() ) );
#endif // LOG_QUERY_HANDLING
						pHub->sendPacket( pQKR, true, true );
					}
					else
					{
						G2Packet* pQKR = G2Packet::newPacket( "QKR", true );
						pQKR->writePacket( "RNA", (pHub->m_oAddress.protocol() ? 18 : 6)
										   )->writeHostAddress( pHub->m_oAddress );
						Datagrams.sendPacket( pHost->address(), pQKR, false );
						pQKR->release();

#if LOG_QUERY_HANDLING
						systemLog.postLog( LogSeverity::Debug,
										   QString( "Requesting query key from %1 for %2"
													).arg( pHost->address().toString()
														   ).arg( pHub->address().toString() ) );
#endif // LOG_QUERY_HANDLING
					}

					bKeyRequested = true;
				}

				Neighbours.m_pSection.unlock();
			}

			if ( bKeyRequested )
			{
				*pnMaxPackets -= 1;

				if ( !pHost->ack() )
				{
					pHost->setAck( tNow );
				}

				pHost->setKeyTime( tNow );
				pHost->setQueryKey( 0 );
			}
		}

		if ( !(*pnMaxPackets) )
		{
			break;
		}

#if ENABLE_G2_HOST_CACHE_BENCHMARKING
		// now we start with a new search for an agreeable host to search
		tHCWorkStart = tHostCacheWork.elapsed();

		qDebug() << "**** [Search] Host OK! **** ";
#endif

		pHost.clear();
	}

#if ENABLE_G2_HOST_CACHE_BENCHMARKING
	tHCWork += tHostCacheWork.elapsed() - tHCWorkStart;

	oHostCacheLock.unlock();

	hostCache.m_nLockWaitTime.fetchAndAddRelaxed( tHCLock );
	hostCache.m_nWorkTime.fetchAndAddRelaxed( tHCWork );

	qDebug() << "**** [HostCache]"
			 << " Total lock wait time: "
			 << QString::number( hostCache.m_nLockWaitTime.load() ).toLocal8Bit().data()
			 << " Total work time: "
			 << QString::number( hostCache.m_nWorkTime.load()     ).toLocal8Bit().data()
			 << " ****";
#endif
}

void CManagedSearch::sendG2Query(CEndPoint pReceiver, SharedG2HostPtr pHost,
								 quint32* pnMaxPackets, const QDateTime& tNowDT)
{
	Q_ASSERT( !pReceiver.isNull() );

	m_lSearchedNodes[pHost->address()] = tNowDT;

	pHost->setLastQuery( tNowDT.toTime_t() );
	if ( !pHost->ack() )
	{
		pHost->setAck( tNowDT.toTime_t() );
	}

	G2Packet* pQuery = m_pQuery->toG2Packet( &pReceiver, pHost->queryKey() );

	if ( pQuery )
	{
#if LOG_QUERY_HANDLING
		systemLog.postLog( LogSeverity::Debug,
						   QString( "Querying %1" ).arg( pHost->address().toString() ) );
#endif // LOG_QUERY_HANDLING

		*pnMaxPackets -= 1;
		Datagrams.sendPacket( pHost->address(), pQuery, true );
		pQuery->release();
		++m_nQueryCount;
	}
}

void CManagedSearch::requestG2QueryKey(SharedG2HostPtr pHost)
{
	// request a key for our address
	G2Packet* pQKR = G2Packet::newPacket( "QKR", false );
	pQKR->writePacket( "RNA", (Network.m_oAddress.protocol() ? 18 : 6)
					   )->writeHostAddress( Network.m_oAddress );
	Datagrams.sendPacket( pHost->address(), pQKR, false );
	pQKR->release();

#if LOG_QUERY_HANDLING
	systemLog.postLog( LogSeverity::Debug,
					   QString( "Requesting query key from %1"
								).arg( pHost->address().toString() ) );
#endif // LOG_QUERY_HANDLING
}

/**
	 * @brief CManagedSearch::findBestHubForRoutingG2
	 * Locking: Requires lock on Neighbours.m_pSection.
	 * @return
	 */
CG2Node* CManagedSearch::findBestHubForRoutingG2(const CG2Node* const pLastNeighbour)
{
	CG2Node* pHub = NULL;

	// Find best hub for routing
	bool bCheckLast = Neighbours.m_nHubsConnectedG2 > 2;
	for ( QList<CNeighbour*>::iterator itNode = Neighbours.begin();
		  itNode != Neighbours.end(); ++itNode )
	{
		if ( (*itNode)->m_nProtocol != DiscoveryProtocol::G2 ||
			 (*itNode)->m_nState != nsConnected )
		{
			continue;
		}

		CG2Node* pNode = (CG2Node*)(*itNode);

		// Must be a hub that already acked our query
		if ( pNode->m_nType == G2_HUB &&
			 m_lSearchedNodes.contains( pNode->m_oAddress ) )
		{
			if ( ( bCheckLast && pNode == pLastNeighbour ) )
			{
				continue;
			}

			if ( pHub )
			{
				if ( !pNode->m_nPingsWaiting &&
					 pNode->m_tRTT < pHub->m_tRTT &&
					 pNode->m_tRTT < 10000 )
				{
					pHub = pNode;
				}
			}
			else if ( !pNode->m_nPingsWaiting )
			{
				pHub = pNode;
			}
		}
	}

	return pHub;
}

void CManagedSearch::onHostAcknowledge(QHostAddress nHost, const QDateTime& tNow)
{
	m_lSearchedNodes[nHost] = tNow;
}

void CManagedSearch::onQueryHit(CQueryHit* pHits)
{
	CQueryHit* pHit = pHits;
	CQueryHit* pLast = 0;

	while ( pHit )
	{
		++m_nHits;
		++m_nCachedHits;
		pLast = pHit;
		pHit = pHit->m_pNext;
	}

	//emit OnHit(pHits);

	if ( m_pCachedHit )
	{
		pLast->m_pNext = m_pCachedHit;
	}

	m_pCachedHit = pHits;

	if ( m_nCachedHits > 100 )
	{
		sendHits();
	}

	if ( m_nHits > m_nQueryHitLimit && !m_bPaused )
	{
		systemLog.postLog(LogSeverity::Debug, Components::G2, tr("Pausing search: query hit limit reached"));
		pause();
	}
}
void CManagedSearch::sendHits()
{
	if ( !m_pCachedHit )
	{
		return;
	}

	systemLog.postLog(LogSeverity::Debug, Components::G2, QString("Sending hits... %1").arg(m_nCachedHits));
	//qDebug() << "Sending hits..." << m_nCachedHits;
	QueryHitSharedPtr pSHits(m_pCachedHit);
	emit onHit(pSHits);
	m_pCachedHit = 0;
	m_nCachedHits = 0;
}

