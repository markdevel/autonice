#pragma once
class CServiceConfig
{
public:
	CServiceConfig(void);
	virtual ~CServiceConfig(void);

	// ���W�X�g���փZ�[�u
	BOOL SaveToRegistry(HKEY key);

	// ���W�X�g�����烍�[�h
	BOOL LoadFromRegistry(HKEY key);

	// �v���Z�X�̃`�F�b�N�Ԋu(�~���b�P��)
	DWORD ProcessCheckInterval;
};
