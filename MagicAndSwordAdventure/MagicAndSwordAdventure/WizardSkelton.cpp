#include "WizardSkelton.h"

namespace
{
	constexpr VECTOR kLeftDir = { 0.0,90.0f * DX_PI_F / 180.0f,0.0f }; // ������
	constexpr VECTOR kRightDir = { 0.0,270.0f * DX_PI_F / 180.0f,0.0f }; // �E����
	constexpr float kColRadius = 25.0f; // �G�{�̂̓����蔻��
	constexpr float kSerchRange = 500.0f; // ���G�͈�
	constexpr float kAttackRange = 400.0f; // �U���͈�
	constexpr float kMoveSpeed = 2.0f; // �ړ����x
	constexpr float kDebugOffSet = 45.0f;
	constexpr float kMoveAccRate = 1.1f;// ����
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
	constexpr int kMaxHp = 60;
	//int attackCount = 0;
}

WizardSkelton::WizardSkelton():
m_toPlayerDir({0.0f,0.0f,0.0f}),
m_isAttackEnd(false),
attack({ 20.0f,{m_pos.x - attack.attackOffSetX,0,m_pos.z},false,0,0,30,30.0f,0.0,60.0f }),
m_isCasting(false),
m_isCastFinished(false),
m_barrelHandle(-1),
m_rollAngleZ(0.0f)
{
}

void WizardSkelton::Init(std::shared_ptr<Player> pPlayer, VECTOR pos, std::shared_ptr<ScoreManager> pScoreManager)
{
	m_pPlayer = pPlayer;
	m_pScoreManager = pScoreManager;
	m_pos = { 0.0f,0.0f,0.0f };
	m_pos = VAdd(m_pos, pos);
	attack.pos = VGet(m_pos.x - attack.attackOffSetX, 0, m_pos.z);
	m_modelHandle = MV1LoadModel("Data/model/Skeleton_Mage.mv1");
	m_barrelHandle = MV1LoadModel("Data/model/barrel_large.mv1");
	attack.attackCoolTime = -1.0f;
	attack.timer = kAttackDuration;
	m_isDying = false;
	m_isDead = false;
	m_hp = kMaxHp;
	m_power = 20;
	m_knockbackDir = { 0.0f,0.0f,0.0f };
	m_knockbackSpeed = 5.0f;
	m_knockbackDuration = 0.5f;
	m_knockbackTimer = 0.0f;
	MV1SetScale(m_modelHandle, VGet(45, 45, 45));
	MV1SetRotationXYZ(m_modelHandle, kLeftDir);
	AttachAnim(m_modelHandle, kIdleAnimNo);
	m_destroyScore = 800;
	attack = { 20.0f,{m_pos.x - attack.attackOffSetX,0,m_pos.z},false,0,0,30,30.0f,40.0,60.0f };
	m_attackCount = 0;
}

void WizardSkelton::End()
{
	MV1DeleteModel(m_modelHandle);
	MV1DeleteModel(m_barrelHandle);
	attack.pos = { attack.pos.x,attack.pos.y - 1000.0f,attack.pos.z };
	attack.active = false;
	m_pos = { m_pos.x,m_pos.y - 1000.0f,m_pos.z };
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
		m_enemyToPlayer = VSub(m_pos, m_pPlayer->GetPos());
		// �G�l�~�[����v���C���[�܂ł̋����̑傫��
		m_enemyToPlayerDistance = VSize(m_enemyToPlayer);
		//printfDx(L"m_enemyToPlayerDistance:%f\n",m_enemyToPlayerDistance);
		// ���G�͈͓��ɓ�������U������
		if (m_enemyToPlayerDistance < kSerchRange && attack.attackCoolTime < 0 && !attack.active && !m_isCasting)
		{
			m_isCasting = true;
			m_isCastFinished = false;
			ChangeAnim(m_modelHandle,kSpellCastAnimNo,false, kAnimSpeedMedium);
		}
		if (m_isCasting && !m_isCastFinished && GetIsAnimEnd())
		{
			m_isCastFinished = true;
			m_isCasting = false;
			attack.active = true;
			ChangeAnim(m_modelHandle, kAttackAnimNo, false, kAnimSpeedFast);
		}
		if (attack.active)
		{
			DoAttack();
			m_rollAngleZ += 0.1f; // �e�̉�]��i�߂�
			if (m_rollAngleZ > DX_PI_F*2.0f)
			{
				m_rollAngleZ -= DX_PI_F * 2.0f; // 360�x�𒴂�����0�x�ɖ߂�
			}
			attack.timer--;
			if (VSize(VSub(attack.pos, m_pos)) > kAttackRange)
			{
				attack.active = false;
				attack.pos = { m_pos.x,-1000.0f,m_pos.z };
				ChangeAnim(m_modelHandle, kIdleAnimNo, false, kAnimSpeedFast);
				attack.timer = kAttackDuration;
				attack.attackCoolTime = kDefaultAttackCoolTime; // �ēx�N�[���^�C����ݒ�
				m_attackCount = 0;
				m_isAttackEnd = false;
			}
		}
		else
		{
			// �U�����I����Ă���Ȃ�N�[���^�C�������炷
			attack.attackCoolTime--;
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
	attack.active = true;
	// �v���C���[�Ɍ������x�N�g��
	m_toPlayerDir = VNorm(VSub(m_pPlayer->GetPos(), m_pos));
	
	m_attackCount++;
	if (m_enemyToPlayer.x > 0)
	{
		MV1SetRotationXYZ(m_modelHandle, kLeftDir);
	}
	else
	{
		MV1SetRotationXYZ(m_modelHandle, kRightDir);
	}
	// �v���C���[�̈ʒu�Ɍ������čU�����΂�
	attack.pos.x += m_toPlayerDir.x * kMoveSpeed * kMoveAccRate;
	attack.pos.z += m_toPlayerDir.z * kMoveSpeed * kMoveAccRate;
	attack.pos.y = 20.0f;

	MV1SetPosition(m_barrelHandle,attack.pos);
	MV1SetRotationXYZ(m_barrelHandle, VGet(DX_PI_F / 2.0f + DX_PI_F, 0.0f, m_rollAngleZ));
	MV1SetScale(m_barrelHandle, VGet(0.25f, 0.25f, 0.25f));
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
		m_knockbackTimer = m_knockbackDuration;
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
		m_pos = VAdd(m_pos, VScale(m_knockbackDir, m_knockbackSpeed));
		m_knockbackTimer -= 1.0f / 60.0f; // m_knockbackTimer������
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
	if (m_isCasting)
	{
		
	}
	MV1DrawModel(m_modelHandle);
	if (attack.active && !m_isDying)
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
	return attack.active; 
}
