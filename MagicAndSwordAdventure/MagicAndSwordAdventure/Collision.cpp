#include "Collision.h"

Collision::Collision():
	m_isColl(false),
	m_score(0),
	m_fontHandle(0)
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
	m_fontHandle = CreateFontToHandle(NULL, 40, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
}

void Collision::End()
{
	DeleteFontToHandle(m_fontHandle);
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
}

void Collision::Draw()
{
}

void Collision::CheckCol()
{
	if (!m_isColl)
	{
		
	}
	m_isColl = false;
}
