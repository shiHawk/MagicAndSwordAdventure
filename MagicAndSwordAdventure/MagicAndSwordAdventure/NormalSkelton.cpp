#include "NormalSkelton.h"
#include "DxLib.h"
#include <cmath>

namespace
{
	constexpr VECTOR kLeftDir = { 0.0,90.0f * DX_PI_F / 180.0f,0.0f };
	constexpr VECTOR kRightDir = { 0.0,270.0f * DX_PI_F / 180.0f,0.0f };
	constexpr float kColRadius = 25.0f; // �G�{�̂̓����蔻��
	constexpr float kSerchRange = 200.0f; // ���G�͈�
	constexpr float kAttackRange = 90.0f; // �U���͈�
	constexpr float kMoveSpeed = 2.0f; // �ړ����x
	constexpr float kDebugOffSet = 45.0f;
	// ����
	constexpr float kMoveDecRate = 0.80f;
	constexpr float kDefaultAttackCoolTime = 200.0f; // �N�[���^�C��
	// �A�j���[�V�����̔ԍ�
	constexpr int kIdleAnimNo = 41;
	constexpr int kWalkAnimNo = 54;
	constexpr int kAttackAnimNo = 5;
	// �ő�HP
	constexpr int kMaxHp = 100;
	int attackCount = 0;
	int idleCount = 0;
}

NormalSkelton::NormalSkelton():
	m_toPlayerDir({0.0f,0.f,0.0f}),
	m_attackWaitingTime(0.0f),
	m_isMove(false),
	m_moveCount(0)
{
}

void NormalSkelton::Init(std::shared_ptr<Player> pPlayer, VECTOR pos)
{
	m_pPlayer = pPlayer;
	m_pos = { 200,0,0 };
	m_pos = VAdd(m_pos, pos);
	attack.timer = 40.0f;
	attack.attackCoolTime = -1.0f;
	attack.pos = VGet(m_pos.x - attack.attackOffSetX, 0, 0);
	m_attackWaitingTime = 60.0f;
	m_modelHandle = MV1LoadModel(L"Data/model/Skeleton_Rogue.mv1");
	m_isDead = false;
	m_isDying = false;
	m_hp = kMaxHp;
	m_power = 20;
	MV1SetScale(m_modelHandle, VGet(45, 45, 45));
	MV1SetRotationXYZ(m_modelHandle, kLeftDir);
	AttachAnim(m_modelHandle, kIdleAnimNo);
}

void NormalSkelton::End()
{
	MV1DeleteModel(m_modelHandle);
	attack.active = false;
	m_pos = { m_pos.x,m_pos.y - 1000.0f,m_pos.z };
}

void NormalSkelton::Update()
{
	if (m_isDying)
	{
		OnDeath();
	}
	if (!m_isDead)
	{
		m_enemyToPlayer = VSub(m_pos, m_pPlayer->GetPos());
		// �G�l�~�[����v���C���[�܂ł̋���
		m_enemyToPlayerDistance = VSize(m_enemyToPlayer);
		//printfDx(L"m_enemyToPlayer.x:%f\n",m_enemyToPlayer.x);

		if (m_enemyToPlayerDistance < kSerchRange && attack.attackCoolTime < 0)
		{
			TrackPlayer();
		}
		else
		{
			if (m_isMove)
			{
				ChangeAnim(m_modelHandle, kIdleAnimNo, true, 0.5f);
				m_isMove = false;
			}
		}

		if (attack.active)
		{
			attack.timer--;
			if (attack.timer <= 0)
			{
				attack.active = false;
				attack.pos = { 0.0f,-100.0f,0.0f };
				ChangeAnim(m_modelHandle, kIdleAnimNo, true, 0.5f);
				attack.timer = 40.0f;
				attack.attackCoolTime = kDefaultAttackCoolTime; // �ēx�N�[���^�C����ݒ�
				m_attackWaitingTime = 20.0f;
				attackCount = 0;
			}
		}
		else
		{
			// �U�����I����Ă���Ȃ�N�[���^�C�������炷
			attack.attackCoolTime--;
		}
		if (GetIsAnimEnd() && !m_isMove)
		{
			ChangeAnim(m_modelHandle, kIdleAnimNo, true, 0.5f);
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
	if (attackCount < 1)
	{
		// �U���A�j���[�V�����ɕύX
		ChangeAnim(m_modelHandle, kAttackAnimNo, false, 1.0f);
	}
	attackCount++;
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
	ChangeAnim(m_modelHandle, 40, false, 0.5f);
	m_hp -= m_pPlayer->GetPower();
	if (m_hp <= 0 && !m_isDying)
	{
		m_hp = 0;
		m_isDying = true;
		ChangeAnim(m_modelHandle, 25, false, 0.4f);
	}
	printfDx(L"hp:%d\n", m_hp);
}

void NormalSkelton::OnDeath()
{
	if (GetIsAnimEnd())
	{
		m_isDead = true;
	}
}

void NormalSkelton::Draw() const
{
	MV1DrawModel(m_modelHandle);
#if _DEBUG
	DrawSphere3D(VGet(m_pos.x,m_pos.y + kDebugOffSet,m_pos.z), kColRadius, 8, 0xff0000, 0xffffff, false);
	DrawSphere3D(VGet(m_pos.x,m_pos.y + kDebugOffSet,m_pos.z), kSerchRange, 8, 0x00ff00, 0xffffff, false);
	DrawSphere3D(VGet(m_pos.x,m_pos.y + kDebugOffSet,m_pos.z), kAttackRange, 8, 0x0000ff, 0xffffff, false);
	if (attack.active)
	{
		DrawSphere3D(attack.pos, attack.radius, 8, 0x0000ff, 0xffffff, false);
	}
#endif
}

void NormalSkelton::TrackPlayer()
{
	if (m_enemyToPlayerDistance < kAttackRange)
	{
		m_isMove = false;
		// �U�����n�߂�܂ł̑ҋ@���Ԃ����炷
		m_attackWaitingTime--;
		if (m_attackWaitingTime < 0)
		{
			OnAttack();
		}
	}
	else
	{
		if (!m_isMove)
		{
			idleCount = 0;
			// �ړ��A�j���[�V����
			ChangeAnim(m_modelHandle, kWalkAnimNo,true,0.5f);
			m_isMove = true;
		}
		m_moveCount++;
		// �v���C���[�Ɍ������x�N�g��
		m_toPlayerDir = VNorm(VSub(m_pPlayer->GetPos(), m_pos));
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
