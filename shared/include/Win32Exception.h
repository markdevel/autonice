#pragma once

class Win32Exception : public std::exception
{
public:
	Win32Exception(DWORD error, const std::wstring& msg = std::wstring())
		: std::exception(), error(error), msg(msg) { }

	virtual ~Win32Exception() { }

	DWORD GetErrorCode() const
	{
		return error;
	}

	std::wstring GetMessage() const
	{
		return msg;
	}

	std::wstring GetErrorFormatMessage() const
	{
		LPWSTR buf;
		::FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&buf, 0, NULL);
		auto rv = std::wstring(buf);
		::LocalFree(buf);
		return rv;
	}

protected:
	DWORD error;
	std::wstring msg;
};

inline void ThrowLastErrorIf(bool expression, const std::wstring& msg = std::wstring())
{
	if (expression)
	{
		throw Win32Exception(GetLastError(), msg);
	}
}

inline void ThrowLRESULTIf(LRESULT rv, const std::wstring& msg = std::wstring())
{
	if (IS_ERROR(rv))
	{
		throw Win32Exception((DWORD)rv, msg);
	}
}

inline void ThrowHRESULTIf(HRESULT rv, const std::wstring& msg = std::wstring())
{
	if (FAILED(rv))
	{
		throw Win32Exception((DWORD)rv, msg);
	}
}
