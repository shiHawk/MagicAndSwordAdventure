#pragma once
#include "Enemy.h"
#include "Player.h"
class WizardSkelton :public Enemy
{
public:
	WizardSkelton();
	void Init(std::shared_ptr<Player> pPlayer,VECTOR pos);
	void End();
	void Update() override;
	void DoAttack();
	void OnDamage();
	void OnDeath();
	void Draw()const;
	VECTOR GetAttackPos() { return attack.pos; }
	float GetAttackRadius() { return attack.radius; }
	AttackSphere attack = { 20.0f,{m_pos.x - attack.attackOffSetX,0,m_pos.z},false,0,0,30,30.0f,40.0,60.0f };
private:
	std::shared_ptr<Player> m_pPlayer;
	// プレイヤーに向かうベクトル
	VECTOR m_toPlayerDir;
	bool m_isAttackEnd = false;
};

