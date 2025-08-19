#include "Collision.h"

Collision::Collision():
	m_isPlayerHit(false),
	m_normalSkeltonHit(false),
	m_wizardSkeltonHit(false),
	m_invincibilityTime(0.0f),
	m_playerInvincibilityTime(0.0f),
	m_playerAttackToNormalSkeltonDistance(0.0f),
	m_NormalSkeltonAttackToPlayerDistance(0.0f),
	m_playerToNormalSkeltonAttack({0.0f,0.0f,0.0f}),
	m_playerAttackToNormalSkelton({ 0.0f,0.0f,0.0f }),
	m_differencePushedBack({0.0f,0.0f,0.0f}),
	m_differencePushedBackSize(0.0f),
	m_overlapSize(0.0f),
	m_pushDir({ 0.0f,0.0f,0.0f }),
	m_pushBack({ 0.0f,0.0f,0.0f }),
	m_WizardSkeltonAttackToPlayerDistance(0.0f),
	m_playerAttackToWizardSkelton({ 0.0f,0.0f,0.0f }),
	m_playerAttackToWizardSkeltonDistance(0.0f),
	m_playerToWizardSkeltonAttack({ 0.0f,0.0f,0.0f }),
	m_playerHitPos({ 0.0f,0.0f,0.0f }),
	m_normalSkeltonHitPos({ 0.0f,0.0f,0.0f }),
	m_wizardSkeltonHitPos({ 0.0f,0.0f,0.0f })
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
	m_normalSkeltonHit.resize(m_normalSkeltons.size(), false);
	m_normalSkeltonInvincibilityTime.resize(m_normalSkeltons.size(), 0.0f);
	m_wizardSkeltonHit.resize(m_wizardSkeltons.size(), false);
	m_wizardSkeltonInvincibilityTime.resize(m_wizardSkeltons.size(), 0.0f);
	m_invincibilityTime = 160.0f;
	m_playerInvincibilityTime = 100.0f;
	m_playerToNormalSkeltonAttack = VGet(200.0f,0.0f,0.0f);
}

void Collision::End()
{
}

