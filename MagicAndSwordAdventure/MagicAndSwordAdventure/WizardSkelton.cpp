#include "WizardSkelton.h"

namespace
{
	constexpr VECTOR kLeftDir = { 0.0,90.0f * DX_PI_F / 180.0f,0.0f };
	constexpr VECTOR kRightDir = { 0.0,270.0f * DX_PI_F / 180.0f,0.0f };
	constexpr float kColRadius = 25.0f; // 敵本体の当たり判定
	constexpr float kSerchRange = 200.0f; // 索敵範囲
	constexpr float kAttackRange = 200.0f; // 攻撃範囲
	constexpr float kMoveSpeed = 2.0f; // 移動速度
	constexpr float kDebugOffSet = 45.0f;
	constexpr float kMoveAccRate = 1.1f;// 加速
	constexpr float kDefaultAttackCoolTime = 120.0f;
	// アニメーションの番号
	constexpr int kIdleAnimNo = 41;
	constexpr int kWalkAnimNo = 54;
	constexpr int kAttackAnimNo = 5;
	constexpr int kDamageAnimNo = 40;
	constexpr int kDeathAnimNo = 25;
	// 最大HP
	constexpr int kMaxHp = 60;
	//int attackCount = 0;
}

WizardSkelton::WizardSkelton():
m_toPlayerDir({0.0f,0.0f,0.0f}),
m_isAttackEnd(false),
attack({ 20.0f,{m_pos.x - attack.attackOffSetX,0,m_pos.z},false,0,0,30,30.0f,40.0,60.0f })
{
}

void WizardSkelton::Init(std::shared_ptr<Player> pPlayer, VECTOR pos, std::shared_ptr<ScoreManager> pScoreManager)
{
	m_pPlayer = pPlayer;
	m_pScoreManager = pScoreManager;
	m_pos = { 0.0f,0.0f,0.0f };
	m_pos = VAdd(m_pos, pos);
	attack.pos = VGet(m_pos.x - attack.attackOffSetX, 0, m_pos.z);
	m_modelHandle = MV1LoadModel(L"Data/model/Skeleton_Mage.mv1");
	attack.attackCoolTime = -1.0f;
	attack.timer = 60.0f;
	m_isDying = false;
	m_isDead = false;
	m_hp = kMaxHp;
	m_power = 30;
	m_knockbackDir = { 0.0f,0.0f,0.0f };
	m_knockbackSpeed = 5.0f;
	m_knockbackDuration = 0.5f;
	m_knockbackTimer = 0.0f;
	MV1SetScale(m_modelHandle, VGet(45, 45, 45));
	MV1SetRotationXYZ(m_modelHandle, kLeftDir);
	AttachAnim(m_modelHandle, kIdleAnimNo);
	m_destroyScore = 800;
	attack = { 20.0f,{m_pos.x - attack.attackOffSetX,0,m_pos.z},false,0,0,30,30.0f,40.0,60.0f };
}

void WizardSkelton::End()
{
	MV1DeleteModel(m_modelHandle);
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
		// エネミーからプレイヤーまでの距離の大きさ
		m_enemyToPlayerDistance = VSize(m_enemyToPlayer);
		//printfDx(L"m_enemyToPlayerDistance:%f\n",m_enemyToPlayerDistance);
		// 索敵範囲内に入ったら攻撃する
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
				ChangeAnim(m_modelHandle, kIdleAnimNo, false, 0.5f);
			}
			if (VSize(VSub(attack.pos, m_pos)) > kAttackRange)
			{
				attack.active = false;
				attack.pos = { m_pos.x,-1000.0f,m_pos.z };
				ChangeAnim(m_modelHandle, kIdleAnimNo, false, 0.5f);
				attack.timer = 60.0f;
				attack.attackCoolTime = kDefaultAttackCoolTime; // 再度クールタイムを設定
				m_attackCount = 0;
				m_isAttackEnd = false;
			}
		}
		else
		{
			// 攻撃が終わっているならクールタイムを減らす
			attack.attackCoolTime--;
		}
		if (GetIsAnimEnd())
		{
			ChangeAnim(m_modelHandle, kIdleAnimNo, false, 0.5f);
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
	// プレイヤーに向かうベクトル
	m_toPlayerDir = VNorm(VSub(m_pPlayer->GetPos(), m_pos));
	if (m_attackCount < 1)
	{
		ChangeAnim(m_modelHandle, 77, false, 0.4f);
	}
	m_attackCount++;
	if (m_enemyToPlayer.x > 0)
	{
		MV1SetRotationXYZ(m_modelHandle, kLeftDir);
	}
	else
	{
		MV1SetRotationXYZ(m_modelHandle, kRightDir);
	}
	// プレイヤーの位置に向かって攻撃を飛ばす
	attack.pos.x += m_toPlayerDir.x * kMoveSpeed * kMoveAccRate;
	attack.pos.z += m_toPlayerDir.z * kMoveSpeed * kMoveAccRate;
	attack.pos.y = m_pos.y + attack.attackOffSetY;
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
	ChangeAnim(m_modelHandle, kDamageAnimNo, false, 0.5f);
	m_hp -= m_pPlayer->GetPower();
	if (m_hp <= 0 && !m_isDying)
	{
		m_hp = 0;
		m_isDying = true;
		// 吹き飛ぶ方向を決める
		m_knockbackDir = VNorm(VSub(m_pos, m_pPlayer->GetPos()));
		// タイマーをセット
		m_knockbackTimer = m_knockbackDuration;
		ChangeAnim(m_modelHandle, kDeathAnimNo, false, 0.4f);
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
		m_knockbackTimer -= 1.0f / 60.0f; // m_knockbackTimerを減少
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
	if (attack.active && !m_isDying)
	{
		DrawSphere3D(attack.pos, attack.radius, 8, 0x0000ff, 0xffffff, true);
	}
#if _DEBUG
	DrawSphere3D(VGet(m_pos.x, m_pos.y + kDebugOffSet, m_pos.z), kColRadius, 8, 0xff0000, 0xffffff, false);
	DrawSphere3D(VGet(m_pos.x, m_pos.y + kDebugOffSet, m_pos.z), kSerchRange, 8, 0xffff00, 0xffffff, false);
	
#endif
}

bool WizardSkelton::IsAttackActive() const
{
	return attack.active; 
}
