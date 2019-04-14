#pragma once
#include <vector>
#include <string>

template<typename T>
std::size_t find_index(const std::vector<T>& v, const T& e)
{
	auto iter = std::find(v.begin(), v.end(), e);
	if (v.end() == iter)
	{
		return -1;
	}
	else
	{
		return std::distance(v.begin(), iter);
	}
}

template<typename T>
struct case_insensitive_less
{
	virtual bool operator()(const T& _Left, const T& _Right) const = 0;
};

template<>
struct case_insensitive_less<std::wstring>
{
	bool operator()(const std::wstring& _Left, const std::wstring& _Right) const
	{
		return _wcsicmp(_Left.c_str(), _Right.c_str()) < 0;
	}
};
