#pragma once
#include "DxLib.h"
#include "Player.h"
#include "ScoreManager.h"
#include "Camera.h"
class UIManager
{
public:
	UIManager();
	~UIManager();
	void Init(std::shared_ptr<Player> pPlayer, std::shared_ptr<ScoreManager> pScoreManager, std::shared_ptr<Camera> pCamera);
	void End();
	void Update();
	void Draw();
	void DrawHp();
	void DrawNavigation();
	void DrawDestroyScore(); // ���j�X�R�A
	void DrawElapsedTimeSeconds(); // �o�ߎ���
	void DrawNumberOfEnemiesRemaining(int remainingCount);
	void DrawUIFrame();
private:
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<ScoreManager> m_pScoreManager;
	std::shared_ptr<Camera> m_pCamera;
	// UI�̃n���h��
	float m_hpGaugeRate;
	int m_hpGaugeHandle;
	int m_hpGaugeFrameHandle;
	int m_navigationHandle;
	int m_playerIconHandle;
	int m_playerIconPinchHandle;
	int m_playerHpGaugePinchHandle;
	int m_uiFrameHandle;

	float m_blinkTime;
	float m_blinkProgress; // �_�ł̐i�s�x
	int m_alpha; // �����x
	int m_fontTextHandle; // UI�̕ϓ����Ȃ������̃n���h��
	int m_fontHandle; // UI�̕ϓ����鐔���̃n���h��
};

