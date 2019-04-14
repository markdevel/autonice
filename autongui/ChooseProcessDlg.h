#pragma once
#include "resource.h"

class CChooseProcessDlg : public CDialog
{
	DECLARE_DYNAMIC(CChooseProcessDlg)

public:
	CChooseProcessDlg(CWnd* pParent = NULL);
	virtual ~CChooseProcessDlg();

	enum { IDD = IDD_CHOOSE_PROCESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemActivate(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog() override;
	virtual void OnOK() override;
	CString Model;
};
