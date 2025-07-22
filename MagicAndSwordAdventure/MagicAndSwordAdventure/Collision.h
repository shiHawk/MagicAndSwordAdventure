#pragma once
#include<DxLib.h>
#include "Player.h"
#include "Enemy.h"
#include "NormalSkelton.h"
#include "WizardSkelton.h"
#include<memory>
#include <vector>
class Player;
class Enemy;
class NormalSkelton;
class Collision
{
public:
	Collision();
	~Collision();

	void Init(std::shared_ptr<Player> pPlayer, 
		std::vector<std::shared_ptr<NormalSkelton>> normalSkeltons,
		std::vector<std::shared_ptr<WizardSkelton>> wizardSkeltons);
	void End();
	void Update();
	void PlayerHit(float enemyAttackToPlayer,float playerRadius,float enemyAttackRadius,bool enemyAttackActive,int enemyPower);
	void EnemyHit(float playerAttackToEnemy, float playerRadius, float enemyAttackRadius,bool playerAttackActive,bool enemyHit);
	bool GetIsPlayerHit() { return m_isPlayerHit; }
private:
	std::shared_ptr<Player> m_pPlayer;
	std::vector<std::shared_ptr<NormalSkelton>> m_normalSkeltons;
	std::vector<std::shared_ptr<WizardSkelton>> m_wizardSkeltons;

	// プレイヤーが攻撃に当たったか
	bool m_isPlayerHit;
	// NormalSkeltonが攻撃に当たったか
	std::vector<bool> m_normalSkeltonHit;
	std::vector<float> m_normalSkeltonInvincibilityTime;
	// WizardSkeltonが攻撃に当たったか
	std::vector<bool> m_wizardSkeltonHit;
	std::vector<float> m_wizardSkeltonInvincibilityTime;

	// 無敵時間
	float m_invincibilityTime;
	// プレイヤーの無敵時間
	float m_playerInvincibilityTime;

	// 敵とプレイヤーが重なっている部分
	VECTOR m_differencePushedBack;
	// 敵とプレイヤーが重なっている部分の大きさ
	float m_differencePushedBackSize;
	// 重なったかどうかを判断する大きさ
	float m_overlapSize;
	// 押し戻す方向
	VECTOR m_pushDir;
	// 実際に押し戻される距離
	VECTOR m_pushBack;

	float m_playerAttackToNormalSkeltonDistance; // プレイヤーの攻撃からNormalSkeltonまでの距離の大きさ
	float m_NormalSkeltonAttackToPlayerDistance; // NormalSkeltonの攻撃からプレイヤーまでの距離の大きさ
	float m_playerAttackToWizardSkeltonDistance; // プレイヤーの攻撃からWizardSkeltonまでの距離の大きさ
	float m_WizardSkeltonAttackToPlayerDistance; // WizardSkeltonの攻撃からプレイヤーまでの距離の大きさ
	VECTOR m_playerToNormalSkeltonAttack; // プレイヤーからNormalSkeltonの攻撃
	VECTOR m_playerAttackToNormalSkelton; // プレイヤーの攻撃からNormalSkelton
	VECTOR m_playerToWizardSkeltonAttack; // プレイヤーからWizardSkeltonの攻撃
	VECTOR m_playerAttackToWizardSkelton; // プレイヤーの攻撃からWizardSkelton
};

