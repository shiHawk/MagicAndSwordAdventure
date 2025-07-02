#pragma once
#include<DxLib.h>
#include "Player.h"
#include "Enemy.h"
#include "NormalSkelton.h"
#include<memory>
class Player;
class Enemy;
class NormalSkelton;
class Collision
{
public:
	Collision();
	~Collision();

	void Init(std::shared_ptr<Player> pPlayer, std::shared_ptr<Enemy> pEnemy, std::shared_ptr<NormalSkelton> pNormalSkelton);
	void End();
	void Update();
	void Draw();
private:
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Enemy> m_pEnemy;
	std::shared_ptr<NormalSkelton> m_pNormalSkelton;
	// プレイヤーが攻撃が当たったか
	bool m_isPlayerHit;
	// エネミーが攻撃に当たったか
	bool m_enemyHit;
	// 無敵時間
	float m_invincibilityTime;
	float m_playerAttackToEnemyDist;
	VECTOR m_playerToEnemyAttack; // プレイヤーからエネミー
	VECTOR m_playerAttackToEnemy;
};

