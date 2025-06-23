#pragma once
class Animation
{
public:
	Animation();
	~Animation();
	void Init();
	void AttachAnim(int modelHandle, int animNo);
	void Update();
	void ChangeAnim(int modelHandle, int animNo ,bool isLoop);
private:
	// ���Đ�����
	float m_animTotalTime;
	// �Đ�����
	float m_playTime;
	// ���݂̃A�j���[�V�����̃A�^�b�`�ԍ�
	int m_currentAttachNo;
	// ���̃A�j���[�V�����̃A�^�b�`�ԍ�
	int m_nextAttachNo;
	// model�̃n���h��
	int m_modelHandle;
	// �A�j���[�V���������[�v�����邩�ۂ�
	bool m_isLoop;
	// �A�j���[�V�������I��点�邩
	bool m_isEnd;
};

