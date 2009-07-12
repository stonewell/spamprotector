#pragma once

#include "Action.h"
// CEditActionDlg dialog

class CEditActionDlg : public CDialog
{
public:
	CEditActionDlg(CAction * pAction, CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditActionDlg();

// Dialog Data
	enum { IDD = IDD_EDIT_ACTION_DIALOG };

#ifdef WIN32_PLATFORM_WFSP
protected:  // control bar embedded members
	CCommandBar m_dlgCommandBar;
#endif // WIN32_PLATFORM_WFSP

	CAction m_BackupAction;
	CAction * m_pAction;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

protected:
	virtual void OnOK();
};
