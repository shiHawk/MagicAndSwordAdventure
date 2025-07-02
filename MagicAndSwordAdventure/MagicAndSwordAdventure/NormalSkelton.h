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
	void Draw()const;
	VECTOR GetAttackPos() { return attack.pos; };
	float GetAttackRadius() { return attack.radius; }
	AttackSphere attack = { 30,{m_pos.x - attack.attackOffSetX,0,0},false,0,0,30,60.0,40.0 };
private:
	void TrackPlayer();
	std::shared_ptr<Player> m_pPlayer;
};

