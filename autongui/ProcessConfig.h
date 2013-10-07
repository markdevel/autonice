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
	BOOL Enable = 1;

	// 実行ファイル名(フルパスの場合、そうでない場合、双方ある)
	CString ExeFilePath;

	// プロセス優先度
	DWORD PriorityClass = NORMAL_PRIORITY_CLASS;
};
