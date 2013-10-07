#include "stdafx.h"
#include "ProcessConfig.h"
#include "PriorityMap.h"

CProcessConfig::CProcessConfig(void)
{
}

CProcessConfig::~CProcessConfig(void)
{
}

// レジストリへセーブ
BOOL CProcessConfig::SaveToRegistry(HKEY key)
{
	BOOL retval = FALSE;
	HKEY childKey = NULL;
	LSTATUS rv;
	LPCTSTR exeFileName = PathFindFileName(ExeFilePath);
	rv = RegCreateKeyEx(key, exeFileName, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &childKey, NULL);
	if (ERROR_SUCCESS != rv)
	{
		goto FINALLY;
	}
	rv = RegSetValueEx(childKey, _T("ExecuteFilePath"), 0, REG_SZ, (BYTE*)exeFileName, (lstrlen(exeFileName) + 1) * sizeof(TCHAR));
	if (ERROR_SUCCESS != rv)
	{
		goto FINALLY;
	}
	rv = RegSetValueEx(childKey, _T("Enable"), 0, REG_DWORD, (BYTE*)&Enable, sizeof(DWORD));
	if (ERROR_SUCCESS != rv)
	{
		goto FINALLY;
	}
	rv = RegSetValueEx(childKey, _T("PriorityClass"), 0, REG_DWORD, (BYTE*)&PriorityClass, sizeof(DWORD));
	if (ERROR_SUCCESS != rv)
	{
		goto FINALLY;
	}
FINALLY:
	if (childKey != NULL)
	{
		RegCloseKey(childKey);
	}
	return retval;
}

// レジストリからロード
BOOL CProcessConfig::LoadFromRegistry(HKEY key, LPCTSTR keyName)
{
	BOOL retval = FALSE;
	HKEY childKey = NULL;
	LSTATUS rv;
	DWORD type = REG_DWORD;
	DWORD cbData = sizeof(DWORD);

	rv = RegOpenKeyEx(key, keyName, 0, KEY_READ, &childKey);
	if (ERROR_SUCCESS != rv)
	{
		goto FINALLY;
	}
	type = REG_SZ;
	cbData = MAX_PATH * sizeof(TCHAR);
	rv = RegQueryValueEx(childKey, _T("ExecuteFilePath"), NULL, &type, (BYTE*)ExeFilePath.GetBuffer(cbData / sizeof(TCHAR)), &cbData);
	ExeFilePath.ReleaseBuffer();
	if (ERROR_SUCCESS != rv || REG_SZ != type)
	{
		goto FINALLY;
	}
	type = REG_DWORD;
	cbData = sizeof(DWORD);
	rv = RegQueryValueEx(childKey, _T("Enable"), NULL, &type, (BYTE*)&Enable, &cbData);
	if(ERROR_SUCCESS != rv || REG_DWORD != type)
	{
		goto FINALLY;
	}
	type = REG_DWORD;
	cbData = sizeof(DWORD);
	rv = RegQueryValueEx(childKey, _T("PriorityClass"), NULL, &type, (BYTE*)&PriorityClass, &cbData);
	if (ERROR_SUCCESS != rv || REG_DWORD != type)
	{
		goto FINALLY;
	}
	retval = TRUE;
FINALLY:
	if (childKey != NULL)
	{
		RegCloseKey(childKey);
	}
	return retval;
}
