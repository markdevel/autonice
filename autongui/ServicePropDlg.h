#pragma once
#include "ServiceConfig.h"

// CServicePropDlg �_�C�A���O

class CServicePropDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CServicePropDlg)

public:
	CServicePropDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CServicePropDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_SERVICE_PROP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CServiceConfig* model;
};
