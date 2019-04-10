// ProcessPropDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "autongui.h"
#include "ProcessPropDlg.h"
#include "afxdialogex.h"
#include "PriorityMap.h"
#include "resource.h"
#include "findindex.h"

// CProcessPropDlg �_�C�A���O

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
	ON_BN_CLICKED(IDC_PROCESS_EXE_FILE_NAME_BROWSE, &CProcessPropDlg::OnBnClickedProcessPropBrowse)
END_MESSAGE_MAP()

// CProcessPropDlg ���b�Z�[�W �n���h���[

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
	GetDlgItem(IDC_PROCESS_EXE_FILE_NAME)->SetWindowTextW(path);
}

BOOL CProcessPropDlg::OnInitDialog()
{
	// �R���{�{�b�N�X�̏�����
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
	CDialog::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
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
