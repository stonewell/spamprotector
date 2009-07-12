#pragma once

#include "Pattern.h"

class CPatternList : public CObject
{
public:
	DECLARE_SERIAL(CPatternList)

	CPatternList(void);
	CPatternList(const CPatternList & patternList);
	virtual ~CPatternList(void);

public:
	BOOL IsMatch(const CString & strMatch, CPattern ** ppPattern);

	virtual void Serialize( CArchive& archive );
	virtual CPatternList & operator = (const CPatternList & patternList);

public:
	CArray<CPattern> m_Patterns;
};
