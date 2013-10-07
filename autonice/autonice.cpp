#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>

SERVICE_STATUS_HANDLE serviceStatusHandle = NULL;
SERVICE_STATUS serviceStatus;
HANDLE terminateEventHandle = NULL;
#define APP_REG_KEY_ROOT _T("Software\\markdevel\\autonice")
#define APP_REG_KEY_PROCESS_CONFIG _T("ProcessConfig")

void
doRenice (
	)
{
	HKEY keyHandle = NULL;
	HANDLE keyChangeEventHandle = NULL;
	HANDLE processSnapshot = NULL;
	PROCESSENTRY32 processEntry;
	HANDLE handles[2];
	LSTATUS rv;

	rv = RegCreateKeyEx(HKEY_LOCAL_MACHINE, APP_REG_KEY_ROOT _T("\\") APP_REG_KEY_PROCESS_CONFIG, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &keyHandle, NULL);
	if (ERROR_SUCCESS != rv)
	{
		goto FINALLY;
	}
	keyChangeEventHandle = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (NULL == keyChangeEventHandle)
	{
		goto FINALLY;
	}
	handles[0] = keyChangeEventHandle;
	handles[1] = terminateEventHandle;
	do
	{
		processSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (INVALID_HANDLE_VALUE == processSnapshot)
		{
			break;
		}
		processEntry.dwSize = sizeof(processEntry);
		Process32First(processSnapshot, &processEntry);
		do
		{
			HKEY childKey;
			HANDLE processHandle;
			DWORD type;
			DWORD length;
			DWORD enable = 1;
			DWORD priorityClass = NORMAL_PRIORITY_CLASS;
			TCHAR executeFilePath[MAX_PATH];

			// 指定のモジュールがレジストリに存在するかどうかを調べる
			rv = RegOpenKeyEx(keyHandle, processEntry.szExeFile, 0, KEY_READ, &childKey);
			if (ERROR_SUCCESS != rv)
			{
				continue;
			}
			type = REG_SZ;
			length = MAX_PATH * sizeof(TCHAR);
			rv = RegQueryValueEx(childKey, _T("ExecuteFilePath"), NULL, &type, (BYTE*)executeFilePath, &length);
			executeFilePath[length / sizeof(TCHAR)] = UNICODE_NULL;
			type = REG_DWORD;
			length = sizeof(enable);
			RegQueryValueEx(childKey, _T("Enable"), NULL, &type, (LPBYTE)&enable, &length);
			type = REG_DWORD;
			length = sizeof(priorityClass);
			RegQueryValueEx(childKey, _T("PriorityClass"), NULL, &type, (LPBYTE)&priorityClass, &length);
			RegCloseKey(childKey);

			// 優先度を変更する
			if (enable == 0)
			{
				continue;
			}
			processHandle = OpenProcess(PROCESS_SET_INFORMATION | PROCESS_QUERY_LIMITED_INFORMATION, FALSE, processEntry.th32ProcessID);
			if (NULL == processHandle)
			{
				continue;
			}
			SetPriorityClass(processHandle, priorityClass);
			CloseHandle(processHandle);
		} while (Process32Next(processSnapshot, &processEntry));
		CloseHandle(processSnapshot);
		rv = RegNotifyChangeKeyValue(keyHandle, TRUE, REG_NOTIFY_CHANGE_NAME | REG_NOTIFY_CHANGE_ATTRIBUTES | REG_NOTIFY_CHANGE_LAST_SET, keyChangeEventHandle, TRUE);
		if (ERROR_SUCCESS != rv)
		{
			goto FINALLY;
		}
	} while (WaitForMultipleObjects(ARRAYSIZE(handles), handles, FALSE, 5000) != WAIT_OBJECT_0 + 1);
FINALLY:
	if (NULL != keyHandle)
	{
		RegCloseKey(keyHandle);
	}
}

DWORD
WINAPI
handlerEx(
	_In_ DWORD dwControl,
	_In_ DWORD dwEventType,
	_In_ LPVOID lpEventData,
	_In_ LPVOID lpContext
	)
{
	switch(dwControl)
	{
	case SERVICE_CONTROL_STOP:
		serviceStatus.dwCurrentState = SERVICE_STOP_PENDING;
//		serviceStatus.dwCheckPoint = 0;
		serviceStatus.dwWaitHint = 10000;
		SetServiceStatus(serviceStatusHandle, &serviceStatus);
		SetEvent(terminateEventHandle);
		break;

	case SERVICE_CONTROL_INTERROGATE:
		SetServiceStatus(serviceStatusHandle, &serviceStatus);
		break;
	}
	return NO_ERROR;
}

void
WINAPI
ServiceMain(
	_In_ DWORD dwArgc,
	_In_ LPTSTR *lpszArgv
	)
{
	terminateEventHandle = CreateEvent(NULL, FALSE, FALSE, NULL);
	if(NULL == terminateEventHandle){
		return;
	}

	// 開始処理
	serviceStatusHandle = RegisterServiceCtrlHandlerEx(_T("autonice"), handlerEx, NULL);
	if(NULL == serviceStatusHandle){
		return;
	}
	ZeroMemory(&serviceStatus, sizeof(serviceStatus));
	serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	serviceStatus.dwCurrentState = SERVICE_START_PENDING;
	SetServiceStatus(serviceStatusHandle, &serviceStatus);

	// サービスメインルーチン
	serviceStatus.dwCurrentState = SERVICE_RUNNING;
//	serviceStatus.dwCheckPoint = 0;
	serviceStatus.dwWaitHint = 0;
	SetServiceStatus(serviceStatusHandle, &serviceStatus);
	
	doRenice();

	// 終了処理
	serviceStatus.dwCurrentState = SERVICE_STOPPED;
//	serviceStatus.dwCheckPoint = 0;
	serviceStatus.dwWaitHint = 0;
	SetServiceStatus(serviceStatusHandle, &serviceStatus);
	CloseHandle(terminateEventHandle);
}

SERVICE_TABLE_ENTRY ServiceTable[] =
{
    { _T("autonice"), ServiceMain },
    { NULL, NULL }
};

int
WINAPI
_tWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPTSTR lpCmdLine,
    _In_ int nShowCmd
    )
{
	return !StartServiceCtrlDispatcher(ServiceTable);
}
