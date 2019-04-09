#pragma once

class CProcessConfig
{
public:
	CProcessConfig(void);
	virtual ~CProcessConfig(void);

	// ���W�X�g���փZ�[�u
	BOOL SaveToRegistry(HKEY key);

	// ���W�X�g�����烍�[�h
	BOOL LoadFromRegistry(HKEY key, LPCTSTR keyName);

	// �Ď�
	// 1 : ����, 0 : ���Ȃ�
	BOOL Enable;

	// ���s�t�@�C����
	CString ExeFileName;

	// �v���Z�X�D��x
	DWORD PriorityClass;
};
