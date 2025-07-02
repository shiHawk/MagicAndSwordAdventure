#include "Collision.h"

Collision::Collision():
	m_isPlayerHit(false),
	m_enemyHit(false),
	m_invincibilityTime(0.0f),
	m_playerAttackToEnemyDist(0.0f),
	m_playerToEnemyAttack({0.0f,0.0f,0.0f}),
	m_playerAttackToEnemy({ 0.0f,0.0f,0.0f })
{
}

Collision::~Collision()
{
}

void Collision::Init(std::shared_ptr<Player> pPlayer, std::shared_ptr<Enemy> pEnemy, std::shared_ptr<NormalSkelton> pNormalSkelton)
{
	m_pPlayer = pPlayer;
	m_pEnemy = pEnemy;
	m_pNormalSkelton = pNormalSkelton;
	m_isPlayerHit = false;
	m_invincibilityTime = 60.0f;
	m_playerToEnemyAttack = VSub(m_pPlayer->GetPos(), m_pNormalSkelton->GetAttackPos());
}

void Collision::End()
{
}

void Collision::Update()
{
	// プレイヤーの位置から敵の攻撃位置までの距離
	m_playerToEnemyAttack = VSub(m_pPlayer->GetPos(),m_pNormalSkelton->GetAttackPos());
	// プレイヤーの攻撃位置から敵までの距離
	m_playerAttackToEnemy = VSub(m_pPlayer->GetAttackPos(), m_pNormalSkelton->GetPos());
	float dist = VSize(m_playerToEnemyAttack);
	//printfDx(L"dist:%f\n",dist);
	m_playerAttackToEnemyDist = VSize(m_playerAttackToEnemy);
	if (dist < m_pPlayer->GetColRadius() + m_pNormalSkelton->GetAttackRadius() && !m_isPlayerHit)
	{
		m_isPlayerHit = true;
		if (m_isPlayerHit)
		{
			m_pPlayer->OnDamage();
		}
	}
	if (m_playerAttackToEnemyDist < m_pPlayer->GetAttackRadius() + m_pNormalSkelton->GetColRadius() && !m_enemyHit)
	{
		printfDx(L"Hit\n");
		m_enemyHit = true;
	}

	if (m_isPlayerHit)
	{
		m_invincibilityTime--;
		if (m_invincibilityTime < 0)
		{
			m_isPlayerHit = false;
			m_invincibilityTime = 60.0f;
		}
	}
	if (m_enemyHit)
	{
		m_invincibilityTime--;
		if (m_invincibilityTime < 0)
		{
			m_enemyHit = false;
			m_invincibilityTime = 60.0f;
		}
	}
}

void Collision::Draw()
{
}
