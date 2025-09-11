#include "NormalSkelton.h"
#include "DxLib.h"
#include <cmath>

namespace
{
	constexpr VECTOR kLeftDir = { 0.0,90.0f * DX_PI_F / 180.0f,0.0f };
	constexpr VECTOR kRightDir = { 0.0,270.0f * DX_PI_F / 180.0f,0.0f };
	constexpr float kColRadius = 25.0f; // �G�{�̂̓����蔻��
	constexpr float kSerchRange = 500.0f; // ���G�͈�
	constexpr float kAttackRange = 90.0f; // �U���͈�
	constexpr float kMoveSpeed = 4.0f; // �ړ����x
	constexpr float kAttckOffSet = 40.0f;
	constexpr float kBackLimit = 270.0f;
	// ����
	constexpr float kMoveDecRate = 0.80f;
	constexpr float kDefaultAttackCoolTime = 70.0f; // �N�[���^�C��
	constexpr float kDefaultPreparingTime = 115.0f; // �ǐՂ���U���Ɉڂ鎞��
	constexpr float kAttackDuration = 27.0f; // �U���̎�������

	// �A�j���[�V�����̔ԍ�
	constexpr int kIdleAnimNo = 41;
	constexpr int kWalkAnimNo = 54;
	constexpr int kTakeAStandAnimNo = 83;
	constexpr int kAttackAnimNo = 5;
	constexpr int kDamageAnimNo = 40;
	constexpr int kDeathAnimNo = 25;
	// �A�j���[�V�����̑��x
	constexpr float kAnimSpeedFast = 0.5f; // �Z�߂̍Đ��̎�
	constexpr float kAnimSpeedDeath = 0.4f; // ���S���̍Đ����x
	constexpr float kAnimSpeedSlow = 1.0f; // ���߂̍Đ��̎�
	constexpr float kAnimSpeedakeAStand = 0.3f; // �\���Ă���Ƃ��̍Đ����x
	// �ő�HP
	constexpr int kMaxHp = 100;
	// �G�̍U����
	constexpr int kPower = 40;
	// �_�Ŏ���
	constexpr int kBlinkCycleMs = 500;
	// ���f���̃X�P�[��
	constexpr int kModelScale = 45;
	// �������ł��鎞��
	constexpr float kKnockBackSpeed = 5.0f;
	constexpr float kKnockbackDuration = 0.5f;
	// ���j�X�R�A
	constexpr int kDestroyScore = 300;
	// �U���͈̔�
	constexpr float kAttackRadius = 30.0f;
	// �b���ϊ�
	constexpr float kFramesPerSecond = 60.0f;
	// ��ʒ[�̃I�t�Z�b�g
	constexpr float kLimitPosOffSet = 0.001f;
	// ���S��̃|�W�V�����̃I�t�Z�b�g
	constexpr float kDeadPosY = -1000.0f;
}

NormalSkelton::NormalSkelton():
	m_toPlayerDir({0.0f,0.f,0.0f}),
	m_isMove(false),
	m_moveCount(0),
	m_attack({0.0f,{m_pos.x - m_attack.attackOffSetX,0,0},false,0,0,0,0.0f,0.0,0.0f }),
	m_isPreparingAttack(false),
	m_isPrepared(false)
{
}

void NormalSkelton::Init(std::shared_ptr<Player> pPlayer, VECTOR pos, std::shared_ptr<ScoreManager> pScoreManager)
{
	m_pPlayer = pPlayer;
	m_pScoreManager = pScoreManager;
	m_pos = { 0.0f,0.0f,0.0f };
	m_pos = VAdd(m_pos, pos);
	m_attack.timer = 0.0f;
	m_attack.attackCoolTime = -1.0f;
	m_attack.pos = VGet(m_pos.x - m_attack.attackOffSetX, 0, 0);
	m_preparingTime = kDefaultPreparingTime;
	m_modelHandle = MV1LoadModel("Data/model/Skeleton_Rogue.mv1");
	m_isDead = false;
	m_isDying = false;
	m_hp = kMaxHp;
	m_power = kPower;
	m_knockbackDir = { 0.0f,0.0f,0.0f };
	m_knockbackTimer = 0.0f;
	MV1SetScale(m_modelHandle, VGet(kModelScale, kModelScale, kModelScale));
	MV1SetRotationXYZ(m_modelHandle, kLeftDir);
	AttachAnim(m_modelHandle, kIdleAnimNo);
	m_destroyScore = kDestroyScore;
	m_attack = { kAttackRadius,{m_pos.x - m_attack.attackOffSetX,0,0},false,0,0,kAttackDuration,kAttckOffSet,kAttckOffSet,kDefaultAttackCoolTime };
}

