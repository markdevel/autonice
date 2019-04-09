#include "stdafx.h"
#include "ServiceConfig.h"

CServiceConfig::CServiceConfig(void)
	: ProcessCheckInterval(5000)
{
}

CServiceConfig::~CServiceConfig(void)
{
}

// ���W�X�g���փZ�[�u
BOOL CServiceConfig::SaveToRegistry(HKEY key)
{
	LSTATUS rv;
	rv = RegSetValueEx(key, _T("ProcessCheckInterval"), 0, REG_DWORD, (BYTE*)&ProcessCheckInterval, sizeof(ProcessCheckInterval));
	if(ERROR_SUCCESS != rv){
		return FALSE;
	}
	return TRUE;
}

// ���W�X�g�����烍�[�h
BOOL CServiceConfig::LoadFromRegistry(HKEY key)
{
	LSTATUS rv;
	DWORD type = REG_DWORD;
	DWORD cbData = sizeof(DWORD);
	rv = RegQueryValueEx(key, _T("ProcessCheckInterval"), NULL, &type, (BYTE*)&ProcessCheckInterval, &cbData);
	if(ERROR_SUCCESS != rv || REG_DWORD != type){
		return FALSE;
	}
	return TRUE;
}
