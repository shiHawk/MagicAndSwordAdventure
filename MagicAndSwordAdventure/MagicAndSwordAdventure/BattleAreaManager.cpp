#include "BattleAreaManager.h"
#include "game.h"
namespace
{
	constexpr int kDivNum = 16;
	constexpr float kBattleAreaRadiusSize = 400.0f;
	constexpr float kBattleTriggerDistance = 300.0f;
	constexpr int kSegmentCount = 64;    // 円周を分割する数
	constexpr float kLineWidth = 10.0f; // ラインの太さ
	// 光るエフェクト用定数
	constexpr float kGlowSpeed = 0.005f;    // 明滅スピード
	constexpr float kGlowMinOffset = 0.5f;      // sin波を0～1に補正するためのオフセット
	constexpr float kGlowAmplitude = 0.5f;      // sin波の振幅
	constexpr float kColorMaxValue = 255.0f;    // 8bitカラー最大値

	// 水色の固定ベースカラー
	constexpr int kBaseRed = 50;                 // 基本赤成分
	constexpr int kBaseGreen = 200;                // 基本緑成分
	constexpr int kBaseBlue = 200;                // 基本青成分（明滅でここから加算）

	// 円形分割用
	constexpr float kTwoPi = DX_PI_F * 2.0f; // 2π
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
	m_battleAreaRadius = kBattleAreaRadiusSize;
	m_battleTriggerDistance = kBattleTriggerDistance;
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

void BattleAreaManager::DrawBattleAreaBodary()
{
	if (m_battleState == State::InBattle)
	{
		// 明滅する輝き計算
		float glow = (sinf(GetNowCount() * kGlowSpeed) * kGlowAmplitude + kGlowMinOffset) * kColorMaxValue;
		int glowColor = GetColor(kBaseRed, kBaseGreen, kBaseBlue + static_cast<int>(glow));
		for (int i = 0; i < kSegmentCount; ++i)
		{
			// 2点分の角度を計算
			float angle1 = kTwoPi * i / kSegmentCount;
			float angle2 = kTwoPi * (i + 1) / kSegmentCount;

			// 円周上の2点を計算
			VECTOR p1 = VGet(
				m_battleAreaCenterPos.x + cosf(angle1) * m_battleAreaRadius,
				m_battleAreaCenterPos.y,
				m_battleAreaCenterPos.z + sinf(angle1) * m_battleAreaRadius
			);

			VECTOR p2 = VGet(
				m_battleAreaCenterPos.x + cosf(angle2) * m_battleAreaRadius,
				m_battleAreaCenterPos.y,
				m_battleAreaCenterPos.z + sinf(angle2) * m_battleAreaRadius
			);
			// 境界線を描画
			DrawLine3D(p1, p2, glowColor);
		}
	}
}

void BattleAreaManager::EnterBattle(const VECTOR& centerPos)
{
	m_battleState = State::InBattle;
	VECTOR screenCenter = ConvScreenPosToWorldPos(VGet(centerPos.x*0.5f,0.0f,0.0f));
	// 画面中央をバトルエリアの中心とする
	m_battleAreaCenterPos = VGet(screenCenter.x,0.0f,0.0f);
	// 戦闘用のカメラに切り替え(カメラは動かさない)
	m_pCamera->ChangeBattleCamera(screenCenter);

	// 以前のアクティブリストを空にする
	m_activeNormalSkeltons.clear();
	m_activeWizardSkeltons.clear();
	// バトルエリア内の敵だけアクティブリストに入れる
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
