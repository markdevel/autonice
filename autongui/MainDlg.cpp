
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
#include "findindex.h"

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
	ON_WM_SYSCOMMAND()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_MAIN_PROCESS_CONFIG_APPEND, &CMainDlg::OnBnClickedAppend)
	ON_BN_CLICKED(IDC_MAIN_PROCESS_CONFIG_MODIFY, &CMainDlg::OnBnClickedModify)
	ON_BN_CLICKED(IDC_MAIN_PROCESS_CONFIG_DELETE, &CMainDlg::OnBnClickedDelete)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_MAIN_PROCESS_CONFIG_LIST, &CMainDlg::OnItemChangedProcessConfigList)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_MAIN_PROCESS_CONFIG_LIST, &CMainDlg::OnGetDispInfoProcessConfigList)
	ON_NOTIFY(NM_CLICK, IDC_MAIN_COPYRIGHT_URL, &CMainDlg::OnClickCopyrightUrl)
	ON_NOTIFY(NM_DBLCLK, IDC_MAIN_PROCESS_CONFIG_LIST, &CMainDlg::OnDblclkProcessConfigList)
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
		appConfig.Process.emplace(std::make_pair(propDlg.Model.Filename, propDlg.Model));
		ctl->SetItemCountEx((int)appConfig.Process.size(), LVSICF_NOINVALIDATEALL);
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
	int index = ctl->GetNextSelectedItem(pos);

	// 変更ダイアログの表示
	CProcessPropDlg propDlg;
	propDlg.Model = std::next(appConfig.Process.begin(), index)->second;
	INT_PTR rv = propDlg.DoModal();
	if (IDOK == rv)
	{
		// リストの更新
		ctl->Update(index);
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
	int index = ctl->GetNextSelectedItem(pos);

	// リストから削除
	appConfig.Process.erase(std::next(appConfig.Process.begin(), index));
	ctl->SetItemCountEx((int)appConfig.Process.size(), LVSICF_NOINVALIDATEALL);
}

void CMainDlg::OnOK()
{
	CListCtrl* ctl = static_cast<CListCtrl*>(GetDlgItem(IDC_MAIN_PROCESS_CONFIG_LIST));

	// レジストリに保存
	CRegistrySerializer regWriter;
	regWriter.Save(HKEY_LOCAL_MACHINE, APP_REG_KEY_ROOT, appConfig);

	CDialogEx::OnOK();
}

void CMainDlg::OnGetDispInfoProcessConfigList(NMHDR * pNMHDR, LRESULT * pResult)
{
	NMLVDISPINFO* pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	CListCtrl* ctl = static_cast<CListCtrl*>(GetDlgItem(IDC_MAIN_PROCESS_CONFIG_LIST));
	CString text;
	if (pDispInfo->item.mask & LVIF_TEXT)
	{
		int i = pDispInfo->item.iItem;
		auto iter = std::next(appConfig.Process.begin(), i);
		switch (pDispInfo->item.iSubItem)
		{
		case 0:
			StringCchCopy(pDispInfo->item.pszText, pDispInfo->item.cchTextMax, iter->second.Filename.c_str());
			break;

		case 1:
			text.LoadString(g_PriorityResourceStringMap[find_index(g_PriorityClassMap, iter->second.PriorityClass)]);
			StringCchCopy(pDispInfo->item.pszText, pDispInfo->item.cchTextMax, text);
			break;

		case 2:
			text.LoadString(iter->second.Enable ? IDS_YES : IDS_NO);
			StringCchCopy(pDispInfo->item.pszText, pDispInfo->item.cchTextMax, text);
			break;
		}
	}
	*pResult = 0;
}

void CMainDlg::OnItemChangedProcessConfigList(NMHDR * pNMHDR, LRESULT * pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	CListCtrl* ctl = static_cast<CListCtrl*>(GetDlgItem(IDC_MAIN_PROCESS_CONFIG_LIST));
	POSITION pos = ctl->GetFirstSelectedItemPosition();
	BOOL selected = (NULL != pos);
	CWnd * ctlModify = GetDlgItem(IDC_MAIN_PROCESS_CONFIG_MODIFY);
	ctlModify->EnableWindow(selected);
	CWnd * ctlDelete = GetDlgItem(IDC_MAIN_PROCESS_CONFIG_DELETE);
	ctlDelete->EnableWindow(selected);
	*pResult = 0;
}

void CMainDlg::OnDblclkProcessConfigList(NMHDR * pNMHDR, LRESULT * pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CListCtrl* ctl = static_cast<CListCtrl*>(GetDlgItem(IDC_MAIN_PROCESS_CONFIG_LIST));
	int i = pNMItemActivate->iItem;
	if (i >= 0)
	{
		// 変更ダイアログの表示
		CProcessPropDlg propDlg;
		propDlg.Model = std::next(appConfig.Process.begin(), i)->second;
		INT_PTR rv = propDlg.DoModal();
		if (IDOK == rv)
		{
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
		;
	}
}
