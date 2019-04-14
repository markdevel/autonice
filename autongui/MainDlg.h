// autonguiDlg.h : ヘッダー ファイル
//

#pragma once
#include "AppConfig.h"

class CMainDlg : public CDialogEx
{
public:
	CMainDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;

	HICON m_hIcon;
	CAppConfig appConfig;

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog() override;
	virtual void OnOK() override;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedAppend();
	afx_msg void OnBnClickedModify();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnItemChanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnGetDispInfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemActivate(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickCopyrightUrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickedShowServiceProp();
};
