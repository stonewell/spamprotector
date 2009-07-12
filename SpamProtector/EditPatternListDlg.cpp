// EditPatternListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpamProtector.h"
#include "EditPatternListDlg.h"
#include "EditPatternDlg.h"

// CEditPatternListDlg dialog
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CEditPatternListDlg::CEditPatternListDlg(const CString & strTitle, 
		CPatternList * pPatternList,
		CWnd* pParent /*=NULL*/)
	: CDialog(CEditPatternListDlg::IDD, pParent)
	, m_strWindowTitle(strTitle)
	, m_pOriginalList(pPatternList)
	, m_BackupList(*pPatternList)
{
}

CEditPatternListDlg::~CEditPatternListDlg()
{
}

void CEditPatternListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LST_PATTERNS, m_ctrlPatternList);
}


BEGIN_MESSAGE_MAP(CEditPatternListDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	ON_COMMAND(ID_EDIT_ADDPATTERN, &CEditPatternListDlg::OnEditAddpattern)
	ON_COMMAND(ID_EDIT_EDITPATTERN, &CEditPatternListDlg::OnEditEditpattern)
	ON_COMMAND(ID_EDIT_REMOVEPATTERN, &CEditPatternListDlg::OnEditRemovepattern)
	ON_COMMAND(ID_EDIT_REMOVEALL, &CEditPatternListDlg::OnEditRemoveall)
	ON_COMMAND(ID_MENU_SAVECLOSE, &CEditPatternListDlg::OnMenuSaveclose)
END_MESSAGE_MAP()


// CEditPatternListDlg message handlers
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CEditPatternListDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_EDIT_PATTERN_LIST_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_EDIT_PATTERN_LIST_DIALOG));
	}
}
#endif

BOOL CEditPatternListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

#ifdef WIN32_PLATFORM_WFSP
	if (!m_dlgCommandBar.Create(this) ||
	    !m_dlgCommandBar.InsertMenuBar(IDR_EDIT_PATTERN_LIST))
	{
		TRACE0("Failed to create CommandBar\n");
		return FALSE;      // fail to create
	}
#endif // WIN32_PLATFORM_WFSP

	SetWindowText(m_strWindowTitle);

	UpdateData(FALSE);
	for(int i=0; i < m_BackupList.m_Patterns.GetCount(); i++)
	{
		InsertPattern(&m_BackupList.m_Patterns.GetAt(i));
	}
	UpdateData(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CEditPatternListDlg::OnEditAddpattern()
{
	UpdateData(FALSE);

	CPattern pattern;

	CEditPatternDlg dlg(&pattern, this);

	if (dlg.DoModal() != IDCANCEL)
	{
		int nIndex = m_BackupList.m_Patterns.Add(pattern);

		InsertPattern(&m_BackupList.m_Patterns.GetAt(nIndex));
	}

	UpdateData(TRUE);
}

void CEditPatternListDlg::OnEditEditpattern()
{
	UpdateData(FALSE);

	if (m_ctrlPatternList.GetCurSel() < 0)
		return;

	int nIndex = m_ctrlPatternList.GetCurSel();

	CPattern * pPattern = 
		(CPattern *)m_ctrlPatternList.GetItemDataPtr(nIndex);

	CEditPatternDlg dlg(pPattern, this);

	if (dlg.DoModal() != IDCANCEL)
	{
		m_ctrlPatternList.DeleteString(nIndex);

		CString strText;

		strText.Format(_T("%s %s"),
			pPattern->m_bMatchPattern ? _T("Match") : _T("Not Match"),
			pPattern->m_strPattern);

		m_ctrlPatternList.InsertString(nIndex, strText);
		m_ctrlPatternList.SetItemDataPtr(nIndex, pPattern);
	}

	m_ctrlPatternList.SetCurSel(nIndex);

	UpdateData(TRUE);
}

void CEditPatternListDlg::OnEditRemovepattern()
{
	UpdateData(FALSE);

	if (m_ctrlPatternList.GetCurSel() >= 0)
	{
		int nIndex = m_ctrlPatternList.GetCurSel();

		m_ctrlPatternList.DeleteString(nIndex);
		m_BackupList.m_Patterns.RemoveAt(nIndex);
	}

	UpdateData(TRUE);
}

void CEditPatternListDlg::OnEditRemoveall()
{
	UpdateData(FALSE);
	m_ctrlPatternList.ResetContent();
	m_BackupList.m_Patterns.RemoveAll();
	UpdateData(TRUE);
}

void CEditPatternListDlg::OnMenuSaveclose()
{
	*m_pOriginalList = m_BackupList;

	CDialog::OnOK();
}

void CEditPatternListDlg::InsertPattern(CPattern * pPattern)
{
	CString strText;

	strText.Format(_T("%s %s"),
		pPattern->m_bMatchPattern ? _T("Match") : _T("Not Match"),
		pPattern->m_strPattern);

	int nIndex = m_ctrlPatternList.AddString(strText);

	m_ctrlPatternList.SetItemDataPtr(nIndex, pPattern);

	m_ctrlPatternList.SetCurSel(nIndex);
}
