#pragma once

#include "Action.h"

#undef AFX_API
#define AFX_API AFX_EXT_CLASS

class CPattern : public CObject
{
public:
	DECLARE_SERIAL(CPattern)

	CPattern(void);
	CPattern(const CPattern & pattern);
	virtual ~CPattern(void);

	virtual void Serialize( CArchive& archive );
	virtual CPattern & operator = (const CPattern & pattern);

public:
	CActionList m_Actions;
	CString m_strPattern;
	BOOL m_bMatchPattern;//True:  Take Pattern actions only when the give string matches m_strPattern .
						 //False: Take Pattern actions only when the give string DOES NOT match m_strPattern.
};

#undef AFX_API
#define AFX_API
