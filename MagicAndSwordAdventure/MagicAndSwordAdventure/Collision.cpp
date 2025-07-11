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

void Collision::Init(std::shared_ptr<Player> pPlayer, std::shared_ptr<Enemy> pEnemy, std::shared_ptr<NormalSkelton> pNormalSkelton,
	std::shared_ptr<WizardSkelton> pWizardSkelton)
{
	m_pPlayer = pPlayer;
	m_pEnemy = pEnemy;
	m_pNormalSkelton = pNormalSkelton;
	m_pWizardSkelton = pWizardSkelton;
	m_isPlayerHit = false;
	m_invincibilityTime = 60.0f;
	m_playerToNormalSkeltonAttack = VSub(m_pPlayer->GetPos(), m_pNormalSkelton->GetAttackPos());
}

void Collision::End()
{
}

void Collision::Update()
{
	// �v���C���[�̈ʒu����NormalSkelton�̍U���ʒu�܂ł̋���
	m_playerToNormalSkeltonAttack = VSub(m_pPlayer->GetPos(), m_pNormalSkelton->GetAttackPos());
	// �v���C���[�̈ʒu����WizardSkelton�̍U���ʒu�܂ł̋���
	m_playerToWizardSkeltonAttack = VSub(m_pPlayer->GetPos(), m_pWizardSkelton->GetAttackPos());

	// �v���C���[�̍U���ʒu����NormalSkelton�܂ł̋���
	m_playerAttackToNormalSkelton = VSub(m_pPlayer->GetAttackPos(), m_pNormalSkelton->GetPos());
	// �v���C���[�̍U���ʒu����WizardSkelton�܂ł̋���
	m_playerAttackToWizardSkelton = VSub(m_pPlayer->GetAttackPos(), m_pWizardSkelton->GetPos());

	m_NormalSkeltonAttackToPlayerDistance = VSize(m_playerToNormalSkeltonAttack);
	m_playerAttackToNormalSkeltonDistance = VSize(m_playerAttackToNormalSkelton);

	m_WizardSkeltonAttackToPlayerDistance = VSize(m_playerToWizardSkeltonAttack);
	m_playerAttackToWizardSkeltonDistance = VSize(m_playerAttackToWizardSkelton);

	// �v���C���[��NormalSkelton�̍U��������������
	PlayerHit(m_NormalSkeltonAttackToPlayerDistance, m_pPlayer->GetColRadius(),
		      m_pNormalSkelton->GetAttackRadius(), m_pNormalSkelton->GetAttackActive());
	// �v���C���[��WizardSkelton�̍U��������������
	PlayerHit(m_WizardSkeltonAttackToPlayerDistance, m_pPlayer->GetColRadius(), 
			  m_pWizardSkelton->GetAttackRadius(), m_pWizardSkelton->GetAttackActive());
	
	// �v���C���[�̍U����NormalSkelton�ɓ���������
	if (m_playerAttackToNormalSkeltonDistance < m_pPlayer->GetAttackRadius() + m_pNormalSkelton->GetColRadius() && !m_normalSkeltonHit)
	{
		if (m_pPlayer->GetAttackActive())
		{
			m_pNormalSkelton->OnDamage();
			printfDx(L"Hit\n");
			m_normalSkeltonHit = true;
		}
	}
	// �v���C���[�̍U����WizardSkelton�ɓ���������
	if (m_playerAttackToWizardSkeltonDistance < m_pPlayer->GetAttackRadius() + m_pWizardSkelton->GetColRadius() && !m_wizardSkeltonHit)
	{
		if (m_pPlayer->GetAttackActive())
		{
			//printfDx(L"Hit\n");
			m_pWizardSkelton->OnDamage(m_pWizardSkelton->GetHp(), m_pPlayer->GetPower());
			m_wizardSkeltonHit = true;
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
	if (m_normalSkeltonHit && m_pNormalSkelton->GetHp() >= 0)
	{
		m_invincibilityTime--;
		if (m_invincibilityTime < 0)
		{
			m_normalSkeltonHit = false;
			m_invincibilityTime = 60.0f;
		}
	}
	if (m_wizardSkeltonHit)
	{
		m_invincibilityTime--;
		if (m_invincibilityTime < 0)
		{
			m_wizardSkeltonHit = false;
			m_invincibilityTime = 60.0f;
		}
	}
}

void Collision::PlayerHit(float enemyAttackToPlayer, float playerRadius, float enemyAttackRadius, bool enemyAttackActive)
{
	// �v���C���[��enemy�̍U��������������
	if (enemyAttackToPlayer < playerRadius + enemyAttackRadius && !m_isPlayerHit)
	{
		m_isPlayerHit = true;
		if (m_isPlayerHit && enemyAttackActive)
		{
			m_pPlayer->OnDamage();
		}
	}
}

void Collision::EnemyHit(float playerAttackToEnemy, float playerRadius, float enemyAttackRadius, bool playerAttackActive,bool enemyHit)
{
	// �v���C���[�̍U�����G�ɓ���������
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
