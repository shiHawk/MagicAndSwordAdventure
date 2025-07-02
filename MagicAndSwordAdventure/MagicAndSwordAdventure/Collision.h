#pragma once
#include<DxLib.h>
#include "Player.h"
#include "Enemy.h"
#include<memory>
class Player;
class Enemy;
class Collision
{
public:
	Collision();
	~Collision();

	void Init(std::shared_ptr<Player> pPlayer, std::shared_ptr<Enemy> pEnemy);
	void End();
	void Update();
	void Draw();
private:
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Enemy> m_pEnemy;
	bool m_isPlayerHit;
	bool m_enemyHit;
	float m_invincibilityTime;
	float m_playerAttackToEnemyDist;
	VECTOR m_playerToEnemy; // プレイヤーからエネミー
	VECTOR m_playerAttackToEnemy;
};

