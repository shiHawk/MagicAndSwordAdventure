#include "ScoreManager.h"
#include "DxLib.h"

ScoreManager::ScoreManager():
	m_score(0),
	m_startTime(0),
	m_elapsedTimeSeconds(0.0f),
	m_destroyScore(0),
	m_timeBonus(0),
	m_isPlayerDead(false)
{
}

void ScoreManager::Init()
{
	m_score = 0;
	m_startTime = GetNowCount();
	m_destroyScore = 0;
	m_timeBonus = 0;
	m_elapsedTimeSeconds = 0.0f;
	m_isPlayerDead = false;
}

void ScoreManager::End()
{
	m_score = 0;
	m_startTime = 0;
	m_elapsedTimeSeconds = 0;
	m_isPlayerDead = false;
}

void ScoreManager::Update()
{
	m_elapsedTimeSeconds = static_cast<int>((GetNowCount() - m_startTime) / 1000.0f);
}

void ScoreManager::AddScore(int destroyScore)
{
	m_destroyScore += destroyScore;
}

int ScoreManager::GetDestroyScore()
{
	return m_destroyScore;
}

int ScoreManager::GetTime()
{
	return m_elapsedTimeSeconds;
}

int ScoreManager::GetTimeBonus()
{
	if (m_isPlayerDead)
	{
		m_timeBonus = 0;
	}
	else
	{
		m_timeBonus = 1000 - m_elapsedTimeSeconds;
		if (m_timeBonus < 0) m_timeBonus = 0; // �{�[�i�X���}�C�i�X�ɂȂ�Ȃ��悤��
	}
	return m_timeBonus;
}

int ScoreManager::GetScore()
{
	m_score = m_destroyScore + GetTimeBonus();
	return m_score;
}

void ScoreManager::SetIsPlayerDead(bool isDead)
{
	m_isPlayerDead = isDead;
}
