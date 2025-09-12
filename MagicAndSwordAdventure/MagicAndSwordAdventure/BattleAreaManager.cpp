#include "BattleAreaManager.h"
#include "game.h"
namespace
{
	constexpr int kDivNum = 16;
	constexpr float kBattleAreaRadiusSize = 400.0f;
	constexpr float kBattleTriggerDistance = 300.0f;
	constexpr int kSegmentCount = 64;    // �~���𕪊����鐔
	constexpr float kLineWidth = 10.0f; // ���C���̑���
	// ����G�t�F�N�g�p�萔
	constexpr float kGlowSpeed = 0.005f;    // ���ŃX�s�[�h
	constexpr float kGlowMinOffset = 0.5f;      // sin�g��0�`1�ɕ␳���邽�߂̃I�t�Z�b�g
	constexpr float kGlowAmplitude = 0.5f;      // sin�g�̐U��
	constexpr float kColorMaxValue = 255.0f;    // 8bit�J���[�ő�l

	// ���F�̌Œ�x�[�X�J���[
	constexpr int kBaseRed = 50;                 // ��{�Ԑ���
	constexpr int kBaseGreen = 200;                // ��{�ΐ���
	constexpr int kBaseBlue = 200;                // ��{�����i���łł���������Z�j

	// �~�`�����p
	constexpr float kTwoPi = DX_PI_F * 2.0f; // 2��
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
			// ���łɓG�����S���Ă���Ȃ�ʂ�NormalSkelton���`�F�b�N
			if (normalSkelton->IsDead()) continue;
			m_playerToNormalSkeltonDistance = VSize(VSub(normalSkelton->GetPos(), m_pPlayer->GetPos()));
			// �G�ƃv���C���[�܂ł̋��������ȉ��ɂȂ�����
			if (m_playerToNormalSkeltonDistance < m_battleTriggerDistance)
			{
				m_battleState = State::None; // �퓬��ԂɈڍs�ł���悤�퓬���ł͂Ȃ���Ԃɖ߂�
				break;
			}
		}

		for (auto& wizardSkelton : m_wizardSkeltons)
		{
			// ���łɓG�����S���Ă���Ȃ�ʂ�NormalSkelton���`�F�b�N
			if (wizardSkelton->IsDead()) continue;
			m_playerToWizardSkeltonDistance = VSize(VSub(wizardSkelton->GetPos(), m_pPlayer->GetPos()));
			// �G�ƃv���C���[�܂ł̋��������ȉ��ɂȂ�����
			if (m_playerToWizardSkeltonDistance < m_battleTriggerDistance)
			{
				m_battleState = State::None;// �퓬��ԂɈڍs�ł���悤�퓬���ł͂Ȃ���Ԃɖ߂�
				break;
			}
		}
	}
	// �܂��퓬��ԂɂȂ��Ă��Ȃ��ꍇ
	if (m_battleState == State::None)
	{
		for (auto& normalSkelton : m_normalSkeltons)
		{
			// ���łɓG�����S���Ă���Ȃ�ʂ�NormalSkelton���`�F�b�N
			if (normalSkelton->IsDead()) continue;
			m_playerToNormalSkeltonDistance = VSize(VSub(normalSkelton->GetPos(), m_pPlayer->GetPos()));
			// �G�ƃv���C���[�܂ł̋��������ȉ��ɂȂ�����
			if (m_playerToNormalSkeltonDistance < m_battleTriggerDistance)
			{
				EnterBattle(m_pPlayer->GetPos()); // �퓬�J�n
				break;
			}
		}

		for (auto& wizardSkelton : m_wizardSkeltons)
		{
			// ���łɓG�����S���Ă���Ȃ�ʂ�NormalSkelton���`�F�b�N
			if (wizardSkelton->IsDead()) continue;
			m_playerToWizardSkeltonDistance = VSize(VSub(wizardSkelton->GetPos(), m_pPlayer->GetPos()));
			// �G�ƃv���C���[�܂ł̋��������ȉ��ɂȂ�����
			if (m_playerToWizardSkeltonDistance < m_battleTriggerDistance)
			{
				EnterBattle(m_pPlayer->GetPos()); // �퓬�J�n
				break;
			}
		}
	}
	else if(m_battleState == State::InBattle)
	{
		// �v���C���[�̈ړ��𐧌�
		ConstraintPlayerMovement();
		// �o�g�����I����Ă��邩���`�F�b�N
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
		// ���ł���P���v�Z
		float glow = (sinf(GetNowCount() * kGlowSpeed) * kGlowAmplitude + kGlowMinOffset) * kColorMaxValue;
		int glowColor = GetColor(kBaseRed, kBaseGreen, kBaseBlue + static_cast<int>(glow));
		for (int i = 0; i < kSegmentCount; ++i)
		{
			// 2�_���̊p�x���v�Z
			float angle1 = kTwoPi * i / kSegmentCount;
			float angle2 = kTwoPi * (i + 1) / kSegmentCount;

			// �~�����2�_���v�Z
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
			// ���E����`��
			DrawLine3D(p1, p2, glowColor);
		}
	}
}

void BattleAreaManager::EnterBattle(const VECTOR& centerPos)
{
	m_battleState = State::InBattle;
	VECTOR screenCenter = ConvScreenPosToWorldPos(VGet(centerPos.x*0.5f,0.0f,0.0f));
	// ��ʒ������o�g���G���A�̒��S�Ƃ���
	m_battleAreaCenterPos = VGet(screenCenter.x,0.0f,0.0f);
	// �퓬�p�̃J�����ɐ؂�ւ�(�J�����͓������Ȃ�)
	m_pCamera->ChangeBattleCamera(screenCenter);

	// �ȑO�̃A�N�e�B�u���X�g����ɂ���
	m_activeNormalSkeltons.clear();
	m_activeWizardSkeltons.clear();
	// �o�g���G���A���̓G�����A�N�e�B�u���X�g�ɓ����
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
	// �o�g���G���A����o�悤�Ƃ�����
	if (m_battleAreaCenterPosToPlayerDistance > m_battleAreaRadius)
	{
		battleAreaCenterPosToPlayerDir = VNorm(m_battleAreaCenterPosToPlayer); // ���S�܂ł̋����𐳋K��
		// �o�g���G���A�̉��ɖ߂�
		m_afterCorrectionPos = VAdd(m_battleAreaCenterPos,VScale(battleAreaCenterPosToPlayerDir,m_battleAreaRadius));
		m_pPlayer->SetPos(m_afterCorrectionPos);
	}
}

void BattleAreaManager::CheckBattleEnd(std::vector<std::shared_ptr<NormalSkelton>>& normalSkeltons,
	std::vector<std::shared_ptr<WizardSkelton>>& wizardSkeltons)
{
	for (auto& normalSkelton : m_activeNormalSkeltons)
	{
		// �����Ă���G������Ȃ�
		if (!normalSkelton->IsDead()) return; // �����𑱂���
	}

	for (auto& wizardSkelton : m_activeWizardSkeltons)
	{
		// �����Ă���G������Ȃ�
		if (!wizardSkelton->IsDead()) return; // �����𑱂���
	}

	// �����Ă���G�����Ȃ��Ȃ�Finish�ɐ؂�ւ���
	m_battleState = State::Finish;
	m_pCamera->ChangeNormalCamera();
}
