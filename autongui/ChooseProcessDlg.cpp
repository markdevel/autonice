// ProcessPropDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "autongui.h"
#include "ChooseProcessDlg.h"
#include "PriorityMap.h"
#include "resource.h"
#include "stlext.h"
#include "Toolhelp32Snapshot.h"

// CChooseProcessDlg ダイアログ

IMPLEMENT_DYNAMIC(CChooseProcessDlg, CDialog)

CChooseProcessDlg::CChooseProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChooseProcessDlg::IDD, pParent)
{
}

CChooseProcessDlg::~CChooseProcessDlg()
{
}

void CChooseProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CChooseProcessDlg, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_PROCESS_LIST, &CChooseProcessDlg::OnItemChanged)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_PROCESS_LIST, &CChooseProcessDlg::OnItemActivate)
END_MESSAGE_MAP()

// CChooseProcessDlg メッセージ ハンドラー

BOOL CChooseProcessDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CListCtrl* ctl = static_cast<CListCtrl*>(GetDlgItem(IDC_PROCESS_LIST));
	ctl->SetExtendedStyle(LVS_EX_FULLROWSELECT);
	LVCOLUMN col = { LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM };
	CString label;
	label.LoadString(IDS_PROCESSLIST_COLUMN_NAME1);
	col.pszText = const_cast<LPTSTR>(static_cast<LPCTSTR>(label));
	col.cx = 400;
	ctl->InsertColumn(0, &col);

	CToolhelp32Snapshot processes;
	std::vector<std::wstring> v;
	for (auto iter : processes)
	{
		if (iter.th32ProcessID > 4)
		{
			v.emplace_back(iter.szExeFile);
		}
	}
	std::sort(v.begin(), v.end(), case_insensitive_less<std::wstring>());
	v.erase(std::unique(v.begin(), v.end()), v.end());

	LVITEMW item = { LVIF_STATE | LVIF_TEXT };
	std::for_each(std::rbegin(v), std::rend(v), [&](const auto & e) {
		item.pszText = const_cast<LPWSTR>(e.c_str());
		ctl->InsertItem(&item);
	});

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CChooseProcessDlg::OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	GetDlgItem(IDOK)->EnableWindow(TRUE);
}

void CChooseProcessDlg::OnItemActivate(NMHDR* pNMHDR, LRESULT* pResult)
{
	OnOK();
}

void CChooseProcessDlg::OnOK()
{
	UpdateData(TRUE);
	CListCtrl* ctl = static_cast<CListCtrl*>(GetDlgItem(IDC_PROCESS_LIST));
	POSITION pos = ctl->GetFirstSelectedItemPosition();
	int i = ctl->GetNextSelectedItem(pos);
	Model = ctl->GetItemText(i, 0);
	CDialog::OnOK();
}
