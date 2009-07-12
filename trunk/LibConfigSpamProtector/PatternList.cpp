#include "StdAfx.h"
#include "PatternList.h"

IMPLEMENT_SERIAL(CPatternList, CObject, 1)

CPatternList::CPatternList(void)
{
}

CPatternList::CPatternList(const CPatternList & patternList)
{
	*this = patternList;
}

CPatternList::~CPatternList(void)
{
}

BOOL CPatternList::IsMatch(const CString & strMatch, CPattern ** ppPattern)
{
	return FALSE;
}

void CPatternList::Serialize( CArchive& ar )
{
	if (!ar.IsStoring())
	{
		m_Patterns.RemoveAll();

		int nCount = 0;

		ar >> nCount;

		for(int i =0; i < nCount; i++)
		{
			CPattern pattern;

			pattern.Serialize(ar);

			m_Patterns.Add(pattern);
		}
	}
	else
	{
		int nCount = m_Patterns.GetCount();

		ar << nCount;

		for(int i =0; i < nCount; i++)
		{
			m_Patterns.GetAt(i).Serialize(ar);
		}
	}
}

CPatternList & CPatternList::operator = (const CPatternList & patternList)
{
	m_Patterns.RemoveAll();

	for(int i =0; i < patternList.m_Patterns.GetCount(); i++)
	{
		m_Patterns.Add(patternList.m_Patterns.GetAt(i));
	}

	return *this;
}