void Collision::Update()
{
	//printfDx("m_isPlayerHit:%d\n", m_isPlayerHit);
	for (size_t i = 0; i < m_normalSkeltons.size(); ++i)
	{
		auto& normalSkelton = m_normalSkeltons[i];
		m_differencePushedBack = VSub(m_pPlayer->GetPos(), normalSkelton->GetPos());
		m_differencePushedBackSize = VSize(m_differencePushedBack);
		m_overlapSize = m_pPlayer->GetColRadius() + normalSkelton->GetColRadius() - m_differencePushedBackSize;

		// プレイヤーと重なっているならプレイヤーを押し戻す
		if (m_overlapSize > 0.0f)
		{
			m_pushDir = VNorm(m_differencePushedBack); // 敵→プレイヤー方向
			m_pushBack = VScale(m_pushDir, m_overlapSize);
			m_pPlayer->AddPos(m_pushBack);
		}

		// プレイヤーの位置からNormalSkeltonの攻撃位置までの距離
		m_playerToNormalSkeltonAttack = VSub(m_pPlayer->GetPos(), normalSkelton->GetAttackPos());
		// プレイヤーの攻撃位置からNormalSkeltonまでの距離
		m_playerAttackToNormalSkelton = VSub(m_pPlayer->GetAttackPos(), normalSkelton->GetPos());

		m_NormalSkeltonAttackToPlayerDistance = VSize(m_playerToNormalSkeltonAttack);
		m_playerAttackToNormalSkeltonDistance = VSize(m_playerAttackToNormalSkelton);

		// プレイヤーにNormalSkeltonの攻撃が当たったか
		PlayerHit(m_NormalSkeltonAttackToPlayerDistance, m_pPlayer->GetColRadius(),
			normalSkelton->GetAttackRadius(), normalSkelton->IsAttackActive(), normalSkelton->GetPower(),normalSkelton->GetAttackPos());
		// プレイヤーの攻撃がNormalSkeltonに当たったか
		if (m_playerAttackToNormalSkeltonDistance < m_pPlayer->GetAttackRadius() + normalSkelton->GetColRadius() && !m_normalSkeltonHit[i])
		{
			if (m_pPlayer->IsAttackActive())
			{
				normalSkelton->OnDamage();
				m_normalSkeltonHit[i] = true;
				m_normalSkeltonInvincibilityTime[i] = 60.0f;
				m_normalSkeltonHitPos = CalcHitPosition(m_pPlayer->GetAttackPos(),normalSkelton->GetPos());
				m_normalSkeltonHitPos.y = m_normalSkeltonHitPos.y * 2;
			}
		}
		if (m_normalSkeltonHit[i] && normalSkelton->GetHp() >= 0)
		{
			// 無敵時間の更新
			m_normalSkeltonInvincibilityTime[i]--;
			if (m_normalSkeltonInvincibilityTime[i] < 0)
			{
				m_normalSkeltonHit[i] = false;
			}
		}
	}
	
	for (size_t i = 0; i < m_wizardSkeltons.size(); ++i)
	{
		auto& wizardSkelton = m_wizardSkeltons[i];
		m_differencePushedBack = VSub(m_pPlayer->GetPos(), wizardSkelton->GetPos());
		m_differencePushedBackSize = VSize(m_differencePushedBack);
		m_overlapSize = m_pPlayer->GetColRadius() + wizardSkelton->GetColRadius() - m_differencePushedBackSize;

		// プレイヤーと重なっているならプレイヤーを押し戻す
		if (m_overlapSize > 0.0f)
		{
			m_pushDir = VNorm(m_differencePushedBack); // 敵→プレイヤー方向
			m_pushBack = VScale(m_pushDir, m_overlapSize);
			m_pPlayer->AddPos(m_pushBack);
		}
		// プレイヤーの位置からWizardSkeltonの攻撃位置までの距離
		m_playerToWizardSkeltonAttack = VSub(m_pPlayer->GetPos(), wizardSkelton->GetAttackPos());

		// プレイヤーの攻撃位置からWizardSkeltonまでの距離
		m_playerAttackToWizardSkelton = VSub(m_pPlayer->GetAttackPos(), wizardSkelton->GetPos());

		m_WizardSkeltonAttackToPlayerDistance = VSize(m_playerToWizardSkeltonAttack);
		m_playerAttackToWizardSkeltonDistance = VSize(m_playerAttackToWizardSkelton);

		// プレイヤーにWizardSkeltonの攻撃が当たったか
		PlayerHit(m_WizardSkeltonAttackToPlayerDistance, m_pPlayer->GetColRadius(),
			wizardSkelton->GetAttackRadius(), wizardSkelton->IsAttackActive(), wizardSkelton->GetPower(),wizardSkelton->GetAttackPos());

		// プレイヤーの攻撃がWizardSkeltonに当たったか
		if (m_playerAttackToWizardSkeltonDistance < m_pPlayer->GetAttackRadius() + wizardSkelton->GetColRadius() && !m_wizardSkeltonHit[i])
		{
			if (m_pPlayer->IsAttackActive())
			{
				wizardSkelton->OnDamage();
				m_wizardSkeltonHit[i] = true;
				m_wizardSkeltonInvincibilityTime[i] = 60.0f;
				m_wizardSkeltonHitPos = CalcHitPosition(m_pPlayer->GetAttackPos(), wizardSkelton->GetPos());
				m_wizardSkeltonHitPos.y = m_wizardSkeltonHitPos.y * 2;
			}
		}
		if (m_wizardSkeltonHit[i] && wizardSkelton->GetHp() >= 0)
		{
			// 無敵時間の更新
			m_wizardSkeltonInvincibilityTime[i]--;
			if (m_wizardSkeltonInvincibilityTime[i] < 0)
			{
				m_wizardSkeltonHit[i] = false;
			}
		}
	}
	
	if (m_isPlayerHit)
	{
		m_playerInvincibilityTime--;
		if (m_playerInvincibilityTime < 0)
		{
			m_isPlayerHit = false;
		}
	}
}

void Collision::PlayerHit(float enemyAttackToPlayer, float playerRadius, float enemyAttackRadius, bool enemyAttackActive, int enemyPower, VECTOR enemyAttackPos)
{
	// プレイヤーにenemyの攻撃が当たったか
	if (enemyAttackToPlayer < playerRadius + enemyAttackRadius && !m_isPlayerHit && enemyAttackActive)
	{
		m_isPlayerHit = true;
		m_pPlayer->OnDamage(enemyPower);
		m_playerInvincibilityTime = 100.0f;
		m_playerHitPos = CalcHitPosition(enemyAttackPos,m_pPlayer->GetPos());
		m_playerHitPos.y = m_playerHitPos.y * 2;
	}
}

void Collision::EnemyHit(float playerAttackToEnemy, float playerRadius, float enemyAttackRadius, bool playerAttackActive,bool enemyHit)
{
	// プレイヤーの攻撃が敵に当たったか
	if (playerAttackToEnemy < playerRadius + enemyAttackRadius && !enemyHit)
	{
		if (playerAttackActive)
		{
			enemyHit = true;
			m_invincibilityTime = 1000.0f;
		}
	}
}

VECTOR Collision::CalcHitPosition(VECTOR attackCenter, VECTOR targetCenter)
{
	return VScale(VAdd(attackCenter, targetCenter),0.5f);
}
