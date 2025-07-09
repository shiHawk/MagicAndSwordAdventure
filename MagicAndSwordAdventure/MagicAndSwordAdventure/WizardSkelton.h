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
	AttackSphere attack = { 20.0f,{m_pos.x - attack.attackOffSetX,0,0},false,0,0,30,60.0f,40.0,60.0f };
private:
	std::shared_ptr<Player> m_pPlayer;
	// プレイヤーに向かうベクトル
	VECTOR m_toPlayerDir;
	bool m_isAttackEnd = false;
};

