#include "BattleAreaManager.h"
#include "game.h"
namespace
{
	constexpr int kDivNum = 16;
}
BattleAreaManager::BattleAreaManager():
	m_battleAreaCenterPos({0.0f,0.0f,0.0f}),
	m_battleAreaRadius(0.0f),
	m_battleState(State::None),
	m_playerToNormalSkeltonDistance(0.0f),
	m_playerToWizardSkeltonDistance(0.0f),
	m_battleAreaCenterPosToPlayerDistance(0.0f),
	m_afterCorrectionPos({ 0.0f,0.0f,0.0f }),
	m_battleAreaCenterPosToPlayer({ 0.0f,0.0f,0.0f }),
	battleAreaCenterPosToPlayerDir({ 0.0f,0.0f,0.0f }),
	m_battleTriggerDistance(0.0f)
{
}

BattleAreaManager::~BattleAreaManager()
{
}

void BattleAreaManager::Init(std::shared_ptr<Player> pPlayer, std::shared_ptr<Camera> pCamera)
{
	m_pPlayer = pPlayer;
	m_pCamera = pCamera;
	m_battleAreaCenterPos = { 0.0f,0.0f,0.0f };
	m_battleAreaRadius = 350.0f;
	m_battleTriggerDistance = 300.0f;
	m_battleState = State::None;
}

void BattleAreaManager::SetEnemys(const std::vector<std::shared_ptr<NormalSkelton>>& normalSkeltons, std::vector<std::shared_ptr<WizardSkelton>>& wizardSkeltons)
{
	m_normalSkeltons = normalSkeltons;
	m_wizardSkeltons = wizardSkeltons;
}

void BattleAreaManager::Updata(std::vector<std::shared_ptr<NormalSkelton>>& normalSkeltons,
	std::vector<std::shared_ptr<WizardSkelton>>& wizardSkeltons)
{
	if (m_battleState == State::Finish)
	{
		for (auto& normalSkelton : m_normalSkeltons)
		{
			// すでに敵が死亡しているなら別のNormalSkeltonをチェック
			if (normalSkelton->IsDead()) continue;
			m_playerToNormalSkeltonDistance = VSize(VSub(normalSkelton->GetPos(), m_pPlayer->GetPos()));
			// 敵とプレイヤーまでの距離が一定以下になったら
			if (m_playerToNormalSkeltonDistance < m_battleTriggerDistance)
			{
				m_battleState = State::None; // 戦闘状態に移行できるよう戦闘中ではない状態に戻す
				break;
			}
		}

		for (auto& wizardSkelton : m_wizardSkeltons)
		{
			// すでに敵が死亡しているなら別のNormalSkeltonをチェック
			if (wizardSkelton->IsDead()) continue;
			m_playerToWizardSkeltonDistance = VSize(VSub(wizardSkelton->GetPos(), m_pPlayer->GetPos()));
			// 敵とプレイヤーまでの距離が一定以下になったら
			if (m_playerToWizardSkeltonDistance < m_battleTriggerDistance)
			{
				m_battleState = State::None;// 戦闘状態に移行できるよう戦闘中ではない状態に戻す
				break;
			}
		}
	}
	// まだ戦闘状態になっていない場合
	if (m_battleState == State::None)
	{
		for (auto& normalSkelton : m_normalSkeltons)
		{
			// すでに敵が死亡しているなら別のNormalSkeltonをチェック
			if (normalSkelton->IsDead()) continue;
			m_playerToNormalSkeltonDistance = VSize(VSub(normalSkelton->GetPos(), m_pPlayer->GetPos()));
			// 敵とプレイヤーまでの距離が一定以下になったら
			if (m_playerToNormalSkeltonDistance < m_battleTriggerDistance)
			{
				EnterBattle(m_pPlayer->GetPos()); // 戦闘開始
				break;
			}
		}

		for (auto& wizardSkelton : m_wizardSkeltons)
		{
			// すでに敵が死亡しているなら別のNormalSkeltonをチェック
			if (wizardSkelton->IsDead()) continue;
			m_playerToWizardSkeltonDistance = VSize(VSub(wizardSkelton->GetPos(), m_pPlayer->GetPos()));
			// 敵とプレイヤーまでの距離が一定以下になったら
			if (m_playerToWizardSkeltonDistance < m_battleTriggerDistance)
			{
				EnterBattle(m_pPlayer->GetPos()); // 戦闘開始
				break;
			}
		}
	}
	else if(m_battleState == State::InBattle)
	{
		// プレイヤーの移動を制限
		ConstraintPlayerMovement();
		// バトルが終わっているかをチェック
		CheckBattleEnd(m_normalSkeltons,m_wizardSkeltons);
	}
}

