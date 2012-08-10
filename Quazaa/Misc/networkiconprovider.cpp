/*
** $Id: networkiconprovider.cpp 928 2012-02-15 17:19:00Z brov $
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

#include "networkiconprovider.h"
#include <QPixmapCache>
#include <QIcon>

CNetworkIconProvider::CNetworkIconProvider()
{
}

QIcon CNetworkIconProvider::icon(DiscoveryProtocol protocol)
{
	QIcon icon;
	QString key = QLatin1String("nip_dp_") + QString::number(protocol);

	QPixmap pixIcon;
	if( QPixmapCache::find(key, pixIcon) )
	{
		icon.addPixmap(pixIcon);
		return icon;
	}

	switch(protocol)
	{
		case dpNull:
			break;
		case dpG2:
			pixIcon.load(":/Resource/Networks/Gnutella2.png");
			break;
		case dpGnutella:
			pixIcon.load(":/Resource/Networks/Gnutella2.png");
			break;
		case dpAres:
			pixIcon.load(":/Resource/Networks/Ares.png");
			break;
		case dpeDonkey2000:
			pixIcon.load(":/Resource/Networks/EDonkey.png");
			break;
	}

	if(pixIcon.isNull())
		return QIcon();

	icon.addPixmap(pixIcon);
	QPixmapCache::insert(key, pixIcon);

	return icon;
}

QIcon CNetworkIconProvider::icon(TransferProtocol protocol)
{
	QIcon icon;
	QString key = QLatin1String("nip_tp_") + QString::number(protocol);

	QPixmap pixIcon;
	if( QPixmapCache::find(key, pixIcon) )
	{
		icon.addPixmap(pixIcon);
		return icon;
	}

	switch(protocol)
	{
		case tpNull:
			break;
		case tpHTTP:
			pixIcon.load(":/Resource/Networks/http.png");
			break;
		case tpBitTorrent:
			pixIcon.load(":/Resource/Networks/BitTorrent.png");
	}

	if(pixIcon.isNull())
		return QIcon();

	icon.addPixmap(pixIcon);
	QPixmapCache::insert(key, pixIcon);

	return icon;
}
