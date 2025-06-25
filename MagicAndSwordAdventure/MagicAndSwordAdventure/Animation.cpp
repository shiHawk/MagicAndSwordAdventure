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
	m_blendRate(0.0f)
{
}

Animation::~Animation()
{
}

void Animation::Init()
{
	m_blendRate = 1.0f;
}

void Animation::AttachAnim(int modelHandle, int animNo)
{
	m_modelHandle = modelHandle;
	m_currentAttachNo = MV1AttachAnim(modelHandle,animNo);
	m_animTotalTime = MV1GetAttachAnimTotalTime(modelHandle, m_currentAttachNo);
}

void Animation::UpdateAnim()
{
	m_playTime += m_timeIncrement;
	MV1SetAttachAnimTime(m_modelHandle,m_currentAttachNo,m_playTime);
	if (m_playTime >= m_animTotalTime)
	{
		m_playTime = 0.0f;
	}
}

void Animation::ChangeAnim(int modelHandle, int animNo, bool isLoop, float increment)
{
	m_nextAttachNo = MV1AttachAnim(modelHandle, animNo);
	MV1DetachAnim(modelHandle, m_currentAttachNo);
	m_isLoop = isLoop;
	m_playTime = 0.0f;
	m_timeIncrement = increment;
	m_currentAttachNo = m_nextAttachNo;
	m_animTotalTime = MV1GetAttachAnimTotalTime(modelHandle, m_nextAttachNo);
}
