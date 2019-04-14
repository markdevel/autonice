#pragma once
#include "Win32Exception.h"

class CProcessEntryIterator
{
public:
	using difference_type   = ptrdiff_t;
	using value_type        = PROCESSENTRY32W;
	using pointer           = PROCESSENTRY32W *;
	using reference         = PROCESSENTRY32W &;
	using iterator_category = std::forward_iterator_tag;

	CProcessEntryIterator(HANDLE snapshotHandle = NULL) noexcept
		: snapshotHandle(snapshotHandle), processEntry({ 0 }), hasNext(FALSE)
	{
		if (snapshotHandle != NULL)
		{
			processEntry.dwSize = sizeof(processEntry);
			hasNext = Process32FirstW(snapshotHandle, &processEntry);
		}
	}

	virtual ~CProcessEntryIterator(void) { }

	const reference operator*()
	{
		return processEntry;
	}

	const pointer operator->()
	{
		return &processEntry;
	}

	bool operator!=(const CProcessEntryIterator& v) const
	{
		if (hasNext == v.hasNext)
		{
			return false;
		}
		else
		{
			return memcmp(&processEntry, &v.processEntry, sizeof(processEntry)) != 0;
		}
	}

	CProcessEntryIterator& operator++()
	{
		hasNext = Process32NextW(snapshotHandle, &processEntry);
		return *this;
	}

private:
	const HANDLE snapshotHandle;
	PROCESSENTRY32W processEntry;
	BOOL hasNext;
};

class CToolhelp32Snapshot
{
public:
	CToolhelp32Snapshot(void)
	{
		snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		ThrowLastErrorIf(snapshotHandle == INVALID_HANDLE_VALUE);
	}

	virtual ~CToolhelp32Snapshot(void)
	{
		CloseHandle(snapshotHandle);
	}

	CProcessEntryIterator begin() const
	{
		return CProcessEntryIterator(snapshotHandle);
	}

	CProcessEntryIterator end() const
	{
		return CProcessEntryIterator();
	}

protected:
	HANDLE snapshotHandle;
};
