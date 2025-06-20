#include "Animation.h"
#include "DxLib.h"

Animation::Animation():
	m_animTotalTime(0),
	m_playTime(0),
	m_currentAttachNo(-1),
	m_nextAttachNo(-1)
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
	m_currentAttachNo = MV1AttachAnim(modelHandle,animNo);
	m_animTotalTime = MV1GetAttachAnimTotalTime(modelHandle, m_currentAttachNo);
}

void Animation::Update()
{
}

void Animation::ChangeAnim(int modelHandle, int animNo)
{
	m_nextAttachNo = MV1AttachAnim(modelHandle, animNo);
	if (m_nextAttachNo != m_currentAttachNo)
	{
		MV1DetachAnim(modelHandle, m_currentAttachNo);
		m_currentAttachNo = m_nextAttachNo;
	}
	
}
