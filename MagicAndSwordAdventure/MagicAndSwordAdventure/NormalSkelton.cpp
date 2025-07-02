#include "NormalSkelton.h"
#include "DxLib.h"
#include <cmath>

namespace
{
	constexpr VECTOR kLeftDir = { 0.0,90.0f * DX_PI_F / 180.0f,0.0f };
	constexpr float kColRadius = 25.0f; // “–‚½‚è”»’è‚Ì‰~
}

NormalSkelton::NormalSkelton():
m_serchRange(300.0f)
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
	m_enemyToPlayerDistance = VSize(VSub(m_pos, m_pPlayer->GetPos()));
	printfDx(L"m_enemyToPlayerDistance:%f\n",m_enemyToPlayerDistance);
	if (m_enemyToPlayerDistance < m_serchRange)
	{
	}
	MV1SetPosition(m_modelHandle,m_pos);
	UpdateAnim();
}

void NormalSkelton::DoAttack()
{
}

void NormalSkelton::Draw() const
{
	MV1DrawModel(m_modelHandle);
#if _DEBUG
	DrawSphere3D(VGet(m_pos.x,m_pos.y+45,m_pos.z), kColRadius, 8, 0xff0000, 0xffffff, false);
	DrawSphere3D(VGet(m_pos.x,m_pos.y+45,m_pos.z), m_serchRange, 8, 0x00ff00, 0xffffff, false);
#endif
}
