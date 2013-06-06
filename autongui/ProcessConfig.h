#pragma once

class CProcessConfig
{
public:
	CProcessConfig(void);
	virtual ~CProcessConfig(void);

	// レジストリへセーブ
	BOOL SaveToRegistry(HKEY key);

	// レジストリからロード
	BOOL LoadFromRegistry(HKEY key, LPCTSTR keyName);

	// 監視
	// 1 : する, 0 : しない
	BOOL Enable;

	// 実行ファイル名
	CString ExeFileName;

	// プロセス優先度
	DWORD PriorityClass;
};
