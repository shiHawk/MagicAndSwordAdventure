#include "WizardSkelton.h"

namespace
{
	constexpr VECTOR kLeftDir = { 0.0,90.0f * DX_PI_F / 180.0f,0.0f };
	constexpr VECTOR kRightDir = { 0.0,270.0f * DX_PI_F / 180.0f,0.0f };
	constexpr float kColRadius = 25.0f; // �G�{�̂̓����蔻��
	constexpr float kSerchRange = 300.0f; // ���G�͈�
	constexpr float kAttackRange = 300.0f; // �U���͈�
	constexpr float kMoveSpeed = 2.0f; // �ړ����x
	constexpr float kDebugOffSet = 45.0f;
	constexpr float kMoveAccRate = 1.1f;// ����
	constexpr float kDefaultAttackCoolTime = 60.0f;
	// �A�j���[�V�����̔ԍ�
	constexpr int kIdleAnimNo = 41;
	constexpr int kWalkAnimNo = 54;
	constexpr int kAttackAnimNo = 5;
	int attackCount = 0;
}

WizardSkelton::WizardSkelton():
m_toPlayerDir({0.0f,0.0f,0.0f}),
m_isAttackEnd(false)
{
}

void WizardSkelton::Init(std::shared_ptr<Player> pPlayer)
{
	m_pPlayer = pPlayer;
	m_pos = { 300.0f,0.0f,0.0f };
	attack.pos = VGet(m_pos.x - attack.attackOffSetX, 0, 0);
	m_modelHandle = MV1LoadModel(L"Data/model/Skeleton_Mage.mv1");
	attack.attackCoolTime = -1.0f;
	attack.timer = 60.0f;
	m_hp = 60;
	m_power = 30;
	MV1SetScale(m_modelHandle, VGet(45, 45, 45));
	MV1SetRotationXYZ(m_modelHandle, kLeftDir);
	AttachAnim(m_modelHandle, 41);
}

void WizardSkelton::End()
{
	MV1DeleteModel(m_modelHandle);
}

void WizardSkelton::Update()
{
	m_enemyToPlayer = VSub(m_pos, m_pPlayer->GetPos());
	// �G�l�~�[����v���C���[�܂ł̋����̑傫��
	m_enemyToPlayerDistance = VSize(m_enemyToPlayer);
	//printfDx(L"m_enemyToPlayerDistance:%f\n",m_enemyToPlayerDistance);
	if (m_enemyToPlayerDistance < kSerchRange && attack.attackCoolTime < 0)
	{
		attack.active = true;
	}
	if (attack.active)
	{
		DoAttack();
		attack.timer--;
		if (attack.timer < 0.0f && !m_isAttackEnd)
		{
			m_isAttackEnd = true;
			ChangeAnim(m_modelHandle, 41, false, 0.5f);
		}
		if(VSize(VSub(attack.pos,m_pos)) > kAttackRange)
		{
			attack.active = false;
			attack.pos = { m_pos.x,-100.0f,m_pos.z };
			ChangeAnim(m_modelHandle, 41, false, 0.5f);
			attack.timer = 60.0f;
			attack.attackCoolTime = kDefaultAttackCoolTime; // �ēx�N�[���^�C����ݒ�
			attackCount = 0;
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
		ChangeAnim(m_modelHandle, kIdleAnimNo, false, 0.5f);
	}
	MV1SetPosition(m_modelHandle, m_pos);
	UpdateAnim();
}

void WizardSkelton::DoAttack()
{
	attack.active = true;
	// �v���C���[�Ɍ������x�N�g��
	m_toPlayerDir = VNorm(VSub(m_pPlayer->GetPos(), m_pos));
	if (attackCount < 1)
	{
		ChangeAnim(m_modelHandle, 77, false, 0.4f);
	}
	attackCount++;
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
	attack.pos.y = m_pos.y + attack.attackOffSetY;
}

void WizardSkelton::OnDamage()
{
	ChangeAnim(m_modelHandle, 40, false, 0.5f);
	m_hp -= m_pPlayer->GetPower();
	if (m_hp <= 0)
	{
		m_hp = 0;
	}
	printfDx(L"hp:%d\n", m_hp);
}

void WizardSkelton::Draw() const
{
	MV1DrawModel(m_modelHandle);
#if _DEBUG
	DrawSphere3D(VGet(m_pos.x, m_pos.y + kDebugOffSet, m_pos.z), kColRadius, 8, 0xff0000, 0xffffff, false);
	DrawSphere3D(VGet(m_pos.x, m_pos.y + kDebugOffSet, m_pos.z), kSerchRange, 8, 0xffff00, 0xffffff, false);
	if (attack.active)
	{
		DrawSphere3D(attack.pos, attack.radius, 8, 0x0000ff, 0xffffff, true);
	}
#endif
}
