// autonguiDlg.h : ヘッダー ファイル
//

#pragma once
#include "AppConfig.h"

// CMainDlg ダイアログ
class CMainDlg : public CDialogEx
{
// コンストラクション
public:
	CMainDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
	enum { IDD = IDD_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート

// 実装
protected:
	HICON m_hIcon;
	CAppConfig appConfig;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnOK();
	afx_msg void OnBnClickedAppend();
	afx_msg void OnBnClickedModify();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnItemChangedProcessConfigList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnGetDispInfoProcessConfigList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickCopyrightUrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkProcessConfigList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickedShowServiceProp();
};
