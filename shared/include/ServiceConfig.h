#pragma once

class CServiceConfig
{
public:
	CServiceConfig(void)
		: ProcessCheckInterval(5000) { }

	virtual ~CServiceConfig(void) { }

	// プロセスのチェック間隔(ミリ秒単位)
	DWORD ProcessCheckInterval;
};
