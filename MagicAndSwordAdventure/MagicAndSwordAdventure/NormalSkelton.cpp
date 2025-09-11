#include "NormalSkelton.h"
#include "DxLib.h"
#include <cmath>

namespace
{
	constexpr VECTOR kLeftDir = { 0.0,90.0f * DX_PI_F / 180.0f,0.0f };
	constexpr VECTOR kRightDir = { 0.0,270.0f * DX_PI_F / 180.0f,0.0f };
	constexpr float kColRadius = 25.0f; // 敵本体の当たり判定
	constexpr float kSerchRange = 500.0f; // 索敵範囲
	constexpr float kAttackRange = 90.0f; // 攻撃範囲
	constexpr float kMoveSpeed = 4.0f; // 移動速度
	constexpr float kAttckOffSet = 40.0f;
	constexpr float kBackLimit = 270.0f;
	// 減速
	constexpr float kMoveDecRate = 0.80f;
	constexpr float kDefaultAttackCoolTime = 70.0f; // クールタイム
	constexpr float kDefaultPreparingTime = 115.0f; // 追跡から攻撃に移る時間
	constexpr float kAttackDuration = 27.0f; // 攻撃の持続時間

	// アニメーションの番号
	constexpr int kIdleAnimNo = 41;
	constexpr int kWalkAnimNo = 54;
	constexpr int kTakeAStandAnimNo = 83;
	constexpr int kAttackAnimNo = 5;
	constexpr int kDamageAnimNo = 40;
	constexpr int kDeathAnimNo = 25;
	// アニメーションの速度
	constexpr float kAnimSpeedFast = 0.5f; // 短めの再生の時
	constexpr float kAnimSpeedDeath = 0.4f; // 死亡時の再生速度
	constexpr float kAnimSpeedSlow = 1.0f; // 長めの再生の時
	constexpr float kAnimSpeedakeAStand = 0.3f; // 構えているときの再生速度
	// 最大HP
	constexpr int kMaxHp = 100;
	// 敵の攻撃力
	constexpr int kPower = 40;
	// 点滅周期
	constexpr int kBlinkCycleMs = 500;
	// モデルのスケール
	constexpr int kModelScale = 45;
	// 吹き飛んでいる時間
	constexpr float kKnockBackSpeed = 5.0f;
	constexpr float kKnockbackDuration = 0.5f;
	// 撃破スコア
	constexpr int kDestroyScore = 300;
	// 攻撃の範囲
	constexpr float kAttackRadius = 30.0f;
	// 秒数変換
	constexpr float kFramesPerSecond = 60.0f;
	// 画面端のオフセット
	constexpr float kLimitPosOffSet = 0.001f;
	// 死亡後のポジションのオフセット
	constexpr float kDeadPosY = -1000.0f;
}

NormalSkelton::NormalSkelton():
	m_toPlayerDir({0.0f,0.f,0.0f}),
	m_isMove(false),
	m_moveCount(0),
	m_attack({0.0f,{m_pos.x - m_attack.attackOffSetX,0,0},false,0,0,0,0.0f,0.0,0.0f }),
	m_isPreparingAttack(false),
	m_isPrepared(false)
{
}

void NormalSkelton::Init(std::shared_ptr<Player> pPlayer, VECTOR pos, std::shared_ptr<ScoreManager> pScoreManager)
{
	m_pPlayer = pPlayer;
	m_pScoreManager = pScoreManager;
	m_pos = { 0.0f,0.0f,0.0f };
	m_pos = VAdd(m_pos, pos);
	m_attack.timer = 0.0f;
	m_attack.attackCoolTime = -1.0f;
	m_attack.pos = VGet(m_pos.x - m_attack.attackOffSetX, 0, 0);
	m_preparingTime = kDefaultPreparingTime;
	m_modelHandle = MV1LoadModel("Data/model/Skeleton_Rogue.mv1");
	m_isDead = false;
	m_isDying = false;
	m_hp = kMaxHp;
	m_power = kPower;
	m_knockbackDir = { 0.0f,0.0f,0.0f };
	m_knockbackTimer = 0.0f;
	MV1SetScale(m_modelHandle, VGet(kModelScale, kModelScale, kModelScale));
	MV1SetRotationXYZ(m_modelHandle, kLeftDir);
	AttachAnim(m_modelHandle, kIdleAnimNo);
	m_destroyScore = kDestroyScore;
	m_attack = { kAttackRadius,{m_pos.x - m_attack.attackOffSetX,0,0},false,0,0,kAttackDuration,kAttckOffSet,kAttckOffSet,kDefaultAttackCoolTime };
}

