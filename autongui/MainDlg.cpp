
// autonguiDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "autongui.h"
#include "MainDlg.h"
#include "ProcessPropDlg.h"
#include "ServicePropDlg.h"
#include "PriorityMap.h"
#include "Resource.h"
#include "RegistrySerializer.h"
#include "stlext.h"

#define APP_REG_KEY_ROOT _T("Software\\markdevel\\autonice")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainDlg ダイアログ

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_MAIN_PROCESS_CONFIG_APPEND, &CMainDlg::OnBnClickedAppend)
	ON_BN_CLICKED(IDC_MAIN_PROCESS_CONFIG_MODIFY, &CMainDlg::OnBnClickedModify)
	ON_BN_CLICKED(IDC_MAIN_PROCESS_CONFIG_DELETE, &CMainDlg::OnBnClickedDelete)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_MAIN_PROCESS_CONFIG_LIST, &CMainDlg::OnItemChanged)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_MAIN_PROCESS_CONFIG_LIST, &CMainDlg::OnGetDispInfo)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_MAIN_PROCESS_CONFIG_LIST, &CMainDlg::OnItemActivate)
	ON_NOTIFY(NM_CLICK, IDC_MAIN_COPYRIGHT_URL, &CMainDlg::OnClickCopyrightUrl)
	ON_BN_CLICKED(IDC_MAIN_SHOW_SERVICE_PROP, &CMainDlg::OnClickedShowServiceProp)
END_MESSAGE_MAP()


// CMainDlg メッセージ ハンドラー

BOOL CMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// レジストリの読み込み
	CRegistrySerializer regReader;
	appConfig = regReader.Load(HKEY_LOCAL_MACHINE, APP_REG_KEY_ROOT);

	// リストビューの初期化
	CString label;
	CListCtrl* ctl = static_cast<CListCtrl*>(GetDlgItem(IDC_MAIN_PROCESS_CONFIG_LIST));
	ctl->SetExtendedStyle(LVS_EX_FULLROWSELECT);
	LVCOLUMN col;
	ZeroMemory(&col, sizeof(col));
	col.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	label.LoadString(IDS_PROCESSLIST_COLUMN_NAME1);
	col.pszText = const_cast<LPTSTR>(static_cast<LPCTSTR>(label));
	col.cx = 300;
	ctl->InsertColumn(0, &col);
	label.LoadString(IDS_PROCESSLIST_COLUMN_NAME2);
	col.pszText = const_cast<LPTSTR>(static_cast<LPCTSTR>(label));
	col.cx = 60;
	ctl->InsertColumn(1, &col);
	label.LoadString(IDS_PROCESSLIST_COLUMN_NAME3);
	col.pszText = const_cast<LPTSTR>(static_cast<LPCTSTR>(label));
	col.cx = 60;
	ctl->InsertColumn(2, &col);
	ctl->SetItemCountEx((int)appConfig.Process.size(), LVSICF_NOINVALIDATEALL);

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CMainDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CMainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMainDlg::OnBnClickedAppend()
{
	CListCtrl* ctl = static_cast<CListCtrl*>(GetDlgItem(IDC_MAIN_PROCESS_CONFIG_LIST));

	// 新規作成ダイアログの表示
	CProcessPropDlg propDlg;
	INT_PTR rv = propDlg.DoModal();
	if (IDOK == rv)
	{
		// リストに追加
		appConfig.Process.emplace(propDlg.Model.Filename, propDlg.Model);
		ctl->SetItemCountEx((int)appConfig.Process.size(), 0);
	}
}

void CMainDlg::OnBnClickedModify()
{
	CListCtrl* ctl = static_cast<CListCtrl*>(GetDlgItem(IDC_MAIN_PROCESS_CONFIG_LIST));
	POSITION pos = ctl->GetFirstSelectedItemPosition();
	if (NULL == pos)
	{
		return;
	}
	int i = ctl->GetNextSelectedItem(pos);
	auto iter = std::next(appConfig.Process.begin(), i);

	// 変更ダイアログの表示
	CProcessPropDlg propDlg;
	propDlg.Model = iter->second;
	INT_PTR rv = propDlg.DoModal();
	if (IDOK == rv)
	{
		// モデルの更新
		appConfig.Process.erase(iter);
		appConfig.Process.emplace(propDlg.Model.Filename, propDlg.Model);

		// リストの更新
		ctl->Update(i);
	}
}

