#pragma once

#include "PatternList.h"

class CSpamProtectorConfiguration
{
public:
	CSpamProtectorConfiguration(void);
	virtual ~CSpamProtectorConfiguration(void);

public:
	BOOL Load();
	BOOL Load(const CString & strFileName);
	BOOL LoadBuffer(void * pBuffer, size_t szBuf);
	BOOL LoadFile(CFile & file);

	BOOL Store();
	BOOL Store(const CString & strFileName);
	BOOL StoreBuffer(void * pBuffer, size_t szBuf);
	BOOL StoreFile(CFile & file);

	void Initialize();
public:
	CPatternList m_WhiteList;
	CPatternList m_BlackList;
	CPatternList m_ContentList;

	BOOL m_bUseWhiteList;
	BOOL m_bUseBlackList;
	BOOL m_bUseContentList;

	BOOL m_bAlwaysTrustContacts;

protected:
	int m_nVersion;
};

