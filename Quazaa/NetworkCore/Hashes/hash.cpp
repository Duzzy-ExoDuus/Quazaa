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


#include "hash.h"
#include "systemlog.h"
#include <QCryptographicHash>
#include "3rdparty/CyoEncode/CyoEncode.h"
#include "3rdparty/CyoEncode/CyoDecode.h"

#ifdef _DEBUG
#include "debug_new.h"
#endif

CHash::CHash(const CHash &rhs)
{
	if ( !rhs.m_bFinalized )
	{
		systemLog.postLog(LogCategory::Library, LogSeverity::Debug, QObject::tr( "WARNING: Copying non-finalized CHash" ) );
	}

	m_baRawValue = rhs.m_baRawValue;
	m_nHashAlgorithm = rhs.m_nHashAlgorithm;
	m_bFinalized = true;
	m_pContext = 0;
}

CHash::CHash(Algorithm algo)
{
	m_bFinalized = false;
	m_nHashAlgorithm = algo;

	switch( algo )
	{
	case CHash::SHA1:
		m_pContext = new QCryptographicHash( QCryptographicHash::Sha1 );
		break;
	case CHash::MD4:
		m_pContext = new QCryptographicHash( QCryptographicHash::Md4 );
		break;
	case CHash::MD5:
		m_pContext = new QCryptographicHash( QCryptographicHash::Md5 );
		break;
	default:
		m_pContext = 0; /* error? */
	}
}

CHash::CHash(QByteArray baRaw, CHash::Algorithm algo)
{
	if ( baRaw.size() != CHash::ByteCount( algo ) )
	{
		throw invalid_hash_exception();
	}
	m_baRawValue = baRaw;
	m_nHashAlgorithm = algo;
	m_pContext = 0;
	m_bFinalized = true;
}
CHash::~CHash()
{
	if ( m_pContext )
	{
		switch( m_nHashAlgorithm )
		{
		case CHash::SHA1:
		case CHash::MD5:
		case CHash::MD4:
			delete ( (QCryptographicHash*)m_pContext );
		}
	}
}

// Returns raw hash length by hash family
int CHash::ByteCount(int algo)
{
	switch( algo )
	{
	case CHash::SHA1:
		return 20;
	case CHash::MD4:
		return 16;
	case CHash::MD5:
		return 16;
	default:
		return 0;
	}
}

// Parses URN and returns CHash pointer if conversion succeed, 0 otherwise
CHash* CHash::FromURN(QString sURN)
{
	// try to get hash family from URN
	// urn:tree:tiger:/

	if ( sURN.size() < 16 )
	{
		throw invalid_hash_exception();
	}

	QByteArray baFamily;
	int nStart = ( strncmp( "urn:", sURN.toAscii().data(), 4 ) == 0 ? 4 : 0 );
	int nStartHash = sURN.indexOf( ":", nStart ) + 1;
	baFamily = sURN.mid( nStart, nStartHash - nStart - 1 ).toLower().toAscii();
	QByteArray baValue = sURN.mid( nStartHash ).toAscii();
	char pVal[ 128 ];

	if ( baFamily == "sha1" && baValue.length() == 32 )
	{
		// sha1 base32 encoded
		if ( cyoBase32Validate(baValue.data(), baValue.length()) == 0 )
		{
			// valid sha1/base32
			cyoBase32Decode( (char*)&pVal, baValue.data(), baValue.length() );
			CHash* pRet = new CHash(QByteArray( (char*)&pVal ), CHash::SHA1);
			return pRet;
		}
	}
	else if(baFamily == "md5" && baValue.length() == 32)
	{
		if(cyoBase16Validate(baValue.data(), baValue.length()) == 0)
		{
			cyoBase16Decode((char*)&pVal, baValue.data(), baValue.length());
			CHash* pRet = new CHash(QByteArray((char*)&pVal), CHash::MD5);
			return pRet;
		}
	}

	return 0;

}
CHash* CHash::FromRaw(QByteArray &baRaw, CHash::Algorithm algo)
{
	try
	{
		CHash* pRet = new CHash( baRaw, algo );
		return pRet;
	}
	catch( ... )
	{

	}
	return 0;
}

// Returns URN as string
QString CHash::ToURN() const
{
	switch( m_nHashAlgorithm )
	{
	case CHash::SHA1:
		return QString( "urn:sha1:" ) + ToString();
	case CHash::MD5:
		return QString("urn:md5:") + ToString();
	case CHash::MD4:
		break;
	}

	return QString();
}

// Returns hash value as a string in most natural encoding
QString CHash::ToString() const
{
	char pBuff[128];
	memset( &pBuff, 0, sizeof( pBuff ) );

	switch( m_nHashAlgorithm )
	{
	case CHash::SHA1:
		cyoBase32Encode( (char*)&pBuff, RawValue().data(), 20 );
	case CHash::MD5:
		cyoBase16Encode((char*)&pBuff, RawValue().data(), 16);
	case CHash::MD4:
		break;
	}

	return QString( pBuff );
}

void CHash::Finalize()
{
	if(!m_bFinalized)
	{
		Q_ASSERT(m_pContext);

		switch(m_nHashAlgorithm)
		{
		case CHash::SHA1:
		case CHash::MD5:
		case CHash::MD4:
			m_baRawValue = ((QCryptographicHash*)m_pContext)->result();
			delete((QCryptographicHash*)m_pContext);
			m_pContext = 0;
			m_bFinalized = true;
		}
	}
}

void CHash::AddData(const char *pData, quint32 nLength)
{
	Q_ASSERT( !m_bFinalized && m_pContext );

	switch( m_nHashAlgorithm )
	{
	case CHash::SHA1:
	case CHash::MD5:
	case CHash::MD4:
		( (QCryptographicHash*)m_pContext )->addData( pData, nLength );
	}
}
void CHash::AddData(QByteArray baData)
{
	AddData( baData.data(), baData.length() );
}

QString CHash::GetFamilyName()
{
	switch( m_nHashAlgorithm )
	{
	case CHash::SHA1:
		return QString( "sha1" );
	case CHash::MD5:
		return QString( "md5" );
	case CHash::MD4:
		return QString( "md4" );
	}

	return "";
}

QDataStream& operator<<(QDataStream& s, const CHash& rhs)
{
	s << rhs.ToURN();
	return s;
}
QDataStream& operator>>(QDataStream& s, CHash& rhs)
{
	QString sTmp;
	s >> sTmp;
	CHash* pHash = CHash::FromURN(sTmp);
	rhs = *pHash;
	delete pHash;
	return s;
}