void CMainDlg::OnBnClickedDelete()
{
	CListCtrl* ctl = static_cast<CListCtrl*>(GetDlgItem(IDC_MAIN_PROCESS_CONFIG_LIST));
	POSITION pos = ctl->GetFirstSelectedItemPosition();
	if (NULL == pos)
	{
		return;
	}
	int i = ctl->GetNextSelectedItem(pos);
	auto iter = std::next(appConfig.Process.begin(), i);

	// リストから削除
	appConfig.Process.erase(iter);
	ctl->SetItemCountEx((int)appConfig.Process.size(), 0);
}

void CMainDlg::OnOK()
{
	// レジストリに保存
	CRegistrySerializer regWriter;
	regWriter.Save(HKEY_LOCAL_MACHINE, APP_REG_KEY_ROOT, appConfig);

	CDialogEx::OnOK();
}

void CMainDlg::OnGetDispInfo(NMHDR * pNMHDR, LRESULT * pResult)
{
	NMLVDISPINFO* pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	if (pDispInfo->item.mask & LVIF_TEXT)
	{
		CString text;
		int i = pDispInfo->item.iItem;
		auto iter = std::next(appConfig.Process.begin(), i);
		switch (pDispInfo->item.iSubItem)
		{
		case 0:
			StringCchCopy(pDispInfo->item.pszText, pDispInfo->item.cchTextMax, iter->second.Filename.c_str());
			break;

		case 1:
			::AfxLoadString(g_PriorityResourceStringMap[find_index(g_PriorityClassMap, iter->second.PriorityClass)], pDispInfo->item.pszText, pDispInfo->item.cchTextMax);
			break;

		case 2:
			::AfxLoadString(iter->second.Enable ? IDS_YES : IDS_NO, pDispInfo->item.pszText, pDispInfo->item.cchTextMax);
			break;
		}
	}
	*pResult = 0;
}

void CMainDlg::OnItemChanged(NMHDR * pNMHDR, LRESULT * pResult)
{
	CListCtrl* ctl = static_cast<CListCtrl*>(GetDlgItem(IDC_MAIN_PROCESS_CONFIG_LIST));
	POSITION pos = ctl->GetFirstSelectedItemPosition();
	BOOL selected = (NULL != pos);
	GetDlgItem(IDC_MAIN_PROCESS_CONFIG_MODIFY)->EnableWindow(selected);
	GetDlgItem(IDC_MAIN_PROCESS_CONFIG_DELETE)->EnableWindow(selected);
	*pResult = 0;
}

void CMainDlg::OnItemActivate(NMHDR * pNMHDR, LRESULT * pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CListCtrl* ctl = static_cast<CListCtrl*>(GetDlgItem(IDC_MAIN_PROCESS_CONFIG_LIST));
	int i = pNMItemActivate->iItem;
	if (i >= 0)
	{
		auto iter = std::next(appConfig.Process.begin(), i);

		// 変更ダイアログの表示
		CProcessPropDlg propDlg;
		propDlg.Model = iter->second;
		INT_PTR rv = propDlg.DoModal();
		if (IDOK == rv)
		{
			// モデルの更新
			appConfig.Process.erase(iter);
			appConfig.Process.emplace(propDlg.Model.Filename, propDlg.Model);

			// リストの更新
			ctl->Update(i);
		}
	}
	*pResult = 0;
}

void CMainDlg::OnClickCopyrightUrl(NMHDR * pNMHDR, LRESULT * pResult)
{
	PNMLINK pNMLink = (PNMLINK)pNMHDR;
	ShellExecute(m_hWnd, _T("open"), pNMLink->item.szUrl, NULL, NULL, SW_SHOWNORMAL);
	*pResult = 0;
}

void CMainDlg::OnClickedShowServiceProp()
{
	CServicePropDlg propDlg;
	propDlg.Model = appConfig.Service;
	INT_PTR rv = propDlg.DoModal();
	if (IDOK == rv)
	{
		// モデルの更新
		appConfig.Service = propDlg.Model;
	}
}
