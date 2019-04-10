#pragma once
#include <vector>

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
