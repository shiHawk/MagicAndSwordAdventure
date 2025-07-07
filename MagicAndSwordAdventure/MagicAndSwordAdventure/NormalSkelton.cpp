#include "NormalSkelton.h"
#include "DxLib.h"
#include <cmath>

namespace
{
	constexpr VECTOR kLeftDir = { 0.0,90.0f * DX_PI_F / 180.0f,0.0f };
	constexpr VECTOR kRightDir = { 0.0,270.0f * DX_PI_F / 180.0f,0.0f };
	constexpr float kColRadius = 25.0f; // 敵本体の当たり判定
	constexpr float kSerchRange = 300.0f; // 索敵範囲
	constexpr float kAttackRange = 90.0f; // 攻撃範囲
	constexpr float kMoveSpeed = 2.0f; // 移動速度
	constexpr float kDebugOffSet = 45.0f;
	// 減速
	constexpr float kMoveDecRate = 0.80f;
	constexpr float kDefaultAttackCoolTime = 60.0f; // クールタイム
	int attackCount = 0;
}

NormalSkelton::NormalSkelton():
	m_toPlayerDir({0.0f,0.f,0.0f})
{
}

void NormalSkelton::Init(std::shared_ptr<Player> pPlayer)
{
	m_pPlayer = pPlayer;
	m_pos = { 200,0,0 };
	attack.timer = 40.0f;
	attack.attackCoolTime = -1.0f;
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
	// エネミーからプレイヤーまでの距離
	m_enemyToPlayerDistance = VSize(m_enemyToPlayer);
	//printfDx(L"m_enemyToPlayer.x:%f\n",m_enemyToPlayer.x);
	if (m_enemyToPlayerDistance < kSerchRange && attack.attackCoolTime < 0)
	{
		TrackPlayer();
	}
	if (attack.active)
	{
		attack.timer--;
		if (attack.timer <= 0)
		{
			attack.active = false;
			attack.pos = { -100.0f,-100.0f,-100.0f };
			ChangeAnim(m_modelHandle, 41, false, 0.5f);
			attack.timer = 40.0f;
			attack.attackCoolTime = kDefaultAttackCoolTime; // 再度クールタイムを設定
			attackCount = 0;
		}
	}
	else
	{
		// 攻撃が終わっているならクールタイムを減らす
		attack.attackCoolTime--;
	}
	MV1SetPosition(m_modelHandle,m_pos);
	UpdateAnim();
}

void NormalSkelton::DoAttack()
{
	attack.active = true;
	if (attackCount < 1)
	{
		ChangeAnim(m_modelHandle, 5, false, 1.0f);
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
		DoAttack();
	}
	else
	{
		// プレイヤーに向かうベクトル
		m_toPlayerDir = VNorm(VSub(m_pPlayer->GetPos(), m_pos));
		m_pos.x += m_toPlayerDir.x * kMoveSpeed * kMoveDecRate;
		m_pos.z += m_toPlayerDir.z * kMoveSpeed * kMoveDecRate;
		if (m_enemyToPlayer.x > 0)
		{
			// 左を向ける
			MV1SetRotationXYZ(m_modelHandle, kLeftDir);
		}
		else
		{
			// 右を向ける
			MV1SetRotationXYZ(m_modelHandle, kRightDir);
		}
	}
}
