#include "Animation.h"
#include "DxLib.h"

Animation::Animation():
	m_animTotalTime(0),
	m_playTime(0),
	m_currentAttachNo(-1),
	m_nextAttachNo(-1),
	m_isLoop(true),
	m_isEnd(false),
	m_modelHandle(-1)
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

void Animation::Update()
{
	m_playTime += 0.5f;
	MV1SetAttachAnimTime(m_modelHandle,m_currentAttachNo,m_playTime);
	if (m_playTime >= m_animTotalTime)
	{
		m_playTime = 0.0f;
	}
}

void Animation::ChangeAnim(int modelHandle, int animNo, bool isLoop)
{
	m_nextAttachNo = MV1AttachAnim(modelHandle, animNo);
	if (m_nextAttachNo != m_currentAttachNo)
	{
		MV1DetachAnim(modelHandle, m_currentAttachNo);
		m_isLoop = isLoop;
		m_playTime = 0.0f;
		m_currentAttachNo = m_nextAttachNo;
		m_animTotalTime = MV1GetAttachAnimTotalTime(modelHandle, m_currentAttachNo);
	}
	
}
