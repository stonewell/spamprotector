// SpamProtectorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpamProtector.h"
#include "SpamProtectorDlg.h"
#include "EditPatternListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSpamProtectorDlg dialog

CSpamProtectorDlg::CSpamProtectorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSpamProtectorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSpamProtectorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_CHK_CONTACTS, theApp.m_Configuration.m_bAlwaysTrustContacts);
	DDX_Check(pDX, IDC_CHK_WHITE_LIST, theApp.m_Configuration.m_bUseWhiteList);
	DDX_Check(pDX, IDC_CHK_BLACK_LIST, theApp.m_Configuration.m_bUseBlackList);
	DDX_Check(pDX, IDC_CHK_CONTENT_FILTER, theApp.m_Configuration.m_bUseContentList);
}

BEGIN_MESSAGE_MAP(CSpamProtectorDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_WHITE_LIST, &CSpamProtectorDlg::OnBnClickedBtnWhiteList)
	ON_BN_CLICKED(IDC_BTN_BLACK_LIST, &CSpamProtectorDlg::OnBnClickedBtnBlackList)
	ON_BN_CLICKED(IDC_BTN_CONTENT_FILTER, &CSpamProtectorDlg::OnBnClickedBtnContentFilter)
	ON_BN_CLICKED(IDC_BTN_BLOCKED_SMS, &CSpamProtectorDlg::OnBnClickedBtnBlockedSms)
	ON_BN_CLICKED(IDC_BTN_BLOCKED_INCOMING_CALL, &CSpamProtectorDlg::OnBnClickedBtnBlockedIncomingCall)
	ON_BN_CLICKED(IDC_CHK_CONTACTS, &CSpamProtectorDlg::OnBnClickedChkContacts)
	ON_BN_CLICKED(IDC_CHK_WHITE_LIST, &CSpamProtectorDlg::OnBnClickedChkWhiteList)
	ON_BN_CLICKED(IDC_CHK_BLACK_LIST, &CSpamProtectorDlg::OnBnClickedChkBlackList)
	ON_BN_CLICKED(IDC_CHK_CONTENT_FILTER, &CSpamProtectorDlg::OnBnClickedChkContentFilter)
	ON_COMMAND(ID_OPTIONS_ALWAYSTRUSTCONTACTS, &CSpamProtectorDlg::OnOptionsAlwaystrustcontacts)
	ON_WM_INITMENUPOPUP()
	ON_COMMAND(ID_OPTIONS_USEWHITELIST, &CSpamProtectorDlg::OnOptionsUsewhitelist)
	ON_COMMAND(ID_OPTIONS_EDITWHITELIST, &CSpamProtectorDlg::OnOptionsEditwhitelist)
	ON_COMMAND(ID_OPTIONS_USEBLACKLIST, &CSpamProtectorDlg::OnOptionsUseblacklist)
	ON_COMMAND(ID_OPTIONS_EDITBLACKLIST, &CSpamProtectorDlg::OnOptionsEditblacklist)
	ON_COMMAND(ID_OPTIONS_USECONTENTFILTER, &CSpamProtectorDlg::OnOptionsUsecontentfilter)
	ON_COMMAND(ID_OPTIONS_EDITCONTENTFILTER, &CSpamProtectorDlg::OnOptionsEditcontentfilter)
	ON_COMMAND(ID_OPTIONS_VIEWBLOCKEDSMS, &CSpamProtectorDlg::OnOptionsViewblockedsms)
	ON_COMMAND(ID_OPTIONS_VIEWBLOCKEDINCOMINGCALLS, &CSpamProtectorDlg::OnOptionsViewblockedincomingcalls)
	ON_COMMAND(ID_FILE_SAVECLOSE, &CSpamProtectorDlg::OnFileSaveclose)
	ON_COMMAND(ID_FILE_SAVE_ALL, &CSpamProtectorDlg::OnFileSaveAll)
END_MESSAGE_MAP()


// CSpamProtectorDlg message handlers

BOOL CSpamProtectorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

#ifdef WIN32_PLATFORM_WFSP
	if (!m_dlgCommandBar.Create(this) ||
	    !m_dlgCommandBar.InsertMenuBar(IDR_SPAM_PROTECTOR))
	{
		TRACE0("Failed to create CommandBar\n");
		return FALSE;      // fail to create
	}
#endif // WIN32_PLATFORM_WFSP

	OnBnClickedChkWhiteList();
	OnBnClickedChkBlackList();
	OnBnClickedChkContentFilter();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CSpamProtectorDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_SPAMPROTECTOR_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_SPAMPROTECTOR_DIALOG));
	}
}
#endif


void CSpamProtectorDlg::OnBnClickedBtnWhiteList()
{
	CEditPatternListDlg dlg(_T("Edit White List"), &theApp.m_Configuration.m_WhiteList, this);

	dlg.DoModal();
}

void CSpamProtectorDlg::OnBnClickedBtnBlackList()
{
	CEditPatternListDlg dlg(_T("Edit Black List"), &theApp.m_Configuration.m_BlackList, this);

	dlg.DoModal();
}

void CSpamProtectorDlg::OnBnClickedBtnContentFilter()
{
	CEditPatternListDlg dlg(_T("Edit Content Filter"), &theApp.m_Configuration.m_ContentList, this);

	dlg.DoModal();
}

void CSpamProtectorDlg::OnBnClickedBtnBlockedSms()
{
	// TODO: Add your control notification handler code here
}

void CSpamProtectorDlg::OnBnClickedBtnBlockedIncomingCall()
{
	// TODO: Add your control notification handler code here
}

void CSpamProtectorDlg::OnBnClickedChkContacts()
{
}

