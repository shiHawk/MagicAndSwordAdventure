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
	float m_battleTriggerDistance; // �퓬���J�n���鋗��

	float m_playerToNormalSkeltonDistance; // �v���C���[����NormalSkelton�܂ł̋����̑傫��
	float m_playerToWizardSkeltonDistance; // �v���C���[����WizardSkelton�܂ł̋����̑傫��

	VECTOR m_battleAreaCenterPosToPlayer; // �v���C���[����o�g���G���A�̒��S�܂ł̋���
	VECTOR battleAreaCenterPosToPlayerDir; // �v���C���[����o�g���G���A�̒��S�܂ł̋����̌���
	float m_battleAreaCenterPosToPlayerDistance; // �v���C���[����o�g���G���A�̒��S�܂ł̋����̑傫��
	VECTOR m_afterCorrectionPos; // �␳������̈ʒu

	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Camera> m_pCamera;
	std::vector<std::shared_ptr<NormalSkelton>> m_normalSkeltons;
	std::vector<std::shared_ptr<WizardSkelton>> m_wizardSkeltons;
	// ���݃o�g���G���A���ɂ���G�̃��X�g
	std::vector<std::shared_ptr<NormalSkelton>> m_activeNormalSkeltons;
	std::vector<std::shared_ptr<WizardSkelton>> m_activeWizardSkeltons;
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

