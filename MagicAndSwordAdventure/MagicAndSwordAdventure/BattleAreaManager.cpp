#include "BattleAreaManager.h"

BattleAreaManager::BattleAreaManager():
	m_battleAreaCenterPos({0.0f,0.0f,0.0f}),
	m_battleAreaRadius(0.0f),
	m_battleState(State::None),
	m_playerToNormalSkeltonDistance(0.0f),
	m_playerToWizardSkeltonDistance(0.0f),
	m_playerToBattleAreaCenterPosDistance(0.0f),
	m_afterCorrectionPos({ 0.0f,0.0f,0.0f }),
	m_playerToBattleAreaCenterPos({ 0.0f,0.0f,0.0f }),
	m_playerToBattleAreaCenterPosDir({ 0.0f,0.0f,0.0f })
{
}

BattleAreaManager::~BattleAreaManager()
{
}

void BattleAreaManager::Init(std::shared_ptr<Player> pPlayer)
{
	m_pPlayer = pPlayer;
	m_battleAreaCenterPos = { 0.0f,0.0f,0.0f };
	m_battleAreaRadius = 150.0f;
	m_battleState = State::None;
}

void BattleAreaManager::Updata(std::vector<std::shared_ptr<NormalSkelton>>& normalSkeltons,
	std::vector<std::shared_ptr<WizardSkelton>>& wizardSkeltons)
{
	if (m_battleState == State::Finish)
	{
		for (auto& normalSkelton : m_NormalSkeltons)
		{
			// すでに敵が死亡しているなら別のNormalSkeltonをチェック
			if (normalSkelton->IsDead()) continue;
			m_playerToNormalSkeltonDistance = VSize(VSub(normalSkelton->GetPos(), m_pPlayer->GetPos()));
			// 敵とプレイヤーまでの距離が一定以下になったら
			if (m_playerToNormalSkeltonDistance < m_battleAreaRadius)
			{
				m_battleState = State::None;
				break;
			}
		}

		for (auto& wizardSkelton : m_WizardSkeltons)
		{
			// すでに敵が死亡しているなら別のNormalSkeltonをチェック
			if (wizardSkelton->IsDead()) continue;
			m_playerToWizardSkeltonDistance = VSize(VSub(wizardSkelton->GetPos(), m_pPlayer->GetPos()));
			// 敵とプレイヤーまでの距離が一定以下になったら
			if (m_playerToNormalSkeltonDistance < m_battleAreaRadius)
			{
				m_battleState = State::None;
				break;
			}
		}
	}
	// まだ戦闘状態になっていない場合
	if (m_battleState == State::None)
	{
		for (auto& normalSkelton : m_NormalSkeltons)
		{
			// すでに敵が死亡しているなら別のNormalSkeltonをチェック
			if (normalSkelton->IsDead()) continue;
			m_playerToNormalSkeltonDistance = VSize(VSub(normalSkelton->GetPos(), m_pPlayer->GetPos()));
			// 敵とプレイヤーまでの距離が一定以下になったら
			if (m_playerToNormalSkeltonDistance < m_battleAreaRadius)
			{
				EnterBattle(m_pPlayer->GetPos());
				break;
			}
		}

		for (auto& wizardSkelton : m_WizardSkeltons)
		{
			// すでに敵が死亡しているなら別のNormalSkeltonをチェック
			if (wizardSkelton->IsDead()) continue;
			m_playerToWizardSkeltonDistance = VSize(VSub(wizardSkelton->GetPos(), m_pPlayer->GetPos()));
			// 敵とプレイヤーまでの距離が一定以下になったら
			if (m_playerToNormalSkeltonDistance < m_battleAreaRadius)
			{
				EnterBattle(m_pPlayer->GetPos());
				break;
			}
		}
	}
	else if(m_battleState == State::InBattle)
	{
		// プレイヤーの移動を制限
		ConstraintPlayerMovement();
		// バトルが終わっているかをチェック
		CheckBattleEnd(m_NormalSkeltons,m_WizardSkeltons);
	}
}

bool BattleAreaManager::IsInBattle()
{
	return m_battleState == State::InBattle;
}

bool BattleAreaManager::IsFinished()
{
	return m_battleState == State::Finish;
}

void BattleAreaManager::EnterBattle(const VECTOR& centerPos)
{
	m_battleState = State::InBattle;
	m_battleAreaCenterPos = VAdd(centerPos,VGet(50.0f,0.0f,0.0f));
}

void BattleAreaManager::ConstraintPlayerMovement()
{
	m_playerToBattleAreaCenterPos = VSub(m_battleAreaCenterPos, m_pPlayer->GetPos());
	m_playerToBattleAreaCenterPosDistance = VSize(m_playerToBattleAreaCenterPos);
	if (m_playerToBattleAreaCenterPosDistance > m_battleAreaRadius)
	{
		m_playerToBattleAreaCenterPosDir = VNorm(m_playerToBattleAreaCenterPos); // 中心までの距離を正規化
		// バトルエリアの縁に戻す
		m_afterCorrectionPos = VAdd(m_battleAreaCenterPos,VScale(m_playerToBattleAreaCenterPosDir,m_battleAreaRadius));
		m_pPlayer->SetPos(m_afterCorrectionPos);
	}
}

void BattleAreaManager::CheckBattleEnd(std::vector<std::shared_ptr<NormalSkelton>>& normalSkeltons,
	std::vector<std::shared_ptr<WizardSkelton>>& wizardSkeltons)
{
	for (auto& normalSkelton : m_NormalSkeltons)
	{
		// 生きている敵がいるなら
		if (!normalSkelton->IsDead())
		{
			return; // 処理を続ける
		}
	}

	for (auto& wizardSkelton : m_WizardSkeltons)
	{
		// 生きている敵がいるなら
		if (!wizardSkelton->IsDead())
		{
			return; // 処理を続ける
		}
	}

	// 生きている敵がいないならFinishに切り替える
	m_battleState = State::Finish;
}
