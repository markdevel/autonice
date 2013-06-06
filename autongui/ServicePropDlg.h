#pragma once
#include "ServiceConfig.h"

// CServicePropDlg ダイアログ

class CServicePropDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CServicePropDlg)

public:
	CServicePropDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CServicePropDlg();

// ダイアログ データ
	enum { IDD = IDD_SERVICE_PROP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CServiceConfig* model;
};
