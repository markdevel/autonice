#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>

SERVICE_STATUS_HANDLE serviceStatusHandle = NULL;
SERVICE_STATUS serviceStatus;
HANDLE terminateEventHandle = NULL;
#define APP_REG_KEY_ROOT _T("Software\\markdevel\\autonice")
#define APP_REG_KEY_PROCESS_CONFIG _T("ProcessConfig")

void doRenice()
{
	HKEY keyHandle;
	HANDLE processSnapshot;
	PROCESSENTRY32 processEntry;
	LSTATUS rv;

	rv = RegCreateKeyEx(HKEY_LOCAL_MACHINE, APP_REG_KEY_ROOT _T("\\") APP_REG_KEY_PROCESS_CONFIG, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &keyHandle, NULL);
	if(ERROR_SUCCESS != rv)
	{
		// エラー
		return;
	}
	processSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(INVALID_HANDLE_VALUE == processSnapshot)
	{
		// エラー
		return;
	}
	processEntry.dwSize = sizeof(processEntry);
	Process32First(processSnapshot, &processEntry);
	do
	{
		HKEY childKey;
		HANDLE processHandle;
		DWORD type = REG_DWORD;
		DWORD enable = 1;
		DWORD size = sizeof(enable);
		DWORD priorityClass = NORMAL_PRIORITY_CLASS;

		// 指定のモジュールがレジストリに存在するかどうかを調べる
		rv = RegOpenKeyEx(keyHandle, processEntry.szExeFile, 0, KEY_READ, &childKey);
		if(ERROR_SUCCESS != rv)
		{
			continue;
		}
		rv = RegQueryValueEx(childKey, _T("Enable"), NULL, &type, (LPBYTE)&enable, &size);
		if(ERROR_SUCCESS != rv)
		{
			continue;
		}
		if(enable == 0){
			continue;
		}
		rv = RegQueryValueEx(childKey, _T("PriorityClass"), NULL, &type, (LPBYTE)&priorityClass, &size);
		if(ERROR_SUCCESS != rv)
		{
			continue;
		}
		RegCloseKey(childKey);

		// 優先度を変更する
		processHandle = OpenProcess(PROCESS_SET_INFORMATION, FALSE, processEntry.th32ProcessID);
		if(NULL == processHandle)
		{
			continue;
		}
		SetPriorityClass(processHandle, priorityClass);
		CloseHandle(processHandle);
	}
	while(Process32Next(processSnapshot, &processEntry));
	RegCloseKey(keyHandle);
}

DWORD WINAPI handlerEx(
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
	HKEY keyHandle;
	LSTATUS rv;
	DWORD type;
	DWORD interval;
	DWORD dataSize;

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

	// レジストリから設定値を読み込む
	rv = RegCreateKeyEx(HKEY_LOCAL_MACHINE, APP_REG_KEY_ROOT, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &keyHandle, NULL);
	if(ERROR_SUCCESS != rv){
		return;
	}
	type = REG_DWORD;
	interval = 0;
	dataSize = sizeof(interval);
	RegQueryValueEx(keyHandle, _T("ProcessCheckInterval"), NULL, &type, (LPBYTE)&interval, &dataSize);
	if(interval < 100 || interval > 60 * 60 * 1000){
		interval = 5000;
	}
	RegCloseKey(keyHandle);

	// サービスメインルーチン
	serviceStatus.dwCurrentState = SERVICE_RUNNING;
//	serviceStatus.dwCheckPoint = 0;
	serviceStatus.dwWaitHint = 0;
	SetServiceStatus(serviceStatusHandle, &serviceStatus);
	do
	{
		doRenice();
	}
	while(WaitForSingleObject(terminateEventHandle, interval) == WAIT_TIMEOUT);

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
