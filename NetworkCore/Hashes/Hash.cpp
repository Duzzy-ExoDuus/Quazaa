#include "Hash.h"
#include <QCryptographicHash>
#include "3rdparty/CyoEncode/CyoEncode.h"
#include "3rdparty/CyoEncode/CyoDecode.h"

CHash::CHash(CHash::Algorithm algo)
{
	m_bFinalized = false;
	m_nHashAlgorithm = algo;

	switch( algo )
	{
	case CHash::SHA1:
		m_pContext = new QCryptographicHash(QCryptographicHash::Sha1);
		break;
	case CHash::MD4:
		m_pContext = new QCryptographicHash(QCryptographicHash::Md4);
		break;
	case CHash::MD5:
		m_pContext = new QCryptographicHash(QCryptographicHash::Md5);
		break;
	default:
		m_pContext = 0; /* error? */
	}
}

CHash::CHash(QByteArray baRaw, CHash::Algorithm algo)
{
	if( baRaw.size() != CHash::ByteCount(algo) )
		throw invalid_hash_exception();
	m_baRawValue = baRaw;
	m_nHashAlgorithm = algo;
	m_pContext = 0;
	m_bFinalized = true;
}

int CHash::ByteCount(CHash::Algorithm algo)
{
	switch( algo )
	{
	case CHash::SHA1:
		return 20;
	case CHash::MD4:
		return 20;
	case CHash::MD5:
		return 25;
	default:
		return 0;
	}
}

CHash* CHash::FromURN(QString sURN)
{
	// try to get hash family from URN
	// urn:tree:tiger:/

	if( sURN.size() < 16 )
		throw invalid_hash_exception();

	QByteArray baFamily;
	int nStart = (strncmp("urn:", sURN.toAscii().data(), 4) == 0 ? 4 : 0);
	int nStartHash = sURN.indexOf(":", nStart) + 1;
	baFamily = sURN.mid(nStart, nStartHash - nStart - 1).toLower().toAscii();
	QByteArray baValue = sURN.mid(nStartHash).toAscii();
	char pVal[128];

	if( baFamily == "sha1" && baValue.length() == 32 )
	{
		// sha1 base32 encoded
		if( cyoBase32Validate(baValue.data(), baValue.length()) == 0 )
		{
			// valid sha1/base32
			cyoBase32Decode((char*)&pVal, baValue.data(), baValue.length());
			CHash* pRet = new CHash(QByteArray((char*)&pVal), CHash::SHA1);
			return pRet;
		}
	}

	return 0;

}
CHash* CHash::FromRaw(QByteArray &baRaw, CHash::Algorithm algo)
{
	try
	{
		CHash* pRet = new CHash(baRaw, algo);
		return pRet;
	}
	catch(...)
	{

	}
	return 0;
}

QString CHash::ToURN()
{
	switch( m_nHashAlgorithm )
	{
	case CHash::SHA1:
		return QString("urn:sha1:") + ToString();
	case CHash::MD5:
	case CHash::MD4:
		break;
	}

	return QString();
}
QString CHash::ToString()
{
	char pBuff[128];
	memset(&pBuff, 0, sizeof(pBuff));

	switch( m_nHashAlgorithm )
	{
	case CHash::SHA1:
		cyoBase32Encode((char*)&pBuff, RawValue().data(), 20);
	case CHash::MD5:
	case CHash::MD4:
		break;
	}

	return QString(pBuff);
}
QByteArray CHash::RawValue()
{
	if( !m_bFinalized )
	{
		Q_ASSERT(m_pContext);

		switch( m_nHashAlgorithm )
		{
		case CHash::SHA1:
		case CHash::MD5:
		case CHash::MD4:
			m_baRawValue = ((QCryptographicHash*)m_pContext)->result();
			delete ((QCryptographicHash*)m_pContext);
			m_pContext = 0;
			m_bFinalized = true;
		}
	}

	return m_baRawValue;
}

void CHash::AddData(const char *pData, quint32 nLength)
{
	Q_ASSERT(!m_bFinalized && m_pContext);

	switch( m_nHashAlgorithm )
	{
	case CHash::SHA1:
	case CHash::MD5:
	case CHash::MD4:
		((QCryptographicHash*)m_pContext)->addData(pData, nLength);
	}
}
void CHash::AddData(QByteArray &baData)
{
	AddData(baData.data(), baData.length());
}
