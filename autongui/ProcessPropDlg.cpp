// ProcessPropDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "autongui.h"
#include "ProcessPropDlg.h"
#include "PriorityMap.h"
#include "resource.h"
#include "stlext.h"
#include "ChooseProcessDlg.h"
#include "Win32Exception.h"

// CProcessPropDlg ダイアログ

IMPLEMENT_DYNAMIC(CProcessPropDlg, CDialog)

CProcessPropDlg::CProcessPropDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProcessPropDlg::IDD, pParent)
	, Enable(true)
	, PriorityIndex(2)
{
}

CProcessPropDlg::~CProcessPropDlg()
{
}

void CProcessPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_PROCESS_WATCHING_ENABLE, Enable);
	DDX_Text(pDX, IDC_PROCESS_EXE_FILE_NAME, ExeFilePath);
	DDV_MaxChars(pDX, ExeFilePath, MAX_PATH);
	DDX_CBIndex(pDX, IDC_PROCESS_PRIORITY, PriorityIndex);
}

BEGIN_MESSAGE_MAP(CProcessPropDlg, CDialog)
	ON_BN_CLICKED(IDC_FILESYSTEM_FILENAME_BROWSE, &CProcessPropDlg::OnBnClickedFilesystemBrowse)
	ON_BN_CLICKED(IDC_PROCESS_FILENAME_BROWSE, &CProcessPropDlg::OnBnClickedProcessPropBrowse)
	ON_EN_UPDATE(IDC_PROCESS_EXE_FILE_NAME, &CProcessPropDlg::OnUpdateFilename)
END_MESSAGE_MAP()

// CProcessPropDlg メッセージ ハンドラー

void CProcessPropDlg::OnBnClickedFilesystemBrowse()
{
	ATL::CComPtr<IFileDialog> dlg;
	ThrowHRESULTIf(dlg.CoCreateInstance(CLSID_FileOpenDialog));
	static constexpr COMDLG_FILTERSPEC filter[] {
		{ L"Executable Files (*.exe; *.com; *.des)", L"*.exe;*.com;*.des" },
		{ L"All Files (*.*)", L"*.*" }
	};
	dlg->SetFileTypes(ARRAYSIZE(filter), filter);
	FILEOPENDIALOGOPTIONS options;
	dlg->GetOptions(&options);
	dlg->SetOptions(options | FOS_FORCEFILESYSTEM | FOS_FILEMUSTEXIST);
	HRESULT hr = dlg->Show(m_hWnd);
	if (SUCCEEDED(hr))
	{
		ATL::CComPtr<IShellItem> item;
		ThrowHRESULTIf(dlg->GetResult(&item));
		PWSTR path = NULL;
		ThrowHRESULTIf(item->GetDisplayName(SIGDN_FILESYSPATH, &path));
		PathStripPathW(path);
		GetDlgItem(IDC_PROCESS_EXE_FILE_NAME)->SetWindowTextW(path);
		CoTaskMemFree(path);
	}
}

void CProcessPropDlg::OnBnClickedProcessPropBrowse()
{
	CChooseProcessDlg dlg;
	dlg.DoModal();
	GetDlgItem(IDC_PROCESS_EXE_FILE_NAME)->SetWindowTextW(dlg.Model);
}

BOOL CProcessPropDlg::OnInitDialog()
{
	// コンボボックスの初期化
	CComboBox* cb1 = static_cast<CComboBox*>(GetDlgItem(IDC_PROCESS_PRIORITY));
	for (UINT i : g_PriorityResourceStringMap)
	{
		CString label;
		label.LoadString(i);
		cb1->AddString(label);
	}
	Enable = Model.Enable == FALSE;
	ExeFilePath = Model.Filename.c_str();
	PriorityIndex = (int)find_index(g_PriorityClassMap, Model.PriorityClass);
	GetDlgItem(IDOK)->EnableWindow(!ExeFilePath.IsEmpty());
	CDialog::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CProcessPropDlg::OnUpdateFilename()
{
	GetDlgItem(IDOK)->EnableWindow((GetDlgItem(IDC_PROCESS_EXE_FILE_NAME)->GetWindowTextLength() > 0));
}

void CProcessPropDlg::OnOK()
{
	UpdateData(TRUE);
	PathStripPath(ExeFilePath.LockBuffer());
	ExeFilePath.ReleaseBuffer();
	Model.Enable = Enable == 0;
	Model.Filename = ExeFilePath;
	Model.PriorityClass = g_PriorityClassMap[PriorityIndex];
	CDialog::OnOK();
}
