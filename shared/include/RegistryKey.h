#pragma once
#include "ScopedHandle.h"
#include "Win32Exception.h"

class CRegistryKey
{
public:
	CRegistryKey(void) : hKey(NULL) { }

	virtual ~CRegistryKey(void)
	{
		Dispose();
	}

	void Dispose()
	{
		if (NULL != hKey)
		{
			RegCloseKey(hKey);
			hKey = NULL;
		}
	}

	operator HKEY () const
	{
		return hKey;
	}

	DWORD Create(HKEY parentKey, LPCWSTR subKey, DWORD options = 0, REGSAM samDesired = KEY_ALL_ACCESS)
	{
		DWORD disposition = 0;
		ThrowLRESULTIf(RegCreateKeyExW(parentKey, subKey, 0, NULL, options, samDesired, NULL, &hKey, &disposition));
		return disposition;
	}

	void Open(HKEY parentKey, LPCWSTR subKey, DWORD options = 0, REGSAM samDesired = KEY_READ)
	{
		ThrowLRESULTIf(RegOpenKeyExW(parentKey, subKey, options, samDesired, &hKey));
	}

	bool GetChildren(DWORD index, std::wstring& subKey) const
	{
		WCHAR name[256]; // maximum Key name size.
		DWORD nameLength = ARRAYSIZE(name);
		LRESULT rv = RegEnumKeyExW(hKey, index, name, &nameLength, NULL, NULL, NULL, NULL);
		ThrowLRESULTIf(rv);
		subKey.assign(name);
		return rv != ERROR_NO_MORE_ITEMS;
	}

	DWORD QueryDWORDValue(LPCWSTR valueName, DWORD defaultValue) const
	{
		DWORD type = REG_DWORD;
		DWORD value = defaultValue;
		DWORD valueLength = sizeof(value);
		ThrowLRESULTIf(RegQueryValueExW(hKey, valueName, NULL, &type, reinterpret_cast<LPBYTE>(&value), &valueLength));
		return value;
	}

	std::wstring QueryStringValue(LPCWSTR valueName, LPCWSTR defaultValue) const
	{
		DWORD type = REG_SZ;
		DWORD valueLength = 1024;
		std::vector<wchar_t> value(valueLength);
		ThrowLRESULTIf(RegQueryValueExW(hKey, valueName, NULL, &type, reinterpret_cast<LPBYTE>(value.data()), &valueLength));
		return std::wstring(value.data());
	}

	void SetDWORDValue(LPCWSTR valueName, DWORD value) const
	{
		ThrowLRESULTIf(RegSetValueExW(hKey, valueName, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&value), sizeof(DWORD)));
	}

	void SetStringValue(LPCWSTR valueName, const std::wstring& value) const
	{
		ThrowLRESULTIf(RegSetValueExW(hKey, valueName, 0, REG_SZ, reinterpret_cast<const BYTE*>(value.c_str()), (DWORD)(value.length() + 1) * sizeof(wchar_t)));
	}

	void DeleteTree(LPCWSTR subKeyName) const
	{
		ThrowLRESULTIf(RegDeleteTreeW(hKey, subKeyName));
	}

	void RegisterNotifyChange(HANDLE eventHandle, bool watchSubtree = true, DWORD notifyFilter = 0x0f) const
	{
		ThrowLRESULTIf(RegNotifyChangeKeyValue(hKey, watchSubtree ? TRUE : FALSE, notifyFilter, eventHandle, TRUE));
	}

protected:
	HKEY hKey;
};