void NormalSkelton::End()
{
	MV1DeleteModel(m_modelHandle);
	m_pos = { m_pos.x,kDeadPosY,m_pos.z };
	m_attack.pos = { m_attack.pos.x,m_attack.pos.y,m_attack.pos.z };
	m_attack.active = false;
}

void NormalSkelton::Update()
{
	if (m_isDying)
	{
		OnDeath();
		return;
	}
	if (!m_isDead)
	{
		m_enemyToPlayer = VSub(m_pos, m_pPlayer->GetPos());
		// �G�l�~�[����v���C���[�܂ł̋���
		m_enemyToPlayerDistance = VSize(m_enemyToPlayer);
		//printfDx(L"m_enemyToPlayer.x:%f\n",m_enemyToPlayer.x);
		// ���G�͈͓��ɓ�������v���C���[��ǂ�
		if (m_enemyToPlayerDistance < kSerchRange && m_attack.attackCoolTime < 0)
		{
			TrackPlayer();
		}
		else
		{
			if (m_isMove)
			{
				ChangeAnim(m_modelHandle, kIdleAnimNo, true, kAnimSpeedFast);
				m_isMove = false;
			}
		}

		if (m_attack.active)
		{
			if (GetAttachAnimNo() == kAttackAnimNo)
			{
				m_attack.timer++;
			}
			if (m_attack.timer >= kAttackDuration) // �U���̎������Ԃ��I�������U���֌W�̕ϐ������Z�b�g
			{
				m_attack.active = false;
				m_attack.pos = { 0.0f,-100.0f,0.0f };
				m_attack.timer = 0.0f;
				m_attack.attackCoolTime = kDefaultAttackCoolTime; // �ēx�N�[���^�C����ݒ�
				m_preparingTime = kDefaultPreparingTime;
				m_attackCount = 0;
				m_isPreparingAttack = false;
				m_isPrepared = false;
			}
			if (GetIsAnimEnd())
			{
				ChangeAnim(m_modelHandle, kIdleAnimNo, true, kAnimSpeedFast);
			}
		}
		else
		{
			// �U�����I����Ă���Ȃ�N�[���^�C�������炷
			m_attack.attackCoolTime--;
		}
		if (GetIsAnimEnd() && !m_isMove )
		{
			ChangeAnim(m_modelHandle, kIdleAnimNo, true, kAnimSpeedFast);
		}
		MV1SetPosition(m_modelHandle, m_pos);
		UpdateAnim();
	}
	else
	{
		End();
		return;
	}
}

void NormalSkelton::OnAttack()
{
	if (m_isDying || m_isDead) return;
	m_attack.active = true;
	if (m_attackCount < 1)
	{
		// �U���A�j���[�V�����ɕύX
		ChangeAnim(m_modelHandle, kAttackAnimNo, false, kAnimSpeedSlow);
	}
	m_attackCount++;
	// �U�����̕����]��
	if (m_enemyToPlayer.x > 0)
	{
		MV1SetRotationXYZ(m_modelHandle, kLeftDir);
		m_attack.pos.x = m_pos.x - m_attack.attackOffSetX;
	}
	else
	{
		MV1SetRotationXYZ(m_modelHandle, kRightDir);
		m_attack.pos.x = m_pos.x + m_attack.attackOffSetX;
	}
	m_attack.pos.y = m_pos.y + m_attack.attackOffSetY;
	m_attack.pos.z = m_pos.z;
}

