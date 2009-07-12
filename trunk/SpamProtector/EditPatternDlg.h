#pragma once

#include "Pattern.h"
// CEditPatternDlg dialog

class CEditPatternDlg : public CDialog
{
public:
	CEditPatternDlg(CPattern * pPattern, CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditPatternDlg();

// Dialog Data
	enum { IDD = IDD_EDIT_PATTERN_DIALOG };

#ifdef WIN32_PLATFORM_WFSP
protected:  // control bar embedded members
	CCommandBar m_dlgCommandBar;
#endif // WIN32_PLATFORM_WFSP

	CPattern m_BackupPattern;
	CPattern * m_pPattern;
	CListBox m_ctrlActionList;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

	afx_msg void OnMenuAddaction();
	afx_msg void OnMenuEditaction();
	afx_msg void OnMenuRemoveaction();
	afx_msg void OnMenuRemoveallactions();
protected:
	virtual void OnOK();
};
