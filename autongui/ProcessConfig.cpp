#include "stdafx.h"
#include "ProcessConfig.h"
#include "PriorityMap.h"

CProcessConfig::CProcessConfig(void)
	: PriorityClass(NORMAL_PRIORITY_CLASS)
{
}

CProcessConfig::~CProcessConfig(void)
{
}

// レジストリへセーブ
BOOL CProcessConfig::SaveToRegistry(HKEY key)
{
	LSTATUS rv;
	HKEY childKey;
	rv = RegCreateKeyEx(key, ExeFileName, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS | KEY_WOW64_64KEY, NULL, &childKey, NULL);
	if(ERROR_SUCCESS != rv){
		return FALSE;
	}
	rv = RegSetValueEx(childKey, _T("Enable"), 0, REG_DWORD, (BYTE*)&Enable, sizeof(Enable));
	if(ERROR_SUCCESS != rv){
		RegCloseKey(childKey);
		return FALSE;
	}
	rv = RegSetValueEx(childKey, _T("PriorityClass"), 0, REG_DWORD, (BYTE*)&PriorityClass, sizeof(PriorityClass));
	if(ERROR_SUCCESS != rv){
		RegCloseKey(childKey);
		return FALSE;
	}
	RegCloseKey(childKey);
	return TRUE;
}

// レジストリからロード
BOOL CProcessConfig::LoadFromRegistry(HKEY key, LPCTSTR keyName)
{
	LSTATUS rv;
	HKEY childKey;
	DWORD type = REG_DWORD;
	DWORD cbData = sizeof(DWORD);
	rv = RegOpenKeyEx(key, keyName, 0, KEY_READ | KEY_WOW64_64KEY, &childKey);
	if(ERROR_SUCCESS != rv){
		return FALSE;
	}
	rv = RegQueryValueEx(childKey, _T("Enable"), NULL, &type, (BYTE*)&Enable, &cbData);
	if(ERROR_SUCCESS != rv || REG_DWORD != type){
		RegCloseKey(childKey);
		return FALSE;
	}
	rv = RegQueryValueEx(childKey, _T("PriorityClass"), NULL, &type, (BYTE*)&PriorityClass, &cbData);
	if(ERROR_SUCCESS != rv || REG_DWORD != type){
		RegCloseKey(childKey);
		return FALSE;
	}
	RegCloseKey(childKey);
	ExeFileName = keyName;
	return TRUE;
}
