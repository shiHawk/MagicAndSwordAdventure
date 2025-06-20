#pragma once
class Animation
{
public:
	Animation();
	~Animation();
	void Init();
	void AttachAnim(int modelHandle, int animNo);
	void Update();
	void ChangeAnim(int modelHandle, int animNo);
private:
	// ���Đ�����
	float m_animTotalTime;
	// �Đ�����
	float m_playTime;
	// ���݂̃A�j���[�V�����̃A�^�b�`�ԍ�
	int m_currentAttachNo;
	// ���̃A�j���[�V�����̃A�^�b�`�ԍ�
	int m_nextAttachNo;
};

