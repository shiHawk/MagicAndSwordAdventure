#include "Player.h"
#include "DxLib.h"
#include "Pad.h"

namespace
{
	constexpr float kMoveSpeed = 5.0f;
	constexpr float kDashSpeed = 10.0f;
	constexpr float kJumpPower = 9.0f;
	constexpr float kJumpGravity = -0.4f;
	constexpr float kZLimit = 240.0f;

	// 当たり判定の範囲
	constexpr float kColRadius = 40.0f;
	// 最大HP
	constexpr int kMaxHp = 3000;
	// 減速
	constexpr float kMoveDecRate = 0.80f;

	// カメラ移動用のしきい値
	constexpr float kMoveCameraThreshold = 60.0f;

	// 連続攻撃の受付時間
	constexpr float kMaxComboDuration = 90.0f;

	constexpr VECTOR kRightDir = { 0.0,270.0f * DX_PI_F / 180.0f,0.0f };
	constexpr VECTOR kLeftDir = { 0.0,90.0f * DX_PI_F / 180.0f,0.0f };

	// アニメーションの番号
	constexpr int kIdleAnimNo = 1;
	constexpr int kWalkAnimNo = 3;
	constexpr int kAttack1AnimNo = 31;
	constexpr int kAttack2AnimNo = 40;
	constexpr int kAttack3AnimNo = 42;
	constexpr int kDamageAnimNo = 24;
	constexpr int kDeathAnimNo = 26;

	bool isStartGravity = false;
	// 振り返るだけで移動したと判定させないために
	bool isMove = false;
	// 連続で移動アニメーションを呼ばないようにする
	int moveCount = 0;
	int idleCount = 0;
}


Player::Player():
m_damageFrame(0),
m_hp(kMaxHp),
m_rotMtx(MGetIdent()),
m_isJump(false),
m_isDirRight(true),
m_isPrevButton(false),
m_isNowButton(false),
m_jumpCount(0),
m_isAttackDirRight(true),
m_screenPos({0.0f,0.0f,0.0f}),
m_isMovingFlag(false),
m_isNowDirRight(true),
m_isPrevDirRight(true),
m_prevPos({0,0,0}),
m_distanceAfterMoving(0.0f),
attack({ 30,{-500,0,0},false,0.0f,0,30.0f,60.0f,40.0f })
{
}

Player::~Player()
{
}

void Player::Init(std::shared_ptr<Animation> pAnimation)
{
	m_pos = { -400.0f, 0.0f, 0.0f };
	m_vec = { 0, 0, 0 };
	m_pAnimation = pAnimation;
	m_modelHandle = MV1LoadModel(L"Data/model/Barbarian.mv1");
	MV1SetScale(m_modelHandle, VGet(50, 50, 50));
	MV1SetRotationXYZ(m_modelHandle, kRightDir);
	m_pAnimation->AttachAnim(m_modelHandle, 1);
	m_prevPos = m_pos;
	m_power = 20;
	m_hp = kMaxHp;
	m_isDead = false;
	attack = { 30, { -500,0,0 }, false, 0.0f, 0, 30.0f, 60.0f, 40.0f };
}

void Player::End()
{
	MV1DeleteModel(m_modelHandle);
}

void Player::Update()
{
	if (m_isDying)
	{
		OnDeath();
		return;
	}
	if (!m_isDead)
	{
		if (attack.comboDuration > 0.0f)
		{
			attack.comboDuration--;
			if (attack.comboDuration <= 0.0f)
			{
				attack.count = 0;
			}
		}
		if (m_vec.y > 0)
		{
			isStartGravity = true;
		}
		// Bボタンを押したとき
		m_isNowButton = Pad::isPress(PAD_INPUT_2);
		// Bボタンが押されっぱなしでない
		if (m_isNowButton && !m_isPrevButton && !m_isJump && !isMove)
		{
			DoAttack();
		}
		if (attack.active)
		{
			attack.timer--;
			if (attack.timer <= 0 || m_pAnimation->GetIsAnimEnd())
			{
				m_pAnimation->ChangeAnim(m_modelHandle, kIdleAnimNo, true, 0.5f);
				attack.pos = { 0.0f,-1000.0f,0.0f };
				attack.active = false;
			}
		}
		m_isPrevButton = m_isNowButton; // ボタンの更新
		if (Pad::isTrigger(PAD_INPUT_1) && !attack.active)// Aボタンを押したときジャンプ
		{
			m_vec.y = kJumpPower;
			m_jumpCount++;
		}
		// ジャンプは2回まで
		if (m_jumpCount > 1)
		{
			m_isJump = true;
		}
		DoMove();

		//RBボタンを押したとき
		if (Pad::isTrigger(PAD_INPUT_6))
		{
			DoEvade();
		}
		if (evadeData.active)
		{
			evadeData.timer--;
			// 回避時間が終わったら
			if (evadeData.timer <= 0)
			{
				m_pAnimation->ChangeAnim(m_modelHandle, kIdleAnimNo, true, 0.5f);
				evadeData.active = false;
				evadeData.evadeCount = 0;
			}
		}

		if (isMove)
		{
			idleCount = 0;
			if (moveCount < 1)
			{
				m_pAnimation->ChangeAnim(m_modelHandle, kWalkAnimNo, true, 0.5f);
			}
			moveCount++;
		}

		if (m_pos.y < 0.0f)
		{
			m_pos.y = 0.0f;
			m_vec.y = 0;
			m_isJump = false;
			m_jumpCount = 0;
		}
		m_pAnimation->UpdateAnim();
	}
	else
	{
		End();
		return;
	}
	
}

