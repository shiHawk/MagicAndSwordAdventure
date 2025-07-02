#include "NormalSkelton.h"
#include "DxLib.h"
#include <cmath>

namespace
{
	constexpr VECTOR kLeftDir = { 0.0,90.0f * DX_PI_F / 180.0f,0.0f };
	constexpr float kColRadius = 25.0f; // “–‚½‚è”»’è‚Ì‰~
	constexpr float kSerchRange = 300.0f; // õ“G”ÍˆÍ
	constexpr float kAttackRange = 90.0f; // UŒ‚”ÍˆÍ
	int attackCount = 0;
}

NormalSkelton::NormalSkelton()
{
}

void NormalSkelton::Init(std::shared_ptr<Player> pPlayer)
{
	m_pPlayer = pPlayer;
	m_pos = { 200,0,0 };
	m_modelHandle = MV1LoadModel(L"Data/model/Skeleton_Rogue.mv1");
	MV1SetScale(m_modelHandle, VGet(45, 45, 45));
	MV1SetRotationXYZ(m_modelHandle, kLeftDir);
	AttachAnim(m_modelHandle, 41);
}

void NormalSkelton::End()
{
	MV1DeleteModel(m_modelHandle);
}

void NormalSkelton::Update()
{
	m_enemyToPlayer = VSub(m_pos, m_pPlayer->GetPos());
	m_enemyToPlayerDistance = VSize(m_enemyToPlayer);
	//printfDx(L"m_enemyToPlayerDistance:%f\n",m_enemyToPlayerDistance);
	if (m_enemyToPlayerDistance < kSerchRange)
	{
		TrackPlayer();
	}
	if (attack.active)
	{
		attack.timer--;
		if (attack.timer <= 0)
		{
			attack.pos = { -100.0f,-100.0f,-100.0f };
			ChangeAnim(m_modelHandle, 41, false, 0.5f);
			attackCount = 0;
			attack.active = false;
		}
	}
	MV1SetPosition(m_modelHandle,m_pos);
	UpdateAnim();
}

void NormalSkelton::DoAttack()
{
	attack.active = true;
	if (attackCount < 1)
	{
		ChangeAnim(m_modelHandle, 5, false, 0.7f);
	}
	attackCount++;
	attack.timer = 40.0f;
	attack.pos.x = m_pos.x - attack.attackOffSetX;
	attack.pos.y = m_pos.y + attack.attackOffSetY;
	attack.pos.z = m_pos.z;
}

void NormalSkelton::Draw() const
{
	MV1DrawModel(m_modelHandle);
#if _DEBUG
	DrawSphere3D(VGet(m_pos.x,m_pos.y+45,m_pos.z), kColRadius, 8, 0xff0000, 0xffffff, false);
	DrawSphere3D(VGet(m_pos.x,m_pos.y+45,m_pos.z), kSerchRange, 8, 0x00ff00, 0xffffff, false);
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
		DoAttack();
	}
}
