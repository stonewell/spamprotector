// EditActionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpamProtector.h"
#include "EditActionDlg.h"


// CEditActionDlg dialog
CEditActionDlg::CEditActionDlg(CAction * pAction, CWnd* pParent /*=NULL*/)
	: CDialog(CEditActionDlg::IDD, pParent)
	, m_pAction(pAction)
	, m_BackupAction(*pAction)
{

}

CEditActionDlg::~CEditActionDlg()
{
}

void CEditActionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEditActionDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
END_MESSAGE_MAP()

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CEditActionDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_EDIT_ACTION_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_EDIT_ACTION_DIALOG));
	}
}
#endif

// CEditActionDlg message handlers
BOOL CEditActionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

#ifdef WIN32_PLATFORM_WFSP
	if (!m_dlgCommandBar.Create(this) ||
	    !m_dlgCommandBar.InsertMenuBar(IDR_EDIT_ACTION))
	{
		TRACE0("Failed to create CommandBar\n");
		return FALSE;      // fail to create
	}
#endif // WIN32_PLATFORM_WFSP

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CEditActionDlg::OnOK()
{
	UpdateData(TRUE);

	*m_pAction = m_BackupAction;

	CDialog::OnOK();
}
