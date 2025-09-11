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
	VECTOR GetAttackPos() { return m_attack.pos; };
	float GetAttackRadius() { return m_attack.radius; }
	bool IsAttackActive() const { return m_attack.active; }
	
private:
	void TrackPlayer();
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<ScoreManager> m_pScoreManager;
	AttackSphere m_attack;
	// プレイヤーに向かうベクトル
	VECTOR m_toPlayerDir;
	bool m_isMove;
	int m_moveCount;
	int m_attackCount = 0;
	int m_idleCount = 0;
	bool m_isPreparingAttack = false; // 攻撃準備中（構えアニメ中）
	bool m_isPrepared = false; // 構えアニメが完了したか
};

