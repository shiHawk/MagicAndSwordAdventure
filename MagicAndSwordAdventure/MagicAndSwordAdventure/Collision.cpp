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
}

void Collision::End()
{
	
}

void Collision::Update()
{
	VECTOR playerToEnemy = VSub(m_pPlayer->GetPos(), m_pEnemy->GetPos());
	float dist = VSize(playerToEnemy);
	if (dist < m_pPlayer->GetColRadius() + m_pEnemy->GetColRadius())
	{
		m_pPlayer->OnDamage();
		m_isColl = true;
	}
	else
	{
		m_isColl = false;
	}
}

void Collision::Draw()
{
}
