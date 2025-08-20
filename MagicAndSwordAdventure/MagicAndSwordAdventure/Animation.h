#pragma once

class Animation
{
public:
	Animation();
	~Animation();
	void Init();
	void AttachAnim(int modelHandle, int animNo);
	void UpdateAnim();
	void ChangeAnim(int modelHandle, int animNo ,bool isLoop, float increment);
	void UpdateBlendAnim(int modelHandle, int animNo);
	void StartBlending();
	bool isNowPlaying() { return m_isNowPlaying; }
	bool GetIsAnimEnd();
	int GetAttachAnimNo() { return m_attachAnimNo; }
	float GetPlayTime() { return m_currentPlayTime; }
private:
	// ���Đ�����
	float m_animTotalTime;
	// �Đ�����
	float m_playTime;
	float m_currentPlayTime;
	float m_nextPlayTime;
	// �ȑO�̃A�j���[�V�����̃A�^�b�`�ԍ�
	int m_oldAttachNo;
	// ���݂̃A�j���[�V�����̃A�^�b�`�ԍ�
	int m_currentAttachNo;
	// ���̃A�j���[�V�����̃A�^�b�`�ԍ�
	int m_nextAttachNo;
	// model�̃n���h��
	int m_modelHandle;
	// �A�j���[�V���������[�v�����邩�ۂ�
	bool m_isLoop;
	// �A�j���[�V�������I�����Ă��邩
	bool m_isEnd;
	// �Đ����Ԃ̑���
	float m_timeIncrement;
	// �A�j���[�V�����̃u�����h��
	float m_blendRate;
	// �u�����h�̊J�n
	bool m_isBlending;
	bool m_isNowPlaying;
	float m_blendTime;
	int m_attachAnimNo;
};

