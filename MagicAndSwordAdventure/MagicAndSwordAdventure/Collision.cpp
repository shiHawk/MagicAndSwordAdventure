#include "Collision.h"

Collision::Collision():
	m_isColl(false)
{
}

Collision::~Collision()
{
}

void Collision::Init(std::shared_ptr<Player> pPlayer, std::shared_ptr<Enemy> pEnemy)
{
	m_pPlayer = pPlayer;
	m_pEnemy = pEnemy;
	m_isColl = false;
	m_invincibilityTime = 60.0f;
}

void Collision::End()
{
	
}

void Collision::Update()
{
	VECTOR playerToEnemy = VSub(m_pPlayer->GetPos(), m_pEnemy->GetPos());
	float dist = VSize(playerToEnemy);
	if (dist < m_pPlayer->GetColRadius() + m_pEnemy->GetColRadius() && !m_isColl)
	{
		m_isColl = true;
		if (m_isColl)
		{
			m_pPlayer->OnDamage();
		}
	}

	if (m_isColl)
	{
		m_invincibilityTime--;
		if (m_invincibilityTime < 0)
		{
			m_isColl = false;
			m_invincibilityTime = 60.0f;
		}
	}
}

void Collision::Draw()
{
}
