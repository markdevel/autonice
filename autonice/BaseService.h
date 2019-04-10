#pragma once
#include "EventSource.h"
#include "ScopedHandle.h"

/* abstract */ class CBaseService
{
public:
	CBaseService() = delete;

	CBaseService(LPCWSTR serviceName, LPCGUID serviceProviderGuid)
		: serviceName(serviceName)
		, terminateEventHandle(CreateEventW(NULL, FALSE, FALSE, NULL), &::CloseHandle)
		, eventSource(serviceProviderGuid)
		{ }

	virtual ~CBaseService() { }

	HANDLE GetTerminateEventHandle() const
	{
		return terminateEventHandle.get();
	}

	const CEventSource& GetEventSource() const
	{
		return eventSource;
	}

	virtual void Run() = 0;

	void ServiceMain()
	{
		// サービスハンドラーの登録
		serviceStatusHandle = RegisterServiceCtrlHandlerExW(serviceName, HandlerExStub, this);
		if (NULL == serviceStatusHandle) {
			return;
		}

		// 開始処理
		SERVICE_STATUS serviceStatus = {
			SERVICE_WIN32_OWN_PROCESS,
			SERVICE_START_PENDING
		};
		SetServiceStatus(serviceStatusHandle, &serviceStatus);

		// サービスメインルーチン
		serviceStatus.dwCurrentState = SERVICE_RUNNING;
		serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PARAMCHANGE;
		SetServiceStatus(serviceStatusHandle, &serviceStatus);

		Run();

		// 終了処理
		serviceStatus.dwCurrentState = SERVICE_STOPPED;
		serviceStatus.dwControlsAccepted = 0;
		serviceStatus.dwCheckPoint = 3;
		SetServiceStatus(serviceStatusHandle, &serviceStatus);
	}

	static DWORD WINAPI HandlerExStub(_In_ DWORD dwControl, _In_ DWORD dwEventType, _In_ LPVOID lpEventData, _In_ LPVOID lpContext)
	{
		auto me = reinterpret_cast<CBaseService*>(lpContext);
		return me->HandlerEx(dwControl, dwEventType, lpEventData);
	}

	virtual DWORD OnParamChange()
	{
		return NO_ERROR;
	}

	DWORD HandlerEx(_In_ DWORD dwControl, _In_ DWORD dwEventType, _In_ LPVOID lpEventData)
	{
		switch (dwControl)
		{
		case SERVICE_CONTROL_PARAMCHANGE:
			return OnParamChange();

		case SERVICE_CONTROL_SHUTDOWN:
		case SERVICE_CONTROL_STOP:
			{
				SERVICE_STATUS serviceStatus = {
					SERVICE_WIN32_OWN_PROCESS,
					SERVICE_STOP_PENDING,
					0,
					0,
					0,
					4
				};
				SetServiceStatus(serviceStatusHandle, &serviceStatus);
				SetEvent(GetTerminateEventHandle());
			}
			break;

		case SERVICE_CONTROL_INTERROGATE:
			break;
		}
		return NO_ERROR;
	}


protected:
	LPCWSTR serviceName;
	SCOPED_HANDLE terminateEventHandle;
	CEventSource eventSource;
	SERVICE_STATUS_HANDLE serviceStatusHandle;
};
