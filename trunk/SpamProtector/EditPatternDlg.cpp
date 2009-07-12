// EditPatternDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpamProtector.h"
#include "EditPatternDlg.h"


// CEditPatternDlg dialog
CEditPatternDlg::CEditPatternDlg(CPattern * pPattern, CWnd* pParent /*=NULL*/)
	: CDialog(CEditPatternDlg::IDD, pParent)
	, m_BackupPattern(*pPattern)
	, m_pPattern(pPattern)
{

}

CEditPatternDlg::~CEditPatternDlg()
{
}

void CEditPatternDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	int nMatch = -1;

	if (!pDX->m_bSaveAndValidate)
	{
		nMatch = m_BackupPattern.m_bMatchPattern ? 0 : 1;
	}

	DDX_Radio(pDX, IDC_RDO_MATCH, nMatch);
	DDX_Text(pDX, IDC_EDT_PATTERN, m_BackupPattern.m_strPattern);
	DDX_Control(pDX, IDC_LST_ACTIONS, m_ctrlActionList);

	if (pDX->m_bSaveAndValidate)
	{
		m_BackupPattern.m_bMatchPattern = nMatch == 0;
	}
}


BEGIN_MESSAGE_MAP(CEditPatternDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	ON_COMMAND(ID_MENU_ADDACTION, &CEditPatternDlg::OnMenuAddaction)
	ON_COMMAND(ID_MENU_EDITACTION, &CEditPatternDlg::OnMenuEditaction)
	ON_COMMAND(ID_MENU_REMOVEACTION, &CEditPatternDlg::OnMenuRemoveaction)
	ON_COMMAND(ID_MENU_REMOVEALLACTIONS, &CEditPatternDlg::OnMenuRemoveallactions)
END_MESSAGE_MAP()


// CEditPatternDlg message handlers
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CEditPatternDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_EDIT_PATTERN_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_EDIT_PATTERN_DIALOG));
	}
}
#endif

BOOL CEditPatternDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

#ifdef WIN32_PLATFORM_WFSP
	if (!m_dlgCommandBar.Create(this) ||
	    !m_dlgCommandBar.InsertMenuBar(IDR_EDIT_PATTERN))
	{
		TRACE0("Failed to create CommandBar\n");
		return FALSE;      // fail to create
	}
#endif // WIN32_PLATFORM_WFSP

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CEditPatternDlg::OnMenuAddaction()
{
	// TODO: Add your command handler code here
}

void CEditPatternDlg::OnMenuEditaction()
{
	// TODO: Add your command handler code here
}

void CEditPatternDlg::OnMenuRemoveaction()
{
	// TODO: Add your command handler code here
}

void CEditPatternDlg::OnMenuRemoveallactions()
{
	// TODO: Add your command handler code here
}

void CEditPatternDlg::OnOK()
{
	UpdateData(TRUE);

	*m_pPattern = m_BackupPattern;

	CDialog::OnOK();
}
