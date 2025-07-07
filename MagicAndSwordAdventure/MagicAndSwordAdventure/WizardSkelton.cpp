#include "WizardSkelton.h"

namespace
{
	constexpr VECTOR kLeftDir = { 0.0,90.0f * DX_PI_F / 180.0f,0.0f };
	constexpr VECTOR kRightDir = { 0.0,270.0f * DX_PI_F / 180.0f,0.0f };
	constexpr float kColRadius = 25.0f; // �G�{�̂̓����蔻��
	constexpr float kSerchRange = 300.0f; // ���G�͈�
	constexpr float kMoveSpeed = 2.0f; // �ړ����x
	constexpr float kDebugOffSet = 45.0f;
	constexpr float kMoveDecRate = 1.1f;// ����
	constexpr float kDefaultAttackCoolTime = 60.0f;
	int attackCount = 0;
}

WizardSkelton::WizardSkelton():
m_toPlayerDir({0.0f,0.0f,0.0f})
{
}

void WizardSkelton::Init(std::shared_ptr<Player> pPlayer)
{
	m_pPlayer = pPlayer;
	m_pos = { 300.0f,0.0f,0.0f };
	attack.pos.x = m_pos.x - attack.attackOffSetX;
	m_modelHandle = MV1LoadModel(L"Data/model/Skeleton_Mage.mv1");
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
	if (m_enemyToPlayerDistance < kSerchRange)
	{
		DoAttack();
	}
	if (attack.active)
	{
		attack.timer--;
		if (attack.timer <= 0)
		{
			attack.active = false;
			attack.pos = { m_pos.x - attack.attackOffSetX,-100.0f,-100.0f };
			ChangeAnim(m_modelHandle, 41, false, 0.5f);
			attackCount = 0;
		}
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
		ChangeAnim(m_modelHandle, 75, false, 0.7f);
	}
	attackCount++;
	attack.timer = 40.0f;
	if (m_enemyToPlayer.x > 0)
	{
		MV1SetRotationXYZ(m_modelHandle, kLeftDir);
		attack.pos.x += m_toPlayerDir.x * kMoveSpeed * kMoveDecRate;
		attack.pos.z += m_toPlayerDir.z * kMoveSpeed * kMoveDecRate;
	}
	else
	{
		MV1SetRotationXYZ(m_modelHandle, kRightDir);
		attack.pos.x -= m_toPlayerDir.x * kMoveSpeed * kMoveDecRate;
		attack.pos.z -= m_toPlayerDir.z * kMoveSpeed * kMoveDecRate;
	}
	attack.pos.y = m_pos.y + attack.attackOffSetY;
}

void WizardSkelton::Draw() const
{
	MV1DrawModel(m_modelHandle);
#if _DEBUG
	DrawSphere3D(VGet(m_pos.x, m_pos.y + kDebugOffSet, m_pos.z), kColRadius, 8, 0xff0000, 0xffffff, false);
	DrawSphere3D(VGet(m_pos.x, m_pos.y + kDebugOffSet, m_pos.z), kSerchRange, 8, 0x00ff00, 0xffffff, false);
	if (attack.active)
	{
		DrawSphere3D(attack.pos, attack.radius, 8, 0x0000ff, 0xffffff, true);
	}
#endif
}
