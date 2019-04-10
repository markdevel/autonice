// autonguiDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once
#include "AppConfig.h"

// CMainDlg �_�C�A���O
class CMainDlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	CMainDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[

// �_�C�A���O �f�[�^
	enum { IDD = IDD_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g

// ����
protected:
	HICON m_hIcon;
	CAppConfig appConfig;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
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
