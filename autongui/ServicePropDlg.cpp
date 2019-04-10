// ServicePropDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "autongui.h"
#include "ServicePropDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "ScopedHandle.h"

// CServicePropDlg ダイアログ

IMPLEMENT_DYNAMIC(CServicePropDlg, CDialogEx)

CServicePropDlg::CServicePropDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CServicePropDlg::IDD, pParent)
{

}

CServicePropDlg::~CServicePropDlg()
{
}

void CServicePropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_SERVICE_PROCESS_CHECK_INTERVAL, Model.ProcessCheckInterval);
	DDV_MinMaxInt(pDX, Model.ProcessCheckInterval, 100, 60 * 60 * 1000);
}


BEGIN_MESSAGE_MAP(CServicePropDlg, CDialogEx)
END_MESSAGE_MAP()


// CServicePropDlg メッセージ ハンドラー

const std::map<DWORD, UINT> CServicePropDlg::statusTextMap {
	{ SERVICE_STOPPED, IDS_SERVICE_STOPPED },
	{ SERVICE_START_PENDING, IDS_SERVICE_START_PENDING },
	{ SERVICE_STOP_PENDING, IDS_SERVICE_STOP_PENDING },
	{ SERVICE_RUNNING, IDS_SERVICE_RUNNING },
	{ SERVICE_CONTINUE_PENDING, IDS_SERVICE_CONTINUE_PENDING },
	{ SERVICE_PAUSE_PENDING, IDS_SERVICE_PAUSE_PENDING },
	{ SERVICE_PAUSED, IDS_SERVICE_PAUSED }
};

BOOL CServicePropDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	auto ctlSpin = reinterpret_cast<CSpinButtonCtrl*>(GetDlgItem(IDC_SERVICE_PROCESS_CHECK_INTERVAL_SPIN));
	ctlSpin->SetRange32(100, 60 * 60 * 1000);
	ctlSpin->SetPos32(Model.ProcessCheckInterval);

	// サービス稼動状態の表示
	SCOPED_SC_HANDLE managerHandle(OpenSCManagerW(NULL, NULL, GENERIC_READ), &::CloseServiceHandle);
	SCOPED_SC_HANDLE serviceHandle(OpenServiceW(managerHandle.get(), L"autonice", GENERIC_READ), &::CloseServiceHandle);
	SERVICE_STATUS status;
	if (QueryServiceStatus(serviceHandle.get(), &status))
	{
		auto iter = statusTextMap.find(status.dwCurrentState);
		auto resId = (iter != statusTextMap.end()) ? iter->second : IDS_SERVICE_UNKNOWN;
		CString statusText;
		statusText.LoadString(resId);
		GetDlgItem(IDC_SERVICE_STATUS)->SetWindowText(statusText);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
