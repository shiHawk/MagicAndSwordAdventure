#include "Player.h"
#include "DxLib.h"
#include "Pad.h"

namespace
{
	constexpr float kMoveSpeed = 5.0f;
	constexpr float kDashSpeed = 10.0f;
	constexpr float kJumpPower = 9.0f;
	constexpr float kJumpGravity = -0.4f;
	// 移動限界
	constexpr float kBackLimit = 100.0f;
	constexpr float kFrontLimit = -340.0f;

	// 当たり判定の範囲
	constexpr float kColRadius = 50.0f;
	// 最大HP
	constexpr int kMaxHp = 50;
	// 減速
	constexpr float kMoveDecRate = 0.80f;

	constexpr VECTOR kRightDir = { 0.0,270.0f * DX_PI_F / 180.0f,0.0f };
	constexpr VECTOR kLeftDir = { 0.0,90.0f * DX_PI_F / 180.0f,0.0f };

	const char* kIdleAnimName = "CharacterArmature|Idle";
	const char* kRunAnimName = "CharacterArmature|Run";
	bool isStartGravity = false;
}


Player::Player():m_handle(-1),
m_damageFrame(0),
m_hp(kMaxHp),
m_rotMtx(MGetIdent()),
m_angle(0.0f),
m_isJump(false),
m_isDirRight(true),
m_isPrevButton(false),
m_isNowButton(false),
m_playerHandle(0),
m_jumpCount(0),
m_isAttackDirRight(true),
m_playTime(0.0f)
{
	m_pos = { 0, 0, 0 };
	m_vec = VGet(0, 0, 0);
}

Player::~Player()
{
}

void Player::Init(std::shared_ptr<Enemy> pEnemy, std::shared_ptr<Animation> pAnimation)
{
	m_pos = VGet(0, 0, 0);
	m_pEnemy = pEnemy;
	m_pAnimation = pAnimation;
	m_playerHandle = MV1LoadModel(L"Data/model/Barbarian.mv1");
	MV1SetRotationXYZ(m_playerHandle, kRightDir);
	m_pAnimation->AttachAnim(m_playerHandle, 1);
}

void Player::End()
{
	MV1DeleteModel(m_playerHandle);
}

void Player::Update()
{
	if (m_playTime >= m_animTotalTime)
	{
		m_playTime = 0.0f;
	}
	if (m_vec.y > 0)
	{
		isStartGravity = true;
	}
	// Bボタンを押したとき
	m_isNowButton = Pad::isPress(PAD_INPUT_2);
	if (isStartGravity)
	{
		m_vec.y += kJumpGravity;
	}
	
	if (Pad::isPress(PAD_INPUT_RIGHT) && !evadeData.active)
	{
		if (!evadeData.active)
		{
			MV1SetRotationXYZ(m_playerHandle, kRightDir);
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
	if (Pad::isPress(PAD_INPUT_LEFT) && !evadeData.active)
	{
		if (!evadeData.active)
		{
			MV1SetRotationXYZ(m_playerHandle, kLeftDir);
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
	if (Pad::isPress(PAD_INPUT_UP))
	{
		m_vec.z = kMoveSpeed;
	}
	if (Pad::isPress(PAD_INPUT_DOWN))
	{
		m_vec.z = -kMoveSpeed;
	}
	// Aボタンを押したときジャンプ
	if (Pad::isTrigger(PAD_INPUT_1) && !m_isJump)
	{
		m_vec.y = kJumpPower;
		m_jumpCount++;
	}
	// ジャンプは2回まで
	if (m_jumpCount > 1)
	{
		m_isJump = true;
	}

	//RBボタンを押したとき
	if (Pad::isTrigger(PAD_INPUT_6) )
	{
		if (evadeData.evadeCount < 1)
		{
			DoEvade();
		}
	}
	if (evadeData.active)
	{
		evadeData.timer--;
		if (evadeData.timer <= 0)
		{
			m_pAnimation->ChangeAnim(m_playerHandle, 1, true);
			evadeData.active = false;
			evadeData.evadeCount = 0;
		}
	}

	m_vec.x *= kMoveDecRate;
	m_vec.z *= kMoveDecRate;

	m_pos = VAdd(m_pos, m_vec);
	MV1SetScale(m_playerHandle,VGet(50,50,50));
	MV1SetPosition(m_playerHandle,m_pos);
	if (m_pos.y < 0.0f)
	{
		m_pos.y = 0.0f;
		m_vec.y = 0;
		m_isJump = false;
		m_jumpCount = 0;
	}

	// Bボタンが押されっぱなしでない
	if (m_isNowButton && !m_isPrevButton)
	{
		DoAttack();
	}
	m_isPrevButton = m_isNowButton;

	if (attack.active)
	{
		attack.timer--;
		attack.comboDuration--;
		if (attack.timer <= 0)
		{
			m_pAnimation->ChangeAnim(m_playerHandle,1,true);
			attack.active = false;
		}
	}
	//printfDx(L"%f\n", m_screenPos.x);
	m_pAnimation->Update();
}

void  Player::Draw() const
{
	MV1DrawModel(m_playerHandle);
#if _DEBUG
	if (attack.active)
	{
		DrawSphere3D(VGet(attack.x,attack.y,attack.z),attack.radius,8,0xff0000,0xffffff,false);
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

void Player::OnDamage()
{
}

void Player::DoAttack()
{
	attack.active = true;
	m_pAnimation->ChangeAnim(m_playerHandle, 31,false);
	attack.timer = 50.0f;
	attack.comboDuration = 20.0f;
	attack.count++;
	if (attack.count > 3)
	{
		attack.count = 0;
	}
	printfDx(L"attack.count:%d\n", attack.count);
	if (m_isAttackDirRight)
	{
		m_vec.x = +kMoveSpeed * 0.5f;
		attack.x = m_pos.x + 60;
		if (attack.count == 4)
		{
			m_vec.x = +kMoveSpeed;
			attack.x = m_pos.x + 60;
		}
	}
	else 
	{
		m_vec.x = -kMoveSpeed * 0.5f;
		attack.x = m_pos.x - 60;
		if (attack.count == 4)
		{
			m_vec.x = -kMoveSpeed;
			attack.x = m_pos.x - 60;
		}
	}

	attack.y = m_pos.y+40;
	attack.z = m_pos.z;
}

void Player::DoEvade()
{
	evadeData.active = true;
	m_pAnimation->ChangeAnim(m_playerHandle, 16, false);
	evadeData.evadeCount++;
	evadeData.timer = 30.0f;
	if (evadeData.evadeCount > 1)
	{
		return;
	}

	if (m_isDirRight)
	{
		MV1SetRotationXYZ(m_playerHandle, kLeftDir);
		m_vec.x = kMoveSpeed * 3.0f;
		m_isAttackDirRight = false;
	}
	else
	{
		MV1SetRotationXYZ(m_playerHandle, kRightDir);
		m_vec.x = -kMoveSpeed * 3.0f;
		m_isAttackDirRight = true;
	}

	m_vec.y = kJumpPower * 0.50f;
}