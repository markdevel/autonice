#pragma once
#include "ServiceConfig.h"

// CServicePropDlg ダイアログ

class CServicePropDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CServicePropDlg)

public:
	CServicePropDlg(CWnd* pParent = NULL);
	virtual ~CServicePropDlg();
	virtual BOOL OnInitDialog();
	CServiceConfig Model;
	enum { IDD = IDD_SERVICE_PROP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()

private:
	static const std::map<DWORD, UINT> statusTextMap;
};
