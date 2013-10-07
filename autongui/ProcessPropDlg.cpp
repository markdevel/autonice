// ProcessPropDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "autongui.h"
#include "ProcessPropDlg.h"
#include "afxdialogex.h"
#include "PriorityMap.h"
#include "resource.h"

// CProcessPropDlg ダイアログ

IMPLEMENT_DYNAMIC(CProcessPropDlg, CDialog)

CProcessPropDlg::CProcessPropDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProcessPropDlg::IDD, pParent)
{
}

CProcessPropDlg::~CProcessPropDlg()
{
}

void CProcessPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_PROCESS_WATCHING_ENABLE, Watching);
	DDX_Text(pDX, IDC_PROCESS_EXE_FILE_NAME, ExeFilePath);
	DDV_MaxChars(pDX, ExeFilePath, MAX_PATH);
	DDX_CBIndex(pDX, IDC_PROCESS_PRIORITY, PriorityIndex);
}

BEGIN_MESSAGE_MAP(CProcessPropDlg, CDialog)
	ON_BN_CLICKED(IDC_PROCESS_EXE_FILE_NAME_BROWSE, &CProcessPropDlg::OnBnClickedProcessPropBrowse)
END_MESSAGE_MAP()

// CProcessPropDlg メッセージ ハンドラー

void CProcessPropDlg::OnBnClickedProcessPropBrowse()
{
	CString label;
	label.LoadString(IDS_BROWSE_EXE_FILE);
	TCHAR path[MAX_PATH];
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(bi));
	bi.hwndOwner = this->GetSafeHwnd();
	bi.pszDisplayName = path;
	bi.lpszTitle = label;
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_BROWSEINCLUDEFILES | BIF_NONEWFOLDERBUTTON;
	LPITEMIDLIST pidlBrowse = SHBrowseForFolder(&bi);
	if (NULL == pidlBrowse)
	{
		return;
	}
	SHGetPathFromIDList(pidlBrowse, path);
	PathStripPath(path);
	LPTSTR ext = PathFindExtension(path);
	if (NULL == ext)
	{
		::AfxMessageBox(IDS_BAD_EXE_FILE, MB_ICONERROR);
		return;
	}
	if (_tcsicmp(ext, _T(".exe")) != 0
		&& _tcsicmp(ext, _T(".com")) != 0
		&& _tcsicmp(ext, _T(".des")) != 0)
	{
		::AfxMessageBox(IDS_BAD_EXE_FILE, MB_ICONERROR);
		return;
	}
	GetDlgItem(IDC_PROCESS_EXE_FILE_NAME)->SetWindowTextW(path);
}

BOOL CProcessPropDlg::OnInitDialog()
{
	// コンボボックスの初期化
	CComboBox* cb1 = static_cast<CComboBox*>(GetDlgItem(IDC_PROCESS_PRIORITY));
	for (int i = 0; i < 5; ++i){
		CString label;
		label.LoadString(g_PriorityResourceStringMap[i]);
		cb1->AddString(label);
	}
	Watching = model->Enable == FALSE;
	ExeFilePath = model->ExeFilePath;
	PriorityIndex = PriorityClassToIndex(model->PriorityClass);

	CDialog::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CProcessPropDlg::OnOK()
{
	UpdateData(TRUE);

	PathStripPath(ExeFilePath.LockBuffer());
	ExeFilePath.ReleaseBuffer();

	model->Enable = Watching == 0;
	model->ExeFilePath = ExeFilePath;
	model->PriorityClass = g_PriorityClassMap[PriorityIndex];

	CDialog::OnOK();
}
