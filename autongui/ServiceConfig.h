#pragma once
class CServiceConfig
{
public:
	CServiceConfig(void);
	virtual ~CServiceConfig(void);

	// レジストリへセーブ
	BOOL SaveToRegistry(HKEY key);

	// レジストリからロード
	BOOL LoadFromRegistry(HKEY key);

	// プロセスのチェック間隔(ミリ秒単位)
	DWORD ProcessCheckInterval;
};
