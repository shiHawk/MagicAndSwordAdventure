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
	constexpr float kDebugOffSet = 45.0f;
	constexpr float kBackLimit = 270.0f;
	// ����
	constexpr float kMoveDecRate = 0.80f;
	constexpr float kDefaultAttackCoolTime = 80.0f; // �N�[���^�C��
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
	constexpr int kMaxHp = 160;
	// �_�Ŏ���
	constexpr int kBlinkCycleMs = 500;
	// ���f���̃X�P�[��
	constexpr int kModelScale = 45;
}

NormalSkelton::NormalSkelton():
	m_toPlayerDir({0.0f,0.f,0.0f}),
	m_isMove(false),
	m_moveCount(0),
	attack({ 30.0f,{m_pos.x - attack.attackOffSetX,0,0},false,0,0,30,60.0f,40.0,60.0f }),
	m_isPreparingAttack(false),
	m_isPrepared(false),
	m_weponModelHandel(-1)
{
}

void NormalSkelton::Init(std::shared_ptr<Player> pPlayer, VECTOR pos, std::shared_ptr<ScoreManager> pScoreManager)
{
	m_pPlayer = pPlayer;
	m_pScoreManager = pScoreManager;
	m_pos = { 0,0,0 };
	m_pos = VAdd(m_pos, pos);
	attack.timer = 40.0f;
	attack.attackCoolTime = -1.0f;
	attack.pos = VGet(m_pos.x - attack.attackOffSetX, 0, 0);
	m_preparingTime = kDefaultPreparingTime;
	m_modelHandle = MV1LoadModel("Data/model/Skeleton_Rogue.mv1");
	m_weponModelHandel = MV1LoadModel("Data/model/Skeleton_Blade.mv1");
	m_isDead = false;
	m_isDying = false;
	m_hp = kMaxHp;
	m_power = 40;
	m_knockbackDir = { 0.0f,0.0f,0.0f };
	m_knockbackSpeed = 5.0f;
	m_knockbackDuration = 0.5f;
	m_knockbackTimer = 0.0f;
	MV1SetScale(m_modelHandle, VGet(kModelScale, kModelScale, kModelScale));
	MV1SetScale(m_weponModelHandel, VGet(0.1f, 0.1f, 0.1f));
	MV1SetRotationXYZ(m_modelHandle, kLeftDir);
	AttachAnim(m_modelHandle, kIdleAnimNo);
	m_destroyScore = 300;
	attack = { 30.0f,{m_pos.x - attack.attackOffSetX,0,0},false,0,0,30,40.0f,40.0,60.0f };
}

void NormalSkelton::End()
{
	MV1DeleteModel(m_modelHandle);
	MV1DeleteModel(m_weponModelHandel);
	m_pos = { m_pos.x,m_pos.y - 1000.0f,m_pos.z };
	attack.pos = { attack.pos.x,attack.pos.y - 1000.0f,attack.pos.z };
	attack.active = false;
}

void NormalSkelton::Update()
{
	//SetModelFramePos(m_modelHandle,14, m_weponModelHandel);
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
		if (m_enemyToPlayerDistance < kSerchRange && attack.attackCoolTime < 0)
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

		if (attack.active)
		{
			if (GetAttachAnimNo() == kAttackAnimNo)
			{
				attack.timer++;
			}
			if (attack.timer >= kAttackDuration)
			{
				attack.active = false;
				attack.pos = { 0.0f,-100.0f,0.0f };
				attack.timer = 0.0f;
				attack.attackCoolTime = kDefaultAttackCoolTime; // �ēx�N�[���^�C����ݒ�
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
			attack.attackCoolTime--;
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
	attack.active = true;
	if (m_attackCount < 1)
	{
		// �U���A�j���[�V�����ɕύX
		ChangeAnim(m_modelHandle, kAttackAnimNo, false, kAnimSpeedSlow);
	}
	m_attackCount++;
	if (m_enemyToPlayer.x > 0)
	{
		MV1SetRotationXYZ(m_modelHandle, kLeftDir);
		attack.pos.x = m_pos.x - attack.attackOffSetX;
	}
	else
	{
		MV1SetRotationXYZ(m_modelHandle, kRightDir);
		attack.pos.x = m_pos.x + attack.attackOffSetX;
	}
	attack.pos.y = m_pos.y + attack.attackOffSetY;
	attack.pos.z = m_pos.z;
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
		attack.active = false;
		// ������ԕ��������߂�
		m_knockbackDir = VNorm(VSub(m_pos, m_pPlayer->GetPos()));
		// �^�C�}�[���Z�b�g
		m_knockbackTimer = m_knockbackDuration;
		ChangeAnim(m_modelHandle, kDeathAnimNo, false, kAnimSpeedDeath);
	}
	//printfDx(L"hp:%d\n", m_hp);
}

void NormalSkelton::OnDeath()
{
	attack.active = false;
	if (m_knockbackTimer > 0.0f)
	{
		m_pos = VAdd(m_pos, VScale(m_knockbackDir, m_knockbackSpeed));
		m_knockbackTimer -= 1.0f / 60.0f; // m_knockbackTimer������
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
	//MV1DrawModel(m_weponModelHandel);
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
		if (!m_isPreparingAttack && !attack.active)
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
			m_pos.z = kBackLimit - 0.001f;
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
