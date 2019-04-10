#include "stdafx.h"
#include "AutoNiceService.h"

static const wchar_t* SERVICE_NAME = L"autonice";

// {66A2AC69-ACC3-4678-BE2A-8F3CFD5D7D58}
static const GUID SERVICE_PROVIDER_GUID = { 0x66a2ac69, 0xacc3, 0x4678, { 0xbe, 0x2a, 0x8f, 0x3c, 0xfd, 0x5d, 0x7d, 0x58 } };

void
WINAPI
ServiceMain(
	_In_ DWORD dwArgc,
	_In_ LPWSTR *lpszArgv
	)
{
	auto service = new CAutoNiceService(SERVICE_NAME, &SERVICE_PROVIDER_GUID);
	service->ServiceMain();
}

int
WINAPI
wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nShowCmd
    )
{
	static const SERVICE_TABLE_ENTRYW ServiceTable[] =
	{
		{ const_cast<LPWSTR>(SERVICE_NAME), ServiceMain },
		{ NULL, NULL }
	};
	if (StartServiceCtrlDispatcherW(ServiceTable))
	{
		return EXIT_SUCCESS;
	}
	else
	{
		return GetLastError();
	}
}
