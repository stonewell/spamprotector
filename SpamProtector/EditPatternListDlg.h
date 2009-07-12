#pragma once


#include "PatternList.h"
#include "afxwin.h"
// CEditPatternListDlg dialog

class CEditPatternListDlg : public CDialog
{
public:
	CEditPatternListDlg(const CString & strTitle, 
		CPatternList * pPatternList,
		CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditPatternListDlg();

// Dialog Data
	enum { IDD = IDD_EDIT_PATTERN_LIST_DIALOG };

#ifdef WIN32_PLATFORM_WFSP
protected:  // control bar embedded members
	CCommandBar m_dlgCommandBar;
#endif // WIN32_PLATFORM_WFSP

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

private:
	CString m_strWindowTitle;
	CPatternList * m_pOriginalList;
	CPatternList m_BackupList;

public:
	afx_msg void OnEditAddpattern();
	afx_msg void OnEditEditpattern();
	afx_msg void OnEditRemovepattern();
	afx_msg void OnEditRemoveall();
	afx_msg void OnMenuSaveclose();

	CListBox m_ctrlPatternList;
protected:
	void InsertPattern(CPattern * pPattern);
};