void CSpamProtectorDlg::OnBnClickedChkWhiteList()
{
	UpdateData(TRUE);

	CWnd * pWnd = GetDlgItem(IDC_BTN_WHITE_LIST);

	pWnd->EnableWindow(theApp.m_Configuration.m_bUseWhiteList);
}

void CSpamProtectorDlg::OnBnClickedChkBlackList()
{
	UpdateData(TRUE);

	CWnd * pWnd = GetDlgItem(IDC_BTN_BLACK_LIST);

	pWnd->EnableWindow(theApp.m_Configuration.m_bUseBlackList);
}

void CSpamProtectorDlg::OnBnClickedChkContentFilter()
{
	UpdateData(TRUE);

	CWnd * pWnd = GetDlgItem(IDC_BTN_CONTENT_FILTER);

	pWnd->EnableWindow(theApp.m_Configuration.m_bUseContentList);
}

void CSpamProtectorDlg::OnOK()
{
	CDialog::OnOK();
}

void CSpamProtectorDlg::OnOptionsAlwaystrustcontacts()
{
	theApp.m_Configuration.m_bAlwaysTrustContacts = !theApp.m_Configuration.m_bAlwaysTrustContacts;

	UpdateData(FALSE);

	OnBnClickedChkContacts();
}

void CSpamProtectorDlg::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	CDialog::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	UpdateData(TRUE);

	if (theApp.m_Configuration.m_bAlwaysTrustContacts)
	{
		pPopupMenu->CheckMenuItem(ID_OPTIONS_ALWAYSTRUSTCONTACTS, MF_CHECKED | MF_BYCOMMAND); 
	}
	else
	{
		pPopupMenu->CheckMenuItem(ID_OPTIONS_ALWAYSTRUSTCONTACTS, MF_UNCHECKED | MF_BYCOMMAND); 
	}

	if (theApp.m_Configuration.m_bUseWhiteList)
	{
		pPopupMenu->CheckMenuItem(ID_OPTIONS_USEWHITELIST, MF_CHECKED | MF_BYCOMMAND); 
		pPopupMenu->EnableMenuItem(ID_OPTIONS_EDITWHITELIST, MF_ENABLED | MF_BYCOMMAND);
	}
	else
	{
		pPopupMenu->CheckMenuItem(ID_OPTIONS_USEWHITELIST, MF_UNCHECKED | MF_BYCOMMAND); 
		pPopupMenu->EnableMenuItem(ID_OPTIONS_EDITWHITELIST, MF_GRAYED | MF_BYCOMMAND);
	}

	if (theApp.m_Configuration.m_bUseBlackList)
	{
		pPopupMenu->CheckMenuItem(ID_OPTIONS_USEBLACKLIST, MF_CHECKED | MF_BYCOMMAND); 
		pPopupMenu->EnableMenuItem(ID_OPTIONS_EDITBLACKLIST, MF_ENABLED | MF_BYCOMMAND);
	}
	else
	{
		pPopupMenu->CheckMenuItem(ID_OPTIONS_USEBLACKLIST, MF_UNCHECKED | MF_BYCOMMAND); 
		pPopupMenu->EnableMenuItem(ID_OPTIONS_EDITBLACKLIST, MF_GRAYED | MF_BYCOMMAND);
	}

	if (theApp.m_Configuration.m_bUseContentList)
	{
		pPopupMenu->CheckMenuItem(ID_OPTIONS_USECONTENTFILTER, MF_CHECKED | MF_BYCOMMAND); 
		pPopupMenu->EnableMenuItem(ID_OPTIONS_EDITCONTENTFILTER, MF_ENABLED | MF_BYCOMMAND);
	}
	else
	{
		pPopupMenu->CheckMenuItem(ID_OPTIONS_USECONTENTFILTER, MF_UNCHECKED | MF_BYCOMMAND); 
		pPopupMenu->EnableMenuItem(ID_OPTIONS_EDITCONTENTFILTER, MF_GRAYED | MF_BYCOMMAND);
	}
}

void CSpamProtectorDlg::OnOptionsUsewhitelist()
{
	theApp.m_Configuration.m_bUseWhiteList = !theApp.m_Configuration.m_bUseWhiteList;

	UpdateData(FALSE);

	OnBnClickedChkWhiteList();
}

void CSpamProtectorDlg::OnOptionsEditwhitelist()
{
	OnBnClickedBtnWhiteList();
}

void CSpamProtectorDlg::OnOptionsUseblacklist()
{
	theApp.m_Configuration.m_bUseBlackList = !theApp.m_Configuration.m_bUseBlackList;

	UpdateData(FALSE);

	OnBnClickedChkBlackList();
}

void CSpamProtectorDlg::OnOptionsEditblacklist()
{
	OnBnClickedBtnBlackList();
}

void CSpamProtectorDlg::OnOptionsUsecontentfilter()
{
	theApp.m_Configuration.m_bUseContentList = !theApp.m_Configuration.m_bUseContentList;

	UpdateData(FALSE);

	OnBnClickedChkContentFilter();
}

void CSpamProtectorDlg::OnOptionsEditcontentfilter()
{
	OnBnClickedBtnContentFilter();
}

void CSpamProtectorDlg::OnOptionsViewblockedsms()
{
	OnBnClickedBtnBlockedSms();
}

void CSpamProtectorDlg::OnOptionsViewblockedincomingcalls()
{
	OnBnClickedBtnBlockedIncomingCall();
}

void CSpamProtectorDlg::OnFileSaveclose()
{
	OnFileSaveAll();

	CDialog::OnOK();
}

void CSpamProtectorDlg::OnFileSaveAll()
{
	UpdateData(TRUE);

	if (!theApp.m_Configuration.Store())
	{
		AfxMessageBox(_T("Store Fail"));
	}
}
