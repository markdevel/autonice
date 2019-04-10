#pragma once
#include "ServiceConfig.h"
#include "ProcessConfig.h"

class CAppConfig
{
public:
	CAppConfig() { }
	virtual ~CAppConfig() { }

	CServiceConfig Service;
	std::map<std::wstring, CProcessConfig> Process;
};
