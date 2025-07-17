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
	bool IsInBattle(); // �퓬����
	bool IsFinished(); // �퓬���I�����Ă��邩
private:
	enum class State
	{
		None, // �퓬���łȂ� 
		InBattle, // �퓬��
		Finish // �퓬�I��
	};

	State m_battleState;
	VECTOR m_battleAreaCenterPos; // �o�g���G���A�̒��S
	float m_battleAreaRadius; // �o�g���G���A�͈̔�

	float m_playerToNormalSkeltonDistance; // �v���C���[����NormalSkelton�܂ł̋����̑傫��
	float m_playerToWizardSkeltonDistance; // �v���C���[����WizardSkelton�܂ł̋����̑傫��

	VECTOR m_playerToBattleAreaCenterPos; // �v���C���[����o�g���G���A�̒��S�܂ł̋���
	VECTOR m_playerToBattleAreaCenterPosDir; // �v���C���[����o�g���G���A�̒��S�܂ł̋����̌���
	float m_playerToBattleAreaCenterPosDistance; // �v���C���[����o�g���G���A�̒��S�܂ł̋����̑傫��
	VECTOR m_afterCorrectionPos; // �␳������̈ʒu

	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Camera> m_pCamera;
	std::vector<std::shared_ptr<NormalSkelton>> m_NormalSkeltons;
	std::vector<std::shared_ptr<WizardSkelton>> m_WizardSkeltons;
	/// <summary>
	/// �퓬�J�n
	/// </summary>
	void EnterBattle(const VECTOR& centerPos);

	/// <summary>
	/// �v���C���[�̈ړ�����
	/// </summary>
	void ConstraintPlayerMovement();

	/// <summary>
	///  �o�g�����I����Ă��邩���`�F�b�N
	/// </summary>
	void CheckBattleEnd(std::vector<std::shared_ptr<NormalSkelton>>& normalSkeltons,
		std::vector<std::shared_ptr<WizardSkelton>>& wizardSkeltons);
};

