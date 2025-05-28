#include "Pad.h"
#include "DxLib.h"

namespace
{
	constexpr int kLogNum = 16;
	// ���̓��O	0���ŐV�̏��
	int padLog[kLogNum];
}


void Pad::Update()
{
	// ���݂̃p�b�h�̏�Ԃ��擾
	int padState = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	// ���O�̍X�V
	for (int i = kLogNum - 1; i >= 1; i--)
	{
		padLog[i] = padLog[i - 1];
	}
	// �ŐV�̏��
	padLog[0] = padState;
}

// ������������
bool Pad::isPress(int button)
{
	return (padLog[0] & button);
}

// �g���K�[����
bool Pad::isTrigger(int button)
{
	bool isNow = (padLog[0] & button);	// ���݂̏��
	bool isLast = (padLog[1] & button);	// �P�t���[���O�̏��
	return (isNow && !isLast);
}

// ����������
bool Pad::isRelase(int button)
{
	bool isNow = (padLog[0] & button);	// ���݂̏��
	bool isLast = (padLog[1] & button);	// �P�t���[���O�̏��
	return (!isNow && isLast);
}
