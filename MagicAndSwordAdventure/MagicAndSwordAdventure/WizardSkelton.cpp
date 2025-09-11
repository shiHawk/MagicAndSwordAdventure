#include "WizardSkelton.h"

namespace
{
	constexpr VECTOR kLeftDir = { 0.0,90.0f * DX_PI_F / 180.0f,0.0f }; // ������
	constexpr VECTOR kRightDir = { 0.0,270.0f * DX_PI_F / 180.0f,0.0f }; // �E����
	constexpr float kColRadius = 25.0f; // �G�{�̂̓����蔻��
	constexpr float kSerchRange = 500.0f; // ���G�͈�
	constexpr float kAttackRange = 400.0f; // �U���͈�
	constexpr float kMoveSpeed = 4.0f; // �ړ����x
	constexpr float kAttackSpeed = 2.5f; // �ړ����x
	constexpr float kDebugOffSet = 45.0f;
	constexpr float kMoveAccRate = 1.1f;// ����
	constexpr float kMoveDecRate = 0.80f;// ����
	constexpr float kDefaultAttackCoolTime = 120.0f;
	constexpr float kAttackDuration = 60.0f; // �U���̎�������
	// �A�j���[�V�����̔ԍ�
	constexpr int kIdleAnimNo = 41;
	constexpr int kWalkAnimNo = 54;
	constexpr int kSpellCastAnimNo = 76;
	constexpr int kAttackAnimNo = 77;
	constexpr int kDamageAnimNo = 40;
	constexpr int kDeathAnimNo = 25;
	// �A�j���[�V�����̑��x
	constexpr float kAnimSpeedFast = 0.5f; // �Z�߂̍Đ��̎�
	constexpr float kAnimSpeedMedium = 0.7f; // �����x�̃e���|
	constexpr float kAnimSpeedDeath = 0.4f; // ���S���̍Đ����x
	// �ő�HP
	constexpr int kMaxHp = 80;
	constexpr int kMaxHomingTime = 60; // �ǔ����鎞��
	// �U����
	constexpr int kPower = 20;
	// �������ł��鎞��
	constexpr float kKnockBackSpeed = 5.0f;
	constexpr float kKnockbackDuration = 0.5f;
	// �U���͈̔�
	constexpr float kAttackRadius = 20.0f;
	constexpr float kAttckOffSetX = 30.0f;
	constexpr float kAttckOffSetY = 40.0f;
	// ���f���̃X�P�[���T�C�Y
	constexpr float kScaleSize = 45.0f;
	constexpr float kBarrelScaleSize = 0.25f;
	// ���j�X�R�A
	constexpr int kDestroyScore = 300;
	// ���S��̃|�W�V�����̃I�t�Z�b�g
	constexpr float kDeadPosY = -1000.0f;
	// �b���ϊ�
	constexpr float kFramesPerSecond = 60.0f;
	//int attackCount = 0;
}

WizardSkelton::WizardSkelton():
m_toPlayerDir({0.0f,0.0f,0.0f}),
m_isAttackEnd(false),
m_attack({ 0.0f,{m_pos.x - m_attack.attackOffSetX,0,m_pos.z},false,0,0,0,0.0f,0.0f,0.0f }),
m_isCasting(false),
m_isCastFinished(false),
m_barrelHandle(-1),
m_rollAngleZ(0.0f),
m_attackDir({0.0f,0.0f,0.0f}),
m_homingTimer(0),
m_isTrackFlag(false),
m_attackCount(0)
{
}

