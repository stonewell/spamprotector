#include "StdAfx.h"
#include "Action.h"

IMPLEMENT_SERIAL(CAction, CObject, 1)

CAction::CAction(void)
{
}

CAction::CAction(const CAction & action) :	m_ActionType(action.m_ActionType)
,m_strDestFolder(action.m_strDestFolder)
,m_strReplySms(action.m_strReplySms)
,m_strDestSmsNumber(action.m_strDestSmsNumber)
{
}

CAction::~CAction(void)
{
}

void CAction::Serialize(CArchive & ar)
{
	if (ar.IsStoring())
	{
		LONG actionType = m_ActionType;

		ar << actionType << m_strDestFolder
			<< m_strReplySms
			<< m_strDestSmsNumber;
	}
	else
	{
		LONG actionType = SpamProtector::NoAction;

		ar >> actionType >> m_strDestFolder
			>> m_strReplySms
			>> m_strDestSmsNumber;

		m_ActionType = (SpamProtector::ActionTypeEnum)actionType;
	}
}

CAction & CAction::operator = (const CAction & action)
{
	m_ActionType = action.m_ActionType;
	m_strDestFolder = action.m_strDestFolder;
	m_strReplySms = action.m_strReplySms;
	m_strDestSmsNumber = action.m_strDestSmsNumber;

	return *this;
}
