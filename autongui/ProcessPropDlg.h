#pragma once
#include "ProcessConfig.h"

// CProcessPropDlg ダイアログ

class CProcessPropDlg : public CDialog
{
	DECLARE_DYNAMIC(CProcessPropDlg)

public:
	CProcessPropDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CProcessPropDlg();

// ダイアログ データ
	enum { IDD = IDD_PROCESS_PROP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

	int Enable;
	CString ExeFilePath;
	int PriorityIndex;

public:
	afx_msg void OnBnClickedProcessPropBrowse();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	CProcessConfig Model;
};
