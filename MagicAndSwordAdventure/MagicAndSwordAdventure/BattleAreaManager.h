#pragma once
#include "Camera.h"
#include "Player.h"
#include "NormalSkelton.h"
#include "WizardSkelton.h"
#include "DxLib.h"
#include <vector>
#include <memory>
class BattleAreaManager
{
public:
	BattleAreaManager();
	~BattleAreaManager();
	void Init(std::shared_ptr<Player> pPlayer, std::shared_ptr<Camera> pCamera);
	void SetEnemys(const std::vector<std::shared_ptr<NormalSkelton>>& normalSkeltons,
				   std::vector<std::shared_ptr<WizardSkelton>>& wizardSkeltons);
	void Updata(std::vector<std::shared_ptr<NormalSkelton>>& normalSkeltons,
		std::vector<std::shared_ptr<WizardSkelton>>& wizardSkeltons);
	void DebugDraw();
	bool IsInBattle(); // 戦闘中か
	bool IsFinished(); // 戦闘が終了しているか
	void DrawBattleAreaBodary();
private:
	enum class State
	{
		None, // 戦闘中でない 
		InBattle, // 戦闘中
		Finish // 戦闘終了
	};

	State m_battleState;
	VECTOR m_battleAreaCenterPos; // バトルエリアの中心
	float m_battleAreaRadius; // バトルエリアの範囲
	float m_battleTriggerDistance; // 戦闘を開始する距離

	float m_playerToNormalSkeltonDistance; // プレイヤーからNormalSkeltonまでの距離の大きさ
	float m_playerToWizardSkeltonDistance; // プレイヤーからWizardSkeltonまでの距離の大きさ

	VECTOR m_battleAreaCenterPosToPlayer; // プレイヤーからバトルエリアの中心までの距離
	VECTOR battleAreaCenterPosToPlayerDir; // プレイヤーからバトルエリアの中心までの距離の向き
	float m_battleAreaCenterPosToPlayerDistance; // プレイヤーからバトルエリアの中心までの距離の大きさ
	VECTOR m_afterCorrectionPos; // 補正した後の位置

	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Camera> m_pCamera;
	std::vector<std::shared_ptr<NormalSkelton>> m_normalSkeltons;
	std::vector<std::shared_ptr<WizardSkelton>> m_wizardSkeltons;
	// 現在バトルエリア内にいる敵のリスト
	std::vector<std::shared_ptr<NormalSkelton>> m_activeNormalSkeltons;
	std::vector<std::shared_ptr<WizardSkelton>> m_activeWizardSkeltons;
	// 戦闘開始
	void EnterBattle(const VECTOR& centerPos);

	// プレイヤーの移動制限
	void ConstraintPlayerMovement();

	//  バトルが終わっているかをチェック
	void CheckBattleEnd(std::vector<std::shared_ptr<NormalSkelton>>& normalSkeltons,
		std::vector<std::shared_ptr<WizardSkelton>>& wizardSkeltons);
};

