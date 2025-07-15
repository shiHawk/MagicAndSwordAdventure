#include "Collision.h"

Collision::Collision():
	m_isPlayerHit(false),
	m_normalSkeltonHit(false),
	m_wizardSkeltonHit(false),
	m_invincibilityTime(0.0f),
	m_playerAttackToNormalSkeltonDistance(0.0f),
	m_NormalSkeltonAttackToPlayerDistance(0.0f),
	m_playerToNormalSkeltonAttack({0.0f,0.0f,0.0f}),
	m_playerAttackToNormalSkelton({ 0.0f,0.0f,0.0f })
{
}

Collision::~Collision()
{
}

void Collision::Init(std::shared_ptr<Player> pPlayer,std::vector<std::shared_ptr<NormalSkelton>> normalSkeltons,
	std::vector<std::shared_ptr<WizardSkelton>> wizardSkeltons)
{
	m_pPlayer = pPlayer;
	m_normalSkeltons = normalSkeltons;
	m_wizardSkeltons = wizardSkeltons;
	m_isPlayerHit = false;
	m_invincibilityTime = 160.0f;
	m_playerToNormalSkeltonAttack = VGet(200.0f,0.0f,0.0f);
}

void Collision::End()
{
}

void Collision::Update()
{
	for (auto& normalSkelton : m_normalSkeltons)
	{
		// プレイヤーの位置からNormalSkeltonの攻撃位置までの距離
		m_playerToNormalSkeltonAttack = VSub(m_pPlayer->GetPos(), normalSkelton->GetAttackPos());
		// プレイヤーの攻撃位置からNormalSkeltonまでの距離
		m_playerAttackToNormalSkelton = VSub(m_pPlayer->GetAttackPos(), normalSkelton->GetPos());

		m_NormalSkeltonAttackToPlayerDistance = VSize(m_playerToNormalSkeltonAttack);
		m_playerAttackToNormalSkeltonDistance = VSize(m_playerAttackToNormalSkelton);

		// プレイヤーにNormalSkeltonの攻撃が当たったか
		PlayerHit(m_NormalSkeltonAttackToPlayerDistance, m_pPlayer->GetColRadius(),
			normalSkelton->GetAttackRadius(), normalSkelton->GetAttackActive(),normalSkelton->GetPower());
		// プレイヤーの攻撃がNormalSkeltonに当たったか
		if (m_playerAttackToNormalSkeltonDistance < m_pPlayer->GetAttackRadius() + normalSkelton->GetColRadius() && !m_normalSkeltonHit)
		{
			if (m_pPlayer->GetAttackActive())
			{
				normalSkelton->OnDamage();
				//printfDx(L"Hit\n");
				m_normalSkeltonHit = true;
			}
		}

		if (m_normalSkeltonHit && normalSkelton->GetHp() >= 0)
		{
			m_invincibilityTime--;
			//printfDx(L"m_invincibilityTime:%f\n", m_invincibilityTime);
			if (m_invincibilityTime < 0)
			{
				m_normalSkeltonHit = false;
				m_invincibilityTime = 160.0f;
			}
		}
	}
	
	for (auto& wizardSkelton : m_wizardSkeltons)
	{
		// プレイヤーの位置からWizardSkeltonの攻撃位置までの距離
		m_playerToWizardSkeltonAttack = VSub(m_pPlayer->GetPos(), wizardSkelton->GetAttackPos());

		// プレイヤーの攻撃位置からWizardSkeltonまでの距離
		m_playerAttackToWizardSkelton = VSub(m_pPlayer->GetAttackPos(), wizardSkelton->GetPos());

		m_WizardSkeltonAttackToPlayerDistance = VSize(m_playerToWizardSkeltonAttack);
		m_playerAttackToWizardSkeltonDistance = VSize(m_playerAttackToWizardSkelton);

		// プレイヤーにWizardSkeltonの攻撃が当たったか
		PlayerHit(m_WizardSkeltonAttackToPlayerDistance, m_pPlayer->GetColRadius(),
			wizardSkelton->GetAttackRadius(), wizardSkelton->GetAttackActive(),wizardSkelton->GetPower());

		// プレイヤーの攻撃がWizardSkeltonに当たったか
		if (m_playerAttackToWizardSkeltonDistance < m_pPlayer->GetAttackRadius() + wizardSkelton->GetColRadius() && !m_wizardSkeltonHit)
		{
			if (m_pPlayer->GetAttackActive())
			{
				//printfDx(L"Hit\n");
				wizardSkelton->OnDamage();
				m_wizardSkeltonHit = true;
			}
		}
		if (m_wizardSkeltonHit && wizardSkelton->GetHp() >= 0)
		{
			m_invincibilityTime--;
			if (m_invincibilityTime < 0)
			{
				m_wizardSkeltonHit = false;
				m_invincibilityTime = 160.0f;
			}
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
	
	
}

void Collision::PlayerHit(float enemyAttackToPlayer, float playerRadius, float enemyAttackRadius, bool enemyAttackActive, int enemyPower)
{
	// プレイヤーにenemyの攻撃が当たったか
	if (enemyAttackToPlayer < playerRadius + enemyAttackRadius && !m_isPlayerHit)
	{
		m_isPlayerHit = true;
		if (m_isPlayerHit && enemyAttackActive)
		{
			m_pPlayer->OnDamage(enemyPower);
		}
	}
}

void Collision::EnemyHit(float playerAttackToEnemy, float playerRadius, float enemyAttackRadius, bool playerAttackActive,bool enemyHit)
{
	// プレイヤーの攻撃が敵に当たったか
	if (playerAttackToEnemy < playerRadius + enemyAttackRadius && !enemyHit)
	{
		if (playerAttackActive)
		{
			printfDx(L"Hit\n");
			enemyHit = true;
		}
	}
}

void Collision::Draw()
{
}
