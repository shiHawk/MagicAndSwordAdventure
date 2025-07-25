#pragma once
#include "DxLib.h"
#include "Player.h"
#include "ScoreManager.h"
class UIManager
{
public:
	UIManager();
	~UIManager();
	void Init(std::shared_ptr<Player> pPlayer, std::shared_ptr<ScoreManager> pScoreManager);
	void End();
	void Update();
	void DrawHp();
	void DrawNavigation();
	void DrawDestroyScore(); // ���j�X�R�A
	void DrawmElapsedTimeSeconds(); // �o�ߎ���
	void DrawNumberOfEnemiesRemaining(int remainingCount);
private:
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<ScoreManager> m_pScoreManager;
	float m_hpGaugeRate;
	int m_hpGaugeHandle;
	int m_navigationHandle;
	int m_playerIconHandle;
	int m_playerIconPinchHandle;
	float m_blinkTime;
	float m_blinkProgress; // �_�ł̐i�s�x
	int m_alpha;
};

