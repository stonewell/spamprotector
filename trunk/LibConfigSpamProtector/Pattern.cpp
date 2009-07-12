#include "StdAfx.h"
#include "Pattern.h"

IMPLEMENT_SERIAL(CPattern, CObject, 1)

CPattern::CPattern(void)
{
}

CPattern::CPattern(const CPattern & pattern)
{
	*this = pattern;
}

CPattern::~CPattern(void)
{
}

void CPattern::Serialize( CArchive& archive )
{
	if (archive.IsStoring())
	{
		m_Actions.Serialize(archive);
		archive << m_strPattern << m_bMatchPattern;
	}
	else
	{
		m_Actions.RemoveAll();
		m_Actions.Serialize(archive);
		archive >> m_strPattern >> m_bMatchPattern;
	}
}

CPattern & CPattern::operator = (const CPattern & pattern)
{
	m_Actions.RemoveAll();

	for(int i=0;i < pattern.m_Actions.GetCount(); i++)
	{
		m_Actions.Add(pattern.m_Actions.GetAt(i));
	}

	m_strPattern = pattern.m_strPattern;
	m_bMatchPattern = pattern.m_bMatchPattern;
	
	return *this;
}
