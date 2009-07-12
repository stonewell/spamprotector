#pragma once

namespace SpamProtector
{
	enum ActionTypeEnum
	{
		NoAction,
		Delete,
		Move,
		Reject,
		Mute,
		ReplySMS,
		ForwardSMS,
		AddToWhiteList,
		AddToBlackList
	};
};

class CAction : public CObject
{
public:
	DECLARE_SERIAL(CAction)

	CAction(void);
	CAction(const CAction & action);
	virtual ~CAction(void);

	virtual void Serialize(CArchive & ar);

	virtual CAction & operator = (const CAction & action);
public:
	SpamProtector::ActionTypeEnum m_ActionType;

	CString m_strDestFolder;
	CString m_strReplySms;

	CString m_strDestSmsNumber;
};

class CActionList : public CArray<CAction>
{
};