void WizardSkelton::Init(std::shared_ptr<Player> pPlayer, VECTOR pos, std::shared_ptr<ScoreManager> pScoreManager)
{
	m_pPlayer = pPlayer;
	m_pScoreManager = pScoreManager;
	m_pos = { 0.0f,0.0f,0.0f };
	m_pos = VAdd(m_pos, pos);
	m_attack.pos = VGet(m_pos.x - m_attack.attackOffSetX, 0, m_pos.z);
	m_modelHandle = MV1LoadModel("Data/model/Skeleton_Mage.mv1");
	m_barrelHandle = MV1LoadModel("Data/model/barrel_large.mv1");
	m_attack.attackCoolTime = -1.0f;
	m_attack.timer = kAttackDuration;
	m_isDying = false;
	m_isDead = false;
	m_hp = kMaxHp;
	m_power = kPower;
	m_knockbackDir = { 0.0f,0.0f,0.0f };
	m_knockbackTimer = 0.0f;
	MV1SetScale(m_modelHandle, VGet(kScaleSize, kScaleSize, kScaleSize));
	MV1SetRotationXYZ(m_modelHandle, kLeftDir);
	AttachAnim(m_modelHandle, kIdleAnimNo);
	m_destroyScore = kDestroyScore;
	m_attack = { kAttackRadius,{m_pos.x - m_attack.attackOffSetX,0,m_pos.z},false,0,0,kAttackDuration,kAttckOffSetX,kAttckOffSetY,kDefaultAttackCoolTime };
	m_attackCount = 0;
	m_isTrackFlag = false;
}

void WizardSkelton::End()
{
	MV1DeleteModel(m_modelHandle);
	MV1DeleteModel(m_barrelHandle);
	m_attack.pos = { m_attack.pos.x,m_attack.pos.y - 1000.0f,m_attack.pos.z };
	m_attack.active = false;
	m_pos = { m_pos.x,m_pos.y + kDeadPosY,m_pos.z };
	m_isTrackFlag = false;
}

