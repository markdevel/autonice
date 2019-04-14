#pragma once
#include "ProcessConfig.h"

// CProcessPropDlg �_�C�A���O

class CProcessPropDlg : public CDialog
{
	DECLARE_DYNAMIC(CProcessPropDlg)

public:
	CProcessPropDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CProcessPropDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_PROCESS_PROP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;

	DECLARE_MESSAGE_MAP()

	int Enable;
	CString ExeFilePath;
	int PriorityIndex;

public:
	afx_msg void OnBnClickedFilesystemBrowse();
	afx_msg void OnBnClickedProcessPropBrowse();
	afx_msg void OnUpdateFilename();
	virtual BOOL OnInitDialog() override;
	virtual void OnOK() override;
	CProcessConfig Model;
};
