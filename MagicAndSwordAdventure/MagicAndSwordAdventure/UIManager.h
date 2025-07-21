#pragma once
#include "DxLib.h"
#include "Player.h"
class UIManager
{
public:
	UIManager();
	~UIManager();
	void Init(std::shared_ptr<Player> pPlayer);
	void End();
	void Update();
	void DrawHp();
	void DrawNavigation();
private:
	std::shared_ptr<Player> m_pPlayer;
	float m_hpGaugeRate;
	int m_drawNavigationHandle;
};