void WizardSkelton::Update()
{
	if(m_isDying)
	{
		OnDeath();
		return;
	}
	if (!m_isDead)
	{
		m_enemyToPlayer = VSub(m_pPlayer->GetPos(),m_pos);
		// �G�l�~�[����v���C���[�܂ł̋����̑傫��
		m_enemyToPlayerDistance = VSize(m_enemyToPlayer);
		//printfDx(L"m_enemyToPlayerDistance:%f\n",m_enemyToPlayerDistance);
		
		// ���G�͈͓��ɓ�������U������
		if (m_enemyToPlayerDistance < kSerchRange && m_attack.attackCoolTime < 0 && !m_attack.active && !m_isCasting)
		{
			m_isCasting = true;
			m_isCastFinished = false;
			ChangeAnim(m_modelHandle,kSpellCastAnimNo,false, kAnimSpeedMedium);
		}
		if (m_isCasting && !m_isCastFinished && GetIsAnimEnd()) // �\�����삪�I�������U���Ɉڂ�
		{
			m_isCastFinished = true;
			m_isCasting = false;
			m_attack.active = true;
			ChangeAnim(m_modelHandle, kAttackAnimNo, false, kAnimSpeedFast);
		}
		if (m_attack.active)
		{
			DoAttack();
			m_rollAngleZ += 0.1f; // �e�̉�]��i�߂�
			if (m_rollAngleZ > DX_PI_F*2.0f)
			{
				m_rollAngleZ -= DX_PI_F * 2.0f; // 360�x�𒴂�����0�x�ɖ߂�
			}
			m_attack.timer--;
			if (VSize(VSub(m_attack.pos, m_pos)) > kAttackRange) // �U���͈̔͂𒴂�����U�������Z�b�g
			{
				m_attack.active = false;
				m_attack.pos = { m_pos.x,-1000.0f,m_pos.z };
				ChangeAnim(m_modelHandle, kIdleAnimNo, false, kAnimSpeedFast);
				m_attack.timer = kAttackDuration;
				m_attack.attackCoolTime = kDefaultAttackCoolTime; // �ēx�N�[���^�C����ݒ�
				m_attackCount = 0;
				m_isAttackEnd = false;
			}
		}
		else
		{
			// �U�����I����Ă���Ȃ�N�[���^�C�������炷
			m_attack.attackCoolTime--;
		}
		if (GetIsAnimEnd())
		{
			ChangeAnim(m_modelHandle, kIdleAnimNo, false, kAnimSpeedFast);
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

void WizardSkelton::DoAttack()
{
	if (m_isDying || m_isDead) return;
	
	// �v���C���[�Ɍ������x�N�g��
	m_toPlayerDir = VNorm(VSub(m_pPlayer->GetPos(), m_pos));
	if (m_attackCount == 0) // m_attackCount��0����1�ɂȂ�Ƃ����U���J�n�t���[���Ƃ���
	{
		m_attack.active = true;
		m_attackDir = VNorm(VSub(m_pPlayer->GetPos(), m_pos)); // �U���̏����������v�Z
		m_homingTimer = kMaxHomingTime; // �ǔ��^�C�}�[���Z�b�g
	}
	m_attackCount++;
	if (m_homingTimer > 0) // �ǔ��^�C�}�[���c���Ă���Ԃ̓x�N�g�����X�V����
	{
		m_attackDir = VNorm(VSub(m_pPlayer->GetPos(), m_pos)); // �v���C���[�Ɍ������x�N�g�����X�V
		m_homingTimer--; // �^�C�}�[�����炷
	}
	if (m_enemyToPlayer.x > 0)
	{
		MV1SetRotationXYZ(m_modelHandle, kRightDir);
	}
	else
	{
		MV1SetRotationXYZ(m_modelHandle, kLeftDir);
	}
	// �v���C���[�̈ʒu�Ɍ������čU�����΂�
	m_attack.pos.x += m_attackDir.x * kAttackSpeed * kMoveAccRate;
	m_attack.pos.z += m_attackDir.z * kAttackSpeed * kMoveAccRate;
	m_attack.pos.y = 20.0f;

	MV1SetPosition(m_barrelHandle,m_attack.pos);
	MV1SetRotationXYZ(m_barrelHandle, VGet(DX_PI_F / 2.0f + DX_PI_F, 0.0f, m_rollAngleZ));
	MV1SetScale(m_barrelHandle, VGet(kBarrelScaleSize, kBarrelScaleSize, kBarrelScaleSize));
}

void WizardSkelton::OnDamage()
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
		// ������ԕ��������߂�
		m_knockbackDir = VNorm(VSub(m_pos, m_pPlayer->GetPos()));
		// �^�C�}�[���Z�b�g
		m_knockbackTimer = kKnockbackDuration;
		ChangeAnim(m_modelHandle, kDeathAnimNo, false, kAnimSpeedDeath);
	}
	if (m_hp <= 0)
	{
		m_hp = 0;
	}
	//printfDx(L"hp:%d\n", m_hp);
}

void WizardSkelton::OnDeath()
{
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
	MV1SetPosition(m_modelHandle, m_pos);
	UpdateAnim();
}

void WizardSkelton::Draw() const
{
	MV1DrawModel(m_modelHandle);
	if (m_attack.active && !m_isDying)
	{
		//DrawSphere3D(attack.pos, attack.radius, 8, 0x0000ff, 0xffffff, true);
		MV1DrawModel(m_barrelHandle);
	}
#if _DEBUG
	/*DrawSphere3D(VGet(m_pos.x, m_pos.y + kDebugOffSet, m_pos.z), kColRadius, 8, 0xff0000, 0xffffff, false);
	DrawSphere3D(VGet(m_pos.x, m_pos.y + kDebugOffSet, m_pos.z), kSerchRange, 8, 0xffff00, 0xffffff, false);*/
#endif
}

bool WizardSkelton::IsAttackActive() const
{
	return m_attack.active; 
}

void WizardSkelton::TrackPlayer()
{
	m_toPlayerDir = VNorm(VSub(m_pPlayer->GetPos(), m_pos));
	m_pos.x += m_toPlayerDir.x * kMoveSpeed * kMoveDecRate;
	m_pos.z += m_toPlayerDir.z * kMoveSpeed * kMoveDecRate;
	if (m_enemyToPlayer.x > 0)
	{
		MV1SetRotationXYZ(m_modelHandle, kRightDir);
	}
	else
	{
		MV1SetRotationXYZ(m_modelHandle, kLeftDir);
	}
	// �ړ��A�j���[�V����
	ChangeAnim(m_modelHandle, kWalkAnimNo, true, kAnimSpeedFast);
}
