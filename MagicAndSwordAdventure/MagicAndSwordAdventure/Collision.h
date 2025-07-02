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
	bool m_isPlayerHit;
	bool m_enemyHit;
	float m_invincibilityTime;
	float m_playerAttackToEnemyDist;
	VECTOR m_playerToEnemy; // プレイヤーからエネミー
	VECTOR m_playerAttackToEnemy;
};

