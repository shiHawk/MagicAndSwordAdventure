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
	void Draw();
	void DrawHp();
	void DrawNavigation();
	void DrawDestroyScore(); // 撃破スコア
	void DrawElapsedTimeSeconds(); // 経過時間
	void DrawNumberOfEnemiesRemaining(int remainingCount);
	void DrawUIFrame();
private:
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<ScoreManager> m_pScoreManager;
	// UIのハンドル
	float m_hpGaugeRate;
	int m_hpGaugeHandle;
	int m_hpGaugeFrameHandle;
	int m_navigationHandle;
	int m_playerIconHandle;
	int m_playerIconPinchHandle;
	int m_playerHpGaugePinchHandle;
	int m_uiFrameHandle;

	float m_blinkTime;
	float m_blinkProgress; // 点滅の進行度
	int m_alpha; // 透明度
	int m_fontTextHandle; // UIの変動しない文字のハンドル
	int m_fontHandle; // UIの変動する数字のハンドル
};