void NormalSkelton::End()
{
	MV1DeleteModel(m_modelHandle);
	m_pos = { m_pos.x,kDeadPosY,m_pos.z };
	m_attack.pos = { m_attack.pos.x,m_attack.pos.y,m_attack.pos.z };
	m_attack.active = false;
}

void NormalSkelton::Update()
{
	if (m_isDying)
	{
		OnDeath();
		return;
	}
	if (!m_isDead)
	{
		m_enemyToPlayer = VSub(m_pos, m_pPlayer->GetPos());
		// エネミーからプレイヤーまでの距離
		m_enemyToPlayerDistance = VSize(m_enemyToPlayer);
		//printfDx(L"m_enemyToPlayer.x:%f\n",m_enemyToPlayer.x);
		// 索敵範囲内に入ったらプレイヤーを追う
		if (m_enemyToPlayerDistance < kSerchRange && m_attack.attackCoolTime < 0)
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

		if (m_attack.active)
		{
			if (GetAttachAnimNo() == kAttackAnimNo)
			{
				m_attack.timer++;
			}
			if (m_attack.timer >= kAttackDuration) // 攻撃の持続時間が終わったら攻撃関係の変数をリセット
			{
				m_attack.active = false;
				m_attack.pos = { 0.0f,-100.0f,0.0f };
				m_attack.timer = 0.0f;
				m_attack.attackCoolTime = kDefaultAttackCoolTime; // 再度クールタイムを設定
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
			// 攻撃が終わっているならクールタイムを減らす
			m_attack.attackCoolTime--;
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
	m_attack.active = true;
	if (m_attackCount < 1)
	{
		// 攻撃アニメーションに変更
		ChangeAnim(m_modelHandle, kAttackAnimNo, false, kAnimSpeedSlow);
	}
	m_attackCount++;
	// 攻撃時の方向転換
	if (m_enemyToPlayer.x > 0)
	{
		MV1SetRotationXYZ(m_modelHandle, kLeftDir);
		m_attack.pos.x = m_pos.x - m_attack.attackOffSetX;
	}
	else
	{
		MV1SetRotationXYZ(m_modelHandle, kRightDir);
		m_attack.pos.x = m_pos.x + m_attack.attackOffSetX;
	}
	m_attack.pos.y = m_pos.y + m_attack.attackOffSetY;
	m_attack.pos.z = m_pos.z;
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
		m_attack.active = false;
		// 吹き飛ぶ方向を決める
		m_knockbackDir = VNorm(VSub(m_pos, m_pPlayer->GetPos()));
		// タイマーをセット
		m_knockbackTimer = kKnockbackDuration;
		ChangeAnim(m_modelHandle, kDeathAnimNo, false, kAnimSpeedDeath);
	}
	//printfDx(L"hp:%d\n", m_hp);
}

void NormalSkelton::OnDeath()
{
	m_attack.active = false;
	if (m_knockbackTimer > 0.0f)
	{
		m_pos = VAdd(m_pos, VScale(m_knockbackDir, kKnockBackSpeed));
		m_knockbackTimer -= 1.0f / kFramesPerSecond; // m_knockbackTimerを減少
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
		// 移動アニメーションを停止し、構えているアニメーションに切り替える
		if (!m_isPreparingAttack && !m_attack.active)
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
			// 移動アニメーション
			ChangeAnim(m_modelHandle, kWalkAnimNo,true, kAnimSpeedFast);
			m_isMove = true;
		}
		m_moveCount++;
		// プレイヤーに向かうベクトル
		m_toPlayerDir = VNorm(VSub(m_pPlayer->GetPos(), m_pos));
		if (m_pos.z >= kBackLimit)
		{
			m_pos.z = kBackLimit - kLimitPosOffSet;
		}
		// プレイヤーの位置に向かう
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
