#pragma once

class CProcessConfig
{
public:
	CProcessConfig(void) { }

	virtual ~CProcessConfig(void) { }

	// ���s�t�@�C����(���݂̃o�[�W�����ł̓t���p�X�����e���Ȃ�)
	std::wstring Filename;

	// �Ď�
	bool Enable = true;

	// �v���Z�X�D��x
	DWORD PriorityClass = NORMAL_PRIORITY_CLASS;
};