void Player::Draw() const
{
	MV1DrawModel(m_modelHandle);
#if _DEBUG
	//DrawCapsule3D(VGet(m_pos.x, m_pos.y + playerHeadOffSet, m_pos.z), VGet(m_pos.x, m_pos.y+ playerFootOffSet, m_pos.z), 30, 8, 0x00ff00, 0xffffff, false);
	if (attack.active && !m_vec.y > 0)
	{
		DrawSphere3D(attack.pos, attack.radius, 8, 0xff0000, 0xffffff, false);
	}
#endif
}

VECTOR Player::GetScreenPos()
{
	// スクリーン座標に変換
	m_screenPos = ConvWorldPosToScreenPos(m_pos);
	return m_screenPos;
}

float Player::GetColRadius() const
{
	return kColRadius;
}

void Player::OnDamage(int enemyPower)
{
	m_hp -= enemyPower;
	if (m_hp <= 0 )
	{
		m_hp = 0;
		m_pAnimation->ChangeAnim(m_modelHandle, kDeathAnimNo, false, 0.5f);
		printfDx(L"死亡フラグが立ちました\n");
		m_isDying = true;
	}
	else
	{
		m_pAnimation->ChangeAnim(m_modelHandle, kDamageAnimNo, false, 0.5f);
	}
	
	printfDx(L"hp:%d m_isDying:%d\n", m_hp, m_isDying);
}

void Player::OnDeath()
{
	m_pAnimation->UpdateAnim();
	MV1SetPosition(m_modelHandle, m_pos);
	if (m_pAnimation->GetIsAnimEnd())
	{
		m_isDead = true;
		m_isDying = false;
	}
}

void Player::DoAttack()
{
	if (attack.active) return; // 攻撃中なら次の攻撃を受け付けない
	attack.count++;
	// 三回攻撃したらまた一回目に戻る
	if (attack.count > 3)
	{
		attack.count = 1;
	}
	attack.comboDuration = kMaxComboDuration;
	attack.active = true;
	if (m_vec.y > 0)
	{
		// ジャンプ攻撃
		m_pAnimation->ChangeAnim(m_modelHandle, 39, true, 0.7f);
	}
	else
	{
		switch (attack.count) {
		case 1:
			m_power = 20;
			m_pAnimation->ChangeAnim(m_modelHandle, kAttack1AnimNo, false, 0.5f);
			break;
		case 2:
			m_power = 30;
			m_pAnimation->ChangeAnim(m_modelHandle, kAttack2AnimNo, false, 0.7f);
			break;
		case 3:
			m_power = 40;
			m_pAnimation->ChangeAnim(m_modelHandle, kAttack3AnimNo, false, 1.0f);
			break;
		}
	}
	//printfDx(L"attack.count:%d\n", attack.count);
	if (m_isAttackDirRight)
	{
		// 攻撃の時に少し前進する
		m_vec.x = +kMoveSpeed * 0.5f;
		attack.pos.x = m_pos.x + attack.attackOffSetX;
		if (attack.count == 2 && !m_vec.y > 0)
		{
			m_power = 30;
			m_pAnimation->ChangeAnim(m_modelHandle, kAttack2AnimNo, false, 0.7f);
		}
		if (attack.count == 3 && !m_vec.y > 0)
		{
			m_power = 40;
			m_vec.x = +kMoveSpeed;
			attack.pos.x = m_pos.x + attack.attackOffSetX;
			m_pAnimation->ChangeAnim(m_modelHandle, kAttack3AnimNo, false, 1.0f);
		}
	}
	else 
	{
		// 攻撃の時に少し前進する
		m_vec.x = -kMoveSpeed * 0.5f;
		attack.pos.x = m_pos.x - attack.attackOffSetX;
		if (attack.count == 2 && !m_vec.y > 0)
		{
			m_power = 30;
			m_pAnimation->ChangeAnim(m_modelHandle, kAttack2AnimNo, false, 0.7f);
		}
		if (attack.count == 3 && !m_vec.y > 0)
		{
			m_power = 40;
			m_vec.x = -kMoveSpeed;
			attack.pos.x = m_pos.x - attack.attackOffSetX;
			m_pAnimation->ChangeAnim(m_modelHandle, kAttack3AnimNo, false, 1.0f);
		}
	}
	attack.timer = 50.0f;
	attack.pos.y = m_pos.y+attack.attackOffSetY;
	attack.pos.z = m_pos.z;
}

