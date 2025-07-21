#pragma once
#include "Enemy.h"
#include "Player.h"
#include "ScoreManager.h"
#include <memory>
class NormalSkelton : public Enemy
{
public:
	NormalSkelton();
	void Init(std::shared_ptr<Player> pPlayer, VECTOR pos, std::shared_ptr<ScoreManager> pScoreManager);
	void End();
	void Update() override;
	void OnAttack();
	void OnDamage();
	void OnDeath();
	void Draw()const;
	VECTOR GetAttackPos() { return attack.pos; };
	float GetAttackRadius() { return attack.radius; }
	AttackSphere attack = { 30,{m_pos.x - attack.attackOffSetX,0,0},false,0,0,30,60.0f,40.0,60.0f };
private:
	void TrackPlayer();
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<ScoreManager> m_pScoreManager;
	// プレイヤーに向かうベクトル
	VECTOR m_toPlayerDir;
	bool m_isMove;
	int m_moveCount;
};

