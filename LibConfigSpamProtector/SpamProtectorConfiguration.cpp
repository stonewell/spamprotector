#include "StdAfx.h"
#include "SpamProtectorConfiguration.h"

LPCTSTR DEFAULT_CONFIGURATION_FILE_NAME = _T("SpamProtector.dat");

#define DAT_FILE_VERSION 1

CSpamProtectorConfiguration::CSpamProtectorConfiguration(void) :
m_nVersion(DAT_FILE_VERSION)
{
	Initialize();
}

CSpamProtectorConfiguration::~CSpamProtectorConfiguration(void)
{
}

BOOL CSpamProtectorConfiguration::Load()
{
	HMODULE hModule = GetModuleHandle(NULL);

	CString file_name;

	LPTSTR pBuf = file_name.GetBuffer(MAX_PATH);

	DWORD length = GetModuleFileName(hModule, pBuf, MAX_PATH);
	
	if (0 == length)
	{
		//TODO:Do Log here
		file_name.ReleaseBuffer();
		return Load(DEFAULT_CONFIGURATION_FILE_NAME);
	}
	else
	{
		file_name.ReleaseBuffer(length);

		int nEndPos = file_name.ReverseFind('\\');

		if (nEndPos > 0)
		{
			file_name = file_name.Left(nEndPos);
		}

		file_name.AppendFormat(_T("\\%s"), DEFAULT_CONFIGURATION_FILE_NAME);

		return Load(file_name);
	}
}

BOOL CSpamProtectorConfiguration::Load(const CString & strFileName)
{
	CFile file;

	if (file.Open(strFileName, CFile::modeRead))
	{
		return LoadFile(file);
	}

	return FALSE;
}

BOOL CSpamProtectorConfiguration::LoadBuffer(void * pBuffer, size_t szBuf)
{
	CMemFile file((BYTE *)pBuffer, szBuf);

	return LoadFile(file);
}

BOOL CSpamProtectorConfiguration::LoadFile(CFile & file)
{
	CArchive ar(&file, CArchive::load);

	try
	{
		ar >> m_nVersion;

		if (m_nVersion < 0 || m_nVersion > DAT_FILE_VERSION)
			throw 1;

		m_WhiteList.Serialize(ar);
		m_BlackList.Serialize(ar);
		m_ContentList.Serialize(ar);

		ar >> m_bUseWhiteList >> m_bUseBlackList >> m_bUseContentList;

		ar >> m_bAlwaysTrustContacts;
	}
	catch(...)
	{
		//TODO: Do Log error here
		Initialize();
	}

	return TRUE;
}

BOOL CSpamProtectorConfiguration::Store()
{
	HMODULE hModule = GetModuleHandle(NULL);

	CString file_name;

	LPTSTR pBuf = file_name.GetBuffer(MAX_PATH);

	DWORD length = GetModuleFileName(hModule, pBuf, MAX_PATH);
	
	if (0 == length)
	{
		//TODO:Do Log here
		file_name.ReleaseBuffer();
	
		return Store(DEFAULT_CONFIGURATION_FILE_NAME);
	}
	else
	{
		file_name.ReleaseBuffer(length);
		int nEndPos = file_name.ReverseFind('\\');

		if (nEndPos > 0)
		{
			file_name = file_name.Left(nEndPos);
		}

		file_name.AppendFormat(_T("\\%s"), DEFAULT_CONFIGURATION_FILE_NAME);

		return Store(file_name);
	}
}

BOOL CSpamProtectorConfiguration::Store(const CString & strFileName)
{
	CFile file;

	if (file.Open(strFileName, CFile::modeCreate | CFile::modeWrite))
	{
		return StoreFile(file);
	}

	return FALSE;
}

BOOL CSpamProtectorConfiguration::StoreBuffer(void * pBuffer, size_t szBuf)
{
	CMemFile file((BYTE *)pBuffer, szBuf);

	return StoreFile(file);
}

BOOL CSpamProtectorConfiguration::StoreFile(CFile & file)
{
	CArchive ar(&file, CArchive::store);

	try
	{
		ar << m_nVersion;

		m_WhiteList.Serialize(ar);
		m_BlackList.Serialize(ar);
		m_ContentList.Serialize(ar);

		ar << m_bUseWhiteList << m_bUseBlackList << m_bUseContentList;

		ar << m_bAlwaysTrustContacts;
	}
	catch(...)
	{
		//TODO: Do Log error here
		return FALSE;
	}

	return TRUE;
}

void CSpamProtectorConfiguration::Initialize()
{
	m_WhiteList.m_Patterns.RemoveAll();
	m_BlackList.m_Patterns.RemoveAll();
	m_ContentList.m_Patterns.RemoveAll();

	m_bUseWhiteList = FALSE;
	m_bUseBlackList = FALSE;
	m_bUseContentList = FALSE;

	m_bAlwaysTrustContacts = TRUE;
}