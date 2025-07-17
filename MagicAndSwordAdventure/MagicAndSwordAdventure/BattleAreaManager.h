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
	void Updata(std::vector<std::shared_ptr<NormalSkelton>>& normalSkeltons,
		std::vector<std::shared_ptr<WizardSkelton>>& wizardSkeltons);
	bool IsInBattle(); // 戦闘中か
	bool IsFinished(); // 戦闘が終了しているか
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

	float m_playerToNormalSkeltonDistance; // プレイヤーからNormalSkeltonまでの距離の大きさ
	float m_playerToWizardSkeltonDistance; // プレイヤーからWizardSkeltonまでの距離の大きさ

	VECTOR m_playerToBattleAreaCenterPos; // プレイヤーからバトルエリアの中心までの距離
	VECTOR m_playerToBattleAreaCenterPosDir; // プレイヤーからバトルエリアの中心までの距離の向き
	float m_playerToBattleAreaCenterPosDistance; // プレイヤーからバトルエリアの中心までの距離の大きさ
	VECTOR m_afterCorrectionPos; // 補正した後の位置

	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Camera> m_pCamera;
	std::vector<std::shared_ptr<NormalSkelton>> m_NormalSkeltons;
	std::vector<std::shared_ptr<WizardSkelton>> m_WizardSkeltons;
	/// <summary>
	/// 戦闘開始
	/// </summary>
	void EnterBattle(const VECTOR& centerPos);

	/// <summary>
	/// プレイヤーの移動制限
	/// </summary>
	void ConstraintPlayerMovement();

	/// <summary>
	///  バトルが終わっているかをチェック
	/// </summary>
	void CheckBattleEnd(std::vector<std::shared_ptr<NormalSkelton>>& normalSkeltons,
		std::vector<std::shared_ptr<WizardSkelton>>& wizardSkeltons);
};

