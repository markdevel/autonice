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
	BOOL Enable = 1;

	// ���s�t�@�C����(�t���p�X�̏ꍇ�A�����łȂ��ꍇ�A�o������)
	CString ExeFilePath;

	// �v���Z�X�D��x
	DWORD PriorityClass = NORMAL_PRIORITY_CLASS;
};
