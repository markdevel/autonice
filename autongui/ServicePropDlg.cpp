// ServicePropDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "autongui.h"
#include "ServicePropDlg.h"
#include "afxdialogex.h"


// CServicePropDlg �_�C�A���O

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

	DDX_Text(pDX, IDC_SERVICE_PROCESS_CHECK_INTERVAL, model->ProcessCheckInterval);
	DDV_MinMaxInt(pDX, model->ProcessCheckInterval, 100, 60 * 60 * 1000);
}


BEGIN_MESSAGE_MAP(CServicePropDlg, CDialogEx)
END_MESSAGE_MAP()


// CServicePropDlg ���b�Z�[�W �n���h���[


BOOL CServicePropDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CSpinButtonCtrl* ctlSpin = reinterpret_cast<CSpinButtonCtrl*>(GetDlgItem(IDC_SERVICE_PROCESS_CHECK_INTERVAL_SPIN));
	ctlSpin->SetRange32(100, 60 * 60 * 1000);
	ctlSpin->SetPos32(model->ProcessCheckInterval);

	// �T�[�r�X�ғ���Ԃ̕\��
	SC_HANDLE managerHandle = OpenSCManager(NULL, NULL, GENERIC_READ);
	SC_HANDLE serviceHandle = OpenService(managerHandle, _T("autonice"), GENERIC_READ);
	SERVICE_STATUS status;
	if(QueryServiceStatus(serviceHandle, &status)){
		LPTSTR statusText;
		switch(status.dwCurrentState){
		case SERVICE_STOPPED:
			statusText = _T("��~");
			break;
		case SERVICE_START_PENDING:
			statusText = _T("�J�n������");
			break;
		case SERVICE_STOP_PENDING:
			statusText = _T("��~������");
			break;
		case SERVICE_RUNNING:
			statusText = _T("�ғ���");
			break;
		case SERVICE_CONTINUE_PENDING:
			statusText = _T("�ĊJ������");
			break;
		case SERVICE_PAUSE_PENDING:
			statusText = _T("�ꎞ��~������");
			break;
		case SERVICE_PAUSED:
			statusText = _T("�ꎞ��~");
			break;
		default:
			statusText = _T("�s��");
			break;
		}
//		GetDlgItem(IDC_SERVICE_STATUS)->SetWindowText(statusText);
	}
	CloseServiceHandle(serviceHandle);
	CloseServiceHandle(managerHandle);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}
