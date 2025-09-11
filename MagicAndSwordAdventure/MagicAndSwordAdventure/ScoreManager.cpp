#include "ScoreManager.h"
#include "DxLib.h"
namespace
{
	constexpr int kMaxTimeBonus = 1000;                // タイムボーナス計算の上限基準
	constexpr float kMillisecondsPerSecond = 1000.0f;  // ミリ秒から秒への変換
	constexpr int kTimeBonusMultiplier = 20;           // 経過時間をボーナスに変換する倍率
	constexpr int kHpBonusMultiplier = 12;             // HPをボーナスに変換する倍率
}

ScoreManager::ScoreManager():
	m_score(0),
	m_startTime(0),
	m_elapsedTimeSeconds(0.0f),
	m_destroyScore(0),
	m_timeBonus(0),
	m_isPlayerDead(false),
	m_hpBonus(0)
{
}

void ScoreManager::Init()
{
	m_score = 0;
	m_startTime = GetNowCount();
	m_destroyScore = 0;
	m_timeBonus = 0;
	m_elapsedTimeSeconds = 0.0f;
	m_hpBonus = 0;
	m_isPlayerDead = false;
}

void ScoreManager::End()
{
	m_score = 0;
	m_startTime = 0;
	m_elapsedTimeSeconds = 0;
	m_hpBonus = 0;
	m_isPlayerDead = false;
}

void ScoreManager::Update()
{
	m_elapsedTimeSeconds = static_cast<int>((GetNowCount() - m_startTime) / kMillisecondsPerSecond);
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
		m_timeBonus = (kMaxTimeBonus - m_elapsedTimeSeconds) * kTimeBonusMultiplier;
		if (m_timeBonus < 0) m_timeBonus = 0; // ボーナスがマイナスにならないように
	}
	return m_timeBonus;
}

int ScoreManager::GetScore()
{
	m_score = m_destroyScore + GetTimeBonus();
	return m_score;
}

void ScoreManager::HpBonus(int hp)
{
	m_hpBonus = hp * kHpBonusMultiplier; // hpボーナスを設定
	if (m_isPlayerDead)
	{
		m_hpBonus = 0; // もし死亡した場合はボーナスはなし
	}
}

int ScoreManager::GetHpBonus()
{
	return m_hpBonus;
}

void ScoreManager::SetIsPlayerDead(bool isDead)
{
	m_isPlayerDead = isDead;
}
