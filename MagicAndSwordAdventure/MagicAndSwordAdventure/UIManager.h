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
	void DrawDestroyScore(); // 撃破スコア
	void DrawmElapsedTimeSeconds(); // 経過時間
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
	float m_blinkProgress; // 点滅の進行度
	int m_alpha;
};