void NormalSkelton::OnDamage()
{
	if (m_isDying || m_isDead) return;
	if (m_enemyToPlayer.x > 0)
	{
		m_pos.x += 1.0f;
	}
	else
	{
		m_pos.x -= 1.0f;
	}
	ChangeAnim(m_modelHandle, kDamageAnimNo, false, kAnimSpeedFast);
	m_hp -= m_pPlayer->GetPower();
	if (m_hp <= 0 && !m_isDying)
	{
		m_hp = 0;
		m_isDying = true;
		m_attack.active = false;
		// ������ԕ��������߂�
		m_knockbackDir = VNorm(VSub(m_pos, m_pPlayer->GetPos()));
		// �^�C�}�[���Z�b�g
		m_knockbackTimer = kKnockbackDuration;
		ChangeAnim(m_modelHandle, kDeathAnimNo, false, kAnimSpeedDeath);
	}
	//printfDx(L"hp:%d\n", m_hp);
}

void NormalSkelton::OnDeath()
{
	m_attack.active = false;
	if (m_knockbackTimer > 0.0f)
	{
		m_pos = VAdd(m_pos, VScale(m_knockbackDir, kKnockBackSpeed));
		m_knockbackTimer -= 1.0f / kFramesPerSecond; // m_knockbackTimer������
	}
	if (GetIsAnimEnd())
	{
		m_isDead = true;
		m_isDying = false;
		m_pScoreManager->AddScore(m_destroyScore);
	}
	MV1SetPosition(m_modelHandle,m_pos);
	UpdateAnim();
}

void NormalSkelton::Draw() const
{
	MV1DrawModel(m_modelHandle);
#if _DEBUG
	/*DrawSphere3D(VGet(m_pos.x,m_pos.y + kDebugOffSet,m_pos.z), kColRadius, 8, 0xff0000, 0xffffff, false);
	DrawSphere3D(VGet(m_pos.x,m_pos.y + kDebugOffSet,m_pos.z), kSerchRange, 8, 0x00ff00, 0xffffff, false);
	DrawSphere3D(VGet(m_pos.x,m_pos.y + kDebugOffSet,m_pos.z), kAttackRange, 8, 0x0000ff, 0xffffff, false);
	if (attack.active)
	{
		DrawSphere3D(attack.pos, attack.radius, 8, 0x0000ff, 0xffffff, false);
	}*/
#endif
}

void NormalSkelton::TrackPlayer()
{
	
	if (m_isPreparingAttack)
	{
		m_preparingTime--;
		if (m_preparingTime < 0)
		{
			m_isPrepared = true;
			OnAttack();
		}
		return;
	}
	if (m_enemyToPlayerDistance < kAttackRange)
	{
		// �ړ��A�j���[�V�������~���A�\���Ă���A�j���[�V�����ɐ؂�ւ���
		if (!m_isPreparingAttack && !m_attack.active)
		{
			ChangeAnim(m_modelHandle, kTakeAStandAnimNo, true, kAnimSpeedFast);
			m_isPreparingAttack = true;
			m_isPrepared = false;
			m_isMove = false;
		}
	}
	else
	{
		if (!m_isMove)
		{
			m_idleCount = 0;
			// �ړ��A�j���[�V����
			ChangeAnim(m_modelHandle, kWalkAnimNo,true, kAnimSpeedFast);
			m_isMove = true;
		}
		m_moveCount++;
		// �v���C���[�Ɍ������x�N�g��
		m_toPlayerDir = VNorm(VSub(m_pPlayer->GetPos(), m_pos));
		if (m_pos.z >= kBackLimit)
		{
			m_pos.z = kBackLimit - kLimitPosOffSet;
		}
		// �v���C���[�̈ʒu�Ɍ�����
		m_pos.x += m_toPlayerDir.x * kMoveSpeed * kMoveDecRate;
		m_pos.z += m_toPlayerDir.z * kMoveSpeed * kMoveDecRate;
		if (m_enemyToPlayer.x > 0)
		{
			// ����������
			MV1SetRotationXYZ(m_modelHandle, kLeftDir);
		}
		else
		{
			// �E��������
			MV1SetRotationXYZ(m_modelHandle, kRightDir);
		}
	}
}