void Player::DoEvade()
{
	if (evadeData.evadeCount < 1)
	{
		evadeData.active = true;
		// 回避アニメーションに切り替え
		m_pAnimation->ChangeAnim(m_modelHandle, 16, false, 0.5f);
		// 回避回数を増やす
		evadeData.evadeCount++;
		evadeData.timer = 30.0f;
		if (m_isDirRight)
		{
			MV1SetRotationXYZ(m_modelHandle, kLeftDir); // モデルは進行方向に背を向ける
			m_vec.x = kMoveSpeed * 3.0f;
			m_isAttackDirRight = false;
		}
		else
		{
			MV1SetRotationXYZ(m_modelHandle, kRightDir); // モデルは進行方向に背を向ける
			m_vec.x = -kMoveSpeed * 3.0f;
			m_isAttackDirRight = true;
		}
		m_vec.y = kJumpPower * 0.50f;
	}
	MV1SetPosition(m_modelHandle, m_pos);
	m_pos = VAdd(m_pos, m_vec);
}

bool Player::IsMoving()
{
	// 振り向いたら距離をリセット
	if (m_isPrevDirRight != m_isNowDirRight)
	{
		m_prevPos = m_pos;
		m_isMovingFlag = false;
	}
	// 移動前と移動後の距離の大きさを測る
	m_distanceAfterMoving = VSize(VSub(m_pos, m_prevPos));
	m_isPrevDirRight = m_isNowDirRight;
	if (!m_isMovingFlag && m_distanceAfterMoving > kMoveCameraThreshold)
	{
		m_isMovingFlag = true;
	}
	return m_isMovingFlag;
}

int Player::GetMaxHp()
{
	return kMaxHp;
}

void Player::AddPos(VECTOR offset)
{
	m_pos = VAdd(m_pos, offset);
}


void Player::DoMove()
{
	if (isStartGravity)
	{
		m_vec.y += kJumpGravity;
	}

	if (Pad::isPress(PAD_INPUT_RIGHT) && !evadeData.active)
	{
		m_isNowDirRight = true;
		if (!evadeData.active)
		{
			MV1SetRotationXYZ(m_modelHandle, kRightDir);
		}
		if (!isMove)
		{
			isMove = true;
		}
		m_vec.x = kMoveSpeed;
		// ダッシュボタンを押した場合
		if (Pad::isPress(PAD_INPUT_3) && m_pos.y <= 0)
		{
			m_vec.x = kDashSpeed;
		}
		m_isAttackDirRight = true;
		m_isDirRight = true;
	}
	else if (Pad::isPress(PAD_INPUT_LEFT) && !evadeData.active)
	{
		m_isNowDirRight = false;
		if (!evadeData.active)
		{
			MV1SetRotationXYZ(m_modelHandle, kLeftDir);
		}
		if (!isMove)
		{
			isMove = true;
		}
		m_vec.x = -kMoveSpeed;
		// ダッシュボタンを押した場合
		if (Pad::isPress(PAD_INPUT_3) && m_pos.y <= 0)
		{
			m_vec.x = -kDashSpeed;
		}
		m_isAttackDirRight = false;
		m_isDirRight = false;
	}
	else if (Pad::isPress(PAD_INPUT_UP))
	{
		if (!isMove)
		{
			isMove = true;
		}
		m_vec.z = kMoveSpeed;
	}
	else if (Pad::isPress(PAD_INPUT_DOWN))
	{
		if (!isMove)
		{
			isMove = true;
		}
		m_vec.z = -kMoveSpeed;
	}
	else
	{
		isMove = false;
		moveCount = 0;
		if (idleCount < 1)
		{
			m_pAnimation->ChangeAnim(m_modelHandle, kIdleAnimNo, true, 0.5f);
		}
		idleCount++;
	}
	m_vec.x *= kMoveDecRate;
	m_vec.z *= kMoveDecRate;
	MV1SetPosition(m_modelHandle, m_pos);
	if (m_pos.z >= kZLimit)
	{
		m_vec.z = 0.0f;
		m_pos.z = kZLimit-0.001f;
	}
	m_pos = VAdd(m_pos, m_vec);
}