void BattleAreaManager::DebugDraw()
{
	if (m_battleState == State::InBattle)
	{
		DrawSphere3D(m_battleAreaCenterPos, m_battleAreaRadius, kDivNum, 0xff00ff, 0xffffff,false);
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
	VECTOR screenCenter = ConvScreenPosToWorldPos(VGet(Game::kScreenWidth*0.5f,0.0f,0.0f));
	// 画面中央をバトルエリアの中心とする
	m_battleAreaCenterPos = VGet(m_pPlayer->GetPos().x,0.0f,0.0f);
	// 戦闘用のカメラに切り替え(カメラは動かさない)
	m_pCamera->ChangeBattleCamera(m_pPlayer->GetPos());

	// バトルエリア内の敵だけアクティブに入れる
	// 以前のアクティブリストを空にする
	m_activeNormalSkeltons.clear();
	m_activeWizardSkeltons.clear();

	for (auto& normalSkelton : m_normalSkeltons)
	{
		if (!normalSkelton->IsDead() && VSize(VSub(normalSkelton->GetPos(), m_battleAreaCenterPos)) < m_battleTriggerDistance)
		{
			m_activeNormalSkeltons.push_back(normalSkelton);
		}
	}
	for (auto& wizardSkelton : m_wizardSkeltons)
	{
		if (!wizardSkelton->IsDead() && VSize(VSub(wizardSkelton->GetPos(), m_battleAreaCenterPos)) < m_battleTriggerDistance)
		{
			m_activeWizardSkeltons.push_back(wizardSkelton);
		}
	}
}

void BattleAreaManager::ConstraintPlayerMovement()
{
	m_battleAreaCenterPosToPlayer = VSub(m_pPlayer->GetPos(),m_battleAreaCenterPos);
	m_battleAreaCenterPosToPlayerDistance = VSize(m_battleAreaCenterPosToPlayer);
	// バトルエリアから出ようとしたら
	if (m_battleAreaCenterPosToPlayerDistance > m_battleAreaRadius)
	{
		battleAreaCenterPosToPlayerDir = VNorm(m_battleAreaCenterPosToPlayer); // 中心までの距離を正規化
		// バトルエリアの縁に戻す
		m_afterCorrectionPos = VAdd(m_battleAreaCenterPos,VScale(battleAreaCenterPosToPlayerDir,m_battleAreaRadius));
		m_pPlayer->SetPos(m_afterCorrectionPos);
	}
}

void BattleAreaManager::CheckBattleEnd(std::vector<std::shared_ptr<NormalSkelton>>& normalSkeltons,
	std::vector<std::shared_ptr<WizardSkelton>>& wizardSkeltons)
{
	for (auto& normalSkelton : m_activeNormalSkeltons)
	{
		// 生きている敵がいるなら
		if (!normalSkelton->IsDead()) return; // 処理を続ける
	}

	for (auto& wizardSkelton : m_activeWizardSkeltons)
	{
		// 生きている敵がいるなら
		if (!wizardSkelton->IsDead()) return; // 処理を続ける
	}

	// 生きている敵がいないならFinishに切り替える
	m_battleState = State::Finish;
	m_pCamera->ChangeNormalCamera();
}
