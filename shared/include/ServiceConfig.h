#pragma once

class CServiceConfig
{
public:
	CServiceConfig(void)
		: ProcessCheckInterval(5000) { }

	virtual ~CServiceConfig(void) { }

	// �v���Z�X�̃`�F�b�N�Ԋu(�~���b�P��)
	DWORD ProcessCheckInterval;
};
