#pragma once

class CEventSource
{
public:
	explicit CEventSource(LPCGUID providerId) : regHandle(NULL)
	{
		Register(providerId);
	}

	virtual ~CEventSource()
	{
		Unregister();
	}

	void WriteString(UCHAR level, ULONGLONG keyword, PCWSTR string) const
	{
		EventWriteString(regHandle, level, keyword, string);
	}

	void FormatWrite(UCHAR level, PCWSTR format, ...) const
	{
		std::vector<wchar_t> string(4096, 0);
		va_list va;
		va_start(va, format);
		_vsnwprintf_s(&string[0], string.size(), _TRUNCATE, format, va);
		va_end(va);
		WriteString(level, 0, &string[0]);
	}

protected:
	void Register(LPCGUID providerId)
	{
		EventRegister(providerId, NULL, NULL, &regHandle);
	}

	void Unregister()
	{
		EventUnregister(regHandle);
		regHandle = NULL;
	}

protected:
	REGHANDLE regHandle;
};
