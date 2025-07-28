#include "Animation.h"
#include "DxLib.h"


Animation::Animation():
	m_animTotalTime(0),
	m_playTime(0),
	m_oldAttachNo(-1),
	m_currentAttachNo(-1),
	m_nextAttachNo(-1),
	m_isLoop(true),
	m_isEnd(false),
	m_modelHandle(-1),
	m_timeIncrement(0.5f),
	m_blendRate(0.0f),
	m_isBlending(false),
	m_isNowPlaying(false),
	m_blendTime(0.0f)
{
}

Animation::~Animation()
{
}

void Animation::Init()
{
}

void Animation::AttachAnim(int modelHandle, int animNo)
{
	m_modelHandle = modelHandle;
	m_currentAttachNo = MV1AttachAnim(modelHandle,animNo);
	m_animTotalTime = MV1GetAttachAnimTotalTime(modelHandle, m_currentAttachNo);
}

void Animation::UpdateAnim()
{
	// �Đ����Ԃ�i�߂�
	m_playTime += m_timeIncrement;
	// �Đ����Ԃ��Z�b�g����
	MV1SetAttachAnimTime(m_modelHandle, m_currentAttachNo, m_playTime);
	if (m_isBlending)
	{
		//UpdateBlendAnim(m_modelHandle, m_nextAttachNo);
	}
	if (m_playTime >= m_animTotalTime)
	{
		if (m_isLoop)
		{
			m_playTime = 0.0f;
			m_isEnd = false;
		}
		else
		{
			m_playTime = m_animTotalTime; // �Ō�̃t���[���Ŏ~�߂�
			m_isEnd = true;
		}
	}
}

void Animation::ChangeAnim(int modelHandle, int animNo, bool isLoop, float increment)
{
	// ���̃A�j���[�V�������A�^�b�`
	m_nextAttachNo = MV1AttachAnim(modelHandle, animNo);
	// ���A�^�b�`����Ă���A�j���[�V�������f�^�b�`
	MV1DetachAnim(modelHandle, m_currentAttachNo);
	m_currentAttachNo = m_nextAttachNo;
	// �A�j���[�V�����̃u�����h���J�n
	StartBlending();
	m_isLoop = isLoop;
	// �Đ����Ԃ����Z�b�g
	m_playTime = 0.0f;
	m_timeIncrement = increment;
	// ���Đ����Ԃ��Đݒ�
	m_animTotalTime = MV1GetAttachAnimTotalTime(modelHandle, m_nextAttachNo);
	m_isEnd = false;
}

void Animation::UpdateBlendAnim(int modelHandle, int animNo)
{
	if (!m_isBlending) return;
	m_blendRate += 0.01;
	if (m_blendRate > 1.0f)
	{
		m_blendRate = 1.0f;
		m_isBlending = false;
	}
	MV1SetAttachAnimBlendRate(m_modelHandle, m_currentAttachNo, 1.0f - m_blendRate);
	MV1SetAttachAnimBlendRate(m_modelHandle, animNo, m_blendRate);
}

void Animation::StartBlending()
{
	m_isBlending = true;
	m_blendRate = 0.0f;
}

bool Animation::GetIsAnimEnd()
{
	if (m_currentAttachNo == -1) return false;
	return m_playTime >= m_animTotalTime;
}
