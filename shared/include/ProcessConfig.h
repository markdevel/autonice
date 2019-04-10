#pragma once

class CProcessConfig
{
public:
	CProcessConfig(void) { }

	virtual ~CProcessConfig(void) { }

	// 実行ファイル名(現在のバージョンではフルパスを許容しない)
	std::wstring Filename;

	// 監視
	bool Enable = true;

	// プロセス優先度
	DWORD PriorityClass = NORMAL_PRIORITY_CLASS;
};
