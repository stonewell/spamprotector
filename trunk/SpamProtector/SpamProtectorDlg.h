// SpamProtectorDlg.h : header file
//

#pragma once

// CSpamProtectorDlg dialog
class CSpamProtectorDlg : public CDialog
{
// Construction
public:
	CSpamProtectorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SPAMPROTECTOR_DIALOG };

#ifdef WIN32_PLATFORM_WFSP
protected:  // control bar embedded members
	CCommandBar m_dlgCommandBar;
#endif // WIN32_PLATFORM_WFSP

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnWhiteList();
	afx_msg void OnBnClickedBtnBlackList();
	afx_msg void OnBnClickedBtnContentFilter();
	afx_msg void OnBnClickedBtnBlockedSms();
	afx_msg void OnBnClickedBtnBlockedIncomingCall();
	afx_msg void OnBnClickedChkContacts();
	afx_msg void OnBnClickedChkWhiteList();
	afx_msg void OnBnClickedChkBlackList();
	afx_msg void OnBnClickedChkContentFilter();
protected:
	virtual void OnOK();
public:
	afx_msg void OnOptionsAlwaystrustcontacts();
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnOptionsUsewhitelist();
	afx_msg void OnOptionsEditwhitelist();
	afx_msg void OnOptionsUseblacklist();
	afx_msg void OnOptionsEditblacklist();
	afx_msg void OnOptionsUsecontentfilter();
	afx_msg void OnOptionsEditcontentfilter();
	afx_msg void OnOptionsViewblockedsms();
	afx_msg void OnOptionsViewblockedincomingcalls();
	afx_msg void OnFileSaveclose();
	afx_msg void OnFileSaveAll();
};
