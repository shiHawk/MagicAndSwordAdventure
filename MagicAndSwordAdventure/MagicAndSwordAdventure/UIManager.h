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
	void Updata();
	void Draw();
private:
	std::shared_ptr<Player> m_pPlayer;
	float m_hpGaugeRate;
};

