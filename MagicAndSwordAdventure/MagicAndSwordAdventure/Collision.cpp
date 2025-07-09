#include "Collision.h"

Collision::Collision():
	m_isPlayerHit(false),
	m_enemyHit(false),
	m_invincibilityTime(0.0f),
	m_playerAttackToNormalSkeltonDistance(0.0f),
	m_NormalSkeltonAttackToPlayerDistance(0.0f),
	m_playerToEnemyAttack({0.0f,0.0f,0.0f}),
	m_playerAttackToEnemy({ 0.0f,0.0f,0.0f })
{
}

Collision::~Collision()
{
}

void Collision::Init(std::shared_ptr<Player> pPlayer, std::shared_ptr<Enemy> pEnemy, std::shared_ptr<NormalSkelton> pNormalSkelton, std::shared_ptr<WizardSkelton> pWizardSkelton)
{
	m_pPlayer = pPlayer;
	m_pEnemy = pEnemy;
	m_pNormalSkelton = pNormalSkelton;
	m_pWizardSkelton = pWizardSkelton;
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
	m_playerToEnemyAttack = VSub(m_pPlayer->GetPos(), m_pNormalSkelton->GetAttackPos());
	// プレイヤーの攻撃位置から敵までの距離
	m_playerAttackToEnemy = VSub(m_pPlayer->GetAttackPos(), m_pNormalSkelton->GetPos());
	m_NormalSkeltonAttackToPlayerDistance = VSize(m_playerToEnemyAttack);
	//printfDx(L"m_enemyAttackToPlayerDistance:%f\n",m_enemyAttackToPlayerDistance);
	m_playerAttackToNormalSkeltonDistance = VSize(m_playerAttackToEnemy);
	// プレイヤーに敵の攻撃が当たったか
	if (m_NormalSkeltonAttackToPlayerDistance < m_pPlayer->GetColRadius() + m_pNormalSkelton->GetAttackRadius() && !m_isPlayerHit)
	{
		m_isPlayerHit = true;
		if (m_isPlayerHit && m_pNormalSkelton->GetAttackActive())
		{
			m_pPlayer->OnDamage();
		}
	}
	// プレイヤーの攻撃が敵に当たったか
	if (m_playerAttackToNormalSkeltonDistance < m_pPlayer->GetAttackRadius() + m_pNormalSkelton->GetColRadius() && !m_enemyHit)
	{
		if (m_pPlayer->GetAttackActive())
		{
			printfDx(L"Hit\n");
			m_enemyHit = true;
		}
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
