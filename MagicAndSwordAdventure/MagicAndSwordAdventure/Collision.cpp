#include "Collision.h"

Collision::Collision():
	m_isPlayerHit(false),
	m_enemyHit(false),
	m_invincibilityTime(0.0f),
	m_playerAttackToEnemyDist(0.0f),
	m_playerToEnemy({0.0f,0.0f,0.0f}),
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
}

void Collision::End()
{
}

void Collision::Update()
{
	VECTOR m_playerToEnemy = VSub(m_pPlayer->GetPos(), m_pNormalSkelton->GetPos());
	VECTOR m_playerAttackToEnemy = VSub(m_pPlayer->GetAttackPos(), m_pNormalSkelton->GetPos());
	float dist = VSize(m_playerToEnemy);
	m_playerAttackToEnemyDist = VSize(m_playerAttackToEnemy);
	if (dist < m_pPlayer->GetColRadius() + m_pNormalSkelton->GetColRadius() && !m_isPlayerHit)
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
