#pragma once
#include "Enemy.h"
#include "Player.h"
#include <memory>
class NormalSkelton : public Enemy
{
public:
	NormalSkelton();
	void Init(std::shared_ptr<Player> pPlayer);
	void End();
	void Update() override;
	void DoAttack();
	void OnDamage();
	void Draw()const;
	VECTOR GetAttackPos() { return attack.pos; };
	float GetAttackRadius() { return attack.radius; }
	AttackSphere attack = { 30,{m_pos.x - attack.attackOffSetX,0,0},false,0,0,30,60.0f,40.0,60.0f };
private:
	void TrackPlayer();
	std::shared_ptr<Player> m_pPlayer;
	// プレイヤーに向かうベクトル
	VECTOR m_toPlayerDir;
	// 攻撃に移行するまでの時間
	float m_attackWaitingTime;
};

