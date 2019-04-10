#pragma once
#include "BaseService.h"
#include "ScopedHandle.h"
#include "AppConfig.h"
#include "RegistrySerializer.h"

#define APP_REG_KEY_ROOT L"Software\\markdevel\\autonice"

class CAutoNiceService : public CBaseService
{
public:
	CAutoNiceService() = delete;

	CAutoNiceService(LPCWSTR serviceName, LPCGUID serviceProviderGuid)
		: CBaseService(serviceName, serviceProviderGuid) { }

	virtual ~CAutoNiceService() { }

	virtual void Run() override
	{
		GetEventSource().WriteString(TRACE_LEVEL_INFORMATION, 0, L"Running.");
		SCOPED_HANDLE keyChangeEventHandle(CreateEventW(NULL, FALSE, FALSE, NULL), &::CloseHandle);
		CRegistryKey regKey;
		regKey.Create(HKEY_LOCAL_MACHINE, APP_REG_KEY_ROOT);
		HANDLE handles[2]{
			GetTerminateEventHandle(),
			keyChangeEventHandle.get()
		};
		CRegistrySerializer registrySerializer;
		CAppConfig appConfig = registrySerializer.Load(regKey);
		for(;;)
		{
			SCOPED_HANDLE processSnapshot(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0), &::CloseHandle);
			if (INVALID_HANDLE_VALUE == processSnapshot.get())
			{
				break;
			}
			PROCESSENTRY32W processEntry;
			processEntry.dwSize = sizeof(processEntry);
			Process32FirstW(processSnapshot.get(), &processEntry);
			do
			{
				// 指定のモジュールがレジストリに存在するかどうかを調べる
				auto iter = appConfig.Process.find(processEntry.szExeFile);
				if (iter != appConfig.Process.end())
				{
					// 優先度を変更する
					if (iter->second.Enable != 0)
					{
						SCOPED_HANDLE processHandle(OpenProcess(PROCESS_SET_INFORMATION | PROCESS_QUERY_LIMITED_INFORMATION, FALSE, processEntry.th32ProcessID), &::CloseHandle);
						if (processHandle)
						{
							SetPriorityClass(processHandle.get(), iter->second.PriorityClass);
							GetEventSource().FormatWrite(TRACE_LEVEL_VERBOSE, L"Change priority : %s", processEntry.szExeFile);
						}
					}
				}
			}
			while (Process32Next(processSnapshot.get(), &processEntry));
			regKey.RegisterNotifyChange(keyChangeEventHandle.get());
			DWORD rv = WaitForMultipleObjects(ARRAYSIZE(handles), handles, FALSE, appConfig.Service.ProcessCheckInterval);
			if (WAIT_OBJECT_0 + 0 == rv)
			{
				break;
			}
			else if (WAIT_OBJECT_0 + 1 == rv)
			{
				appConfig = registrySerializer.Load(regKey);
			}
		}
		GetEventSource().WriteString(TRACE_LEVEL_INFORMATION, 0, L"Terminated.");
	}
};
