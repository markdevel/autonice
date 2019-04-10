#pragma once
#include "AppConfig.h"
#include "RegistryKey.h"

class CRegistrySerializer
{
public:
	CAppConfig Load(HKEY key, LPCTSTR keyName) const
	{
		CRegistryKey regKey;
		regKey.Open(key, keyName);
		return Load(std::move(regKey));
	}
		
	CAppConfig Load(const CRegistryKey& regKey) const
	{
		CAppConfig appConfig;
		appConfig.Service.ProcessCheckInterval = regKey.QueryDWORDValue(L"ProcessCheckInterval", 5000);
		CRegistryKey regProcessConfigRootKey;
		regProcessConfigRootKey.Open(regKey, L"ProcessConfig");
		for (DWORD i = 0; i < 10000; ++i)
		{
			std::wstring subKeyName;
			if (!regProcessConfigRootKey.GetChildren(i, subKeyName))
			{
				break;
			}
			CRegistryKey regProcessConfigKey;
			regProcessConfigKey.Open(regProcessConfigRootKey, subKeyName.c_str());
			CProcessConfig processConfig;
			processConfig.Filename = regProcessConfigKey.QueryStringValue(L"ExecuteFilePath", nullptr);
			processConfig.Enable = regProcessConfigKey.QueryDWORDValue(L"Enable", 1) != 0;
			processConfig.PriorityClass = regProcessConfigKey.QueryDWORDValue(L"PriorityClass", NORMAL_PRIORITY_CLASS);
			appConfig.Process.emplace(std::wstring(processConfig.Filename), std::move(processConfig));
		}
		return std::move(appConfig);
	}

	void Save(HKEY key, LPCTSTR subKeyName, const CAppConfig& appConfig) const
	{
		CRegistryKey regKey;
		regKey.Create(key, subKeyName, REG_OPTION_NON_VOLATILE, KEY_WRITE);
		return Save(std::move(regKey), appConfig);
	}

	void Save(const CRegistryKey& regKey, const CAppConfig& appConfig) const
	{
		regKey.SetDWORDValue(L"ProcessCheckInterval", appConfig.Service.ProcessCheckInterval);
		regKey.DeleteTree(L"ProcessConfig");
		CRegistryKey regProcessConfigRootKey;
		regProcessConfigRootKey.Create(regKey, L"ProcessConfig");
		for (auto i : appConfig.Process)
		{
			CRegistryKey regProcessConfigKey;
			regProcessConfigKey.Create(regProcessConfigRootKey, i.first.c_str(), REG_OPTION_NON_VOLATILE, KEY_WRITE);
			regProcessConfigKey.SetStringValue(L"ExecuteFilePath", i.second.Filename.c_str());
			regProcessConfigKey.SetDWORDValue(L"Enable", i.second.Enable ? 1 : 0);
			regProcessConfigKey.SetDWORDValue(L"PriorityClass", i.second.PriorityClass);
		}
	}
};
