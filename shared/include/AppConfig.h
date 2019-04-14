#pragma once
#include "ServiceConfig.h"
#include "ProcessConfig.h"
#include "stlext.h"

class CAppConfig
{
public:
	CAppConfig() { }
	virtual ~CAppConfig() { }

	CServiceConfig Service;
	std::map<std::wstring, CProcessConfig, case_insensitive_less<std::wstring>> Process;
};
