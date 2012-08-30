/*
** $Id$
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

#include "commonfunctions.h"
#include <QDir>
#include <QDesktopServices>
#include <QUrl>
#include "Hashes/hash.h"

#ifdef _DEBUG
#include "debug_new.h"
#endif

void common::folderOpen(QString file)
{
    QDir completePath( file );

    if ( !completePath.exists() )
    {
        completePath.mkpath( file );
    }
    QDesktopServices::openUrl( QUrl::fromLocalFile(file) );
}

QString common::formatBytes(quint64 nBytesPerSec)
{
    const char* szUnit[5] = {"B", "KiB", "MiB", "GiB", "TiB"};

    double nBPS = nBytesPerSec;

    int nStep = 0;
    while ( nBPS > 1024 )
    {
        nBPS /= 1024;
        nStep++;
        if ( nStep == 4 )
        {
            break;
        }
    }

    if ( nStep )
    {
        return QString().sprintf( "%1.2f %s", nBPS, szUnit[nStep] );
    }
    else
    {
        return QString().sprintf( "%1.0f %s", nBPS, szUnit[nStep] );
    }
}

QString common::vendorCodeToName(QString vendorCode)
{
    if ( vendorCode == "RAZA" )
    {
        return "Shareaza";
    }
    if ( vendorCode == "RAZB" )
    {
        return "Shareaza Beta";
    }
    if ( vendorCode == "QAZA" )
    {
        return "Quazaa";
    }
    if (vendorCode == "QAZB" )
    {
        return "Quazaa Beta";
    }
    if (vendorCode == "SHLN" )
    {
        return "Sharelin";
    }
    if (vendorCode == "RZCB" )
    {
        return "Shareaza Plus";
    }
    if (vendorCode == "FSCP" )
    {
        return "FileScope";
    }
    if (vendorCode == "AGIO" )
    {
        return "Adiago";
    }
    if (vendorCode == "GNUC" )
    {
        return "Gnucleus";
    }
    if (vendorCode == "MLDK" )
    {
        return "MLDonkey";
    }
    if (vendorCode == "MMMM" )
    {
        return "Morpheus";
    }
    if (vendorCode == "PEER" )
    {
        return "PeerProject";
    }
    else
    {
        return vendorCode;
    }
}

QString common::fixFileName(QString sName)
{
    QString sRet = sName;
    // \ * / ? % : | " > <
    sRet.replace(QRegExp("[\\\\\\*\\/\\?\\%\\:\\|\\\"\\>\\<\\x0000]"), "_");
    return sRet.left(255);
}

QString common::getTempFileName(QString sName)
{
    CHash oHashName(CHash::SHA1);
    oHashName.AddData(sName.toUtf8());
    oHashName.AddData(QString().number(qrand() % qrand()).append(QDateTime::currentDateTimeUtc().toString(Qt::ISODate)).toAscii());
    oHashName.Finalize();
    return oHashName.ToString();
}

/*common::registeredSet common::registerNumber()
{
    registeredSet result;
    result.num8 = 0;
    result.num64 = 0;    // this is used to identify a struct as being invalid.

    for ( quint8 i = 0; i < NO_OF_REGISTRATIONS; ++i )
    {
        quint64 counter = 1;
        quint64 num64comp = 1;
        while ( counter < 64 )
        {
            if ( registeredNumbers[i] & num64comp ) // this means the current number is registered
            {
                ++counter;
                num64comp *= 2;
            }
            else
            {
                result.num8 = i;
                result.num64 = num64comp;

                return result;
            }
        }
    }
    return result;
}

bool common::unregisterNumber(registeredSet registered)
{
    if ( registeredNumbers[ registered.num8 ] & registered.num64 )
    {
        registeredNumbers[ registered.num8 ] = registeredNumbers[ registered.num8 ] & ( ~registered.num64 );
        return true;
    }
    return false;
}*/
