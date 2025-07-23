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
	bool IsAttackActive() const { return attack.active; }
	AttackSphere attack = { 30,{m_pos.x - attack.attackOffSetX,0,0},false,0,0,30,60.0f,40.0,60.0f };
private:
	void TrackPlayer();
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<ScoreManager> m_pScoreManager;
	// プレイヤーに向かうベクトル
	VECTOR m_toPlayerDir;
	bool m_isMove;
	int m_moveCount;
	int m_attackCount = 0;
	int m_idleCount = 0;
	bool m_isPreparingAttack = false; // 攻撃準備中（構えアニメ中）
	bool m_isPrepared = false; // 構えアニメが完了したか
	int m_weponModelHandel;
};

