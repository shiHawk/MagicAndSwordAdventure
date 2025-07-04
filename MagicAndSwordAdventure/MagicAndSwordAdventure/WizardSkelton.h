#pragma once
#include "Enemy.h"
#include "Player.h"
class WizardSkelton :public Enemy
{
public:
	WizardSkelton();
	void Init(std::shared_ptr<Player> pPlayer);
	void End();
	void Update() override;
	void DoAttack();
	void Draw()const;
	VECTOR GetAttackPos() { return attack.pos; }
	float GetAttackRadius() { return attack.radius; }
private:
	std::shared_ptr<Player> m_pPlayer;
	// プレイヤーに向かうベクトル
	VECTOR m_toPlayerDir;
};

