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
		std::vector<wchar_t> buf(1024);
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, 0, error, 0, buf.data(), (DWORD)buf.size() - 1, 0);
		return std::wstring(buf.data());
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
