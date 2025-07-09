#pragma once
#include<DxLib.h>
#include "Player.h"
#include "Enemy.h"
#include "NormalSkelton.h"
#include "WizardSkelton.h"
#include<memory>
class Player;
class Enemy;
class NormalSkelton;
class Collision
{
public:
	Collision();
	~Collision();

	void Init(std::shared_ptr<Player> pPlayer, std::shared_ptr<Enemy> pEnemy, 
			  std::shared_ptr<NormalSkelton> pNormalSkelton, std::shared_ptr<WizardSkelton> pWizardSkelton);
	void End();
	void Update();
	void Draw();
private:
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Enemy> m_pEnemy;
	std::shared_ptr<NormalSkelton> m_pNormalSkelton;
	std::shared_ptr<WizardSkelton> m_pWizardSkelton;
	// プレイヤーが攻撃が当たったか
	bool m_isPlayerHit;
	// エネミーが攻撃に当たったか
	bool m_enemyHit;
	// 無敵時間
	float m_invincibilityTime;
	float m_playerAttackToNormalSkeltonDistance;
	float m_NormalSkeltonAttackToPlayerDistance;
	VECTOR m_playerToEnemyAttack; // プレイヤーからエネミーの攻撃
	VECTOR m_playerAttackToEnemy; // プレイヤーの攻撃から敵
};

