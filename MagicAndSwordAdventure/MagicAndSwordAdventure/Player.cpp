#include "Player.h"
#include "DxLib.h"
#include "Pad.h"

namespace
{
	constexpr float kMoveSpeed = 8.0f;
	constexpr float kDashSpeed = 12.0f;
	constexpr float kJumpPower = 10.0f;
	constexpr float kJumpGravity = -0.4f;
	// à⁄ìÆå¿äE
	constexpr float kBackLimit = 100.0f;
	constexpr float kFrontLimit = -340.0f;

	// ìñÇΩÇËîªíËÇÃîÕàÕ
	constexpr float kColRadius = 50.0f;
	// ç≈ëÂHP
	constexpr int kMaxHp = 50;
	// å∏ë¨
	constexpr float kMoveDecRate = 0.80f;
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
m_playerHandle(0)
{
	m_pos = { 0, 0, 0 };
	m_vec = VGet(0, 0, 0);
}

Player::~Player()
{
}

void Player::Init(std::shared_ptr<Enemy> pEnemy)
{
	m_pos = VGet(0, 40, 0);
	m_pEnemy = pEnemy;
	m_playerHandle = MV1LoadModel(L"Data/model/Barbarian.mv1");
}

void Player::End()
{
	MV1DeleteModel(m_playerHandle);
}

void Player::Update()
{
	m_isNowButton = Pad::isPress(PAD_INPUT_2);
	if (m_isJump)
	{
		m_vec.y += kJumpGravity;
	}
	
	if (Pad::isPress(PAD_INPUT_RIGHT))
	{
		m_vec.x = kMoveSpeed;
		if (Pad::isPress(PAD_INPUT_3))
		{
			m_vec.x = kDashSpeed;
		}
		m_isDirRight = true;
	}
	if (Pad::isPress(PAD_INPUT_LEFT))
	{
		m_vec.x = -kMoveSpeed;
		if (Pad::isPress(PAD_INPUT_3))
		{
			m_vec.x = -kDashSpeed;
		}
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

	if (Pad::isTrigger(PAD_INPUT_1) && !m_isJump)
	{
		m_vec.y = kJumpPower;
		m_isJump = true;
	}

	m_vec.x *= kMoveDecRate;
	m_vec.z *= kMoveDecRate;

	m_pos = VAdd(m_pos, m_vec);
	MV1SetPosition(m_playerHandle,m_pos);
	if (m_pos.y < 40.0f)
	{
		m_pos.y = 40.0f;
		m_vec.y = 0;
		m_isJump = false;
	}

	if (m_isNowButton && !m_isPrevButton)
	{
		DoAttack();
	}
	m_isPrevButton = m_isNowButton;

	if (attack.active)
	{
		attack.timer--;
		if (attack.timer <= 0)
		{
			attack.active = false;
		}
	}
}

void  Player::Draw() const
{
	DrawSphere3D(m_pos, kColRadius,8,0x0000ff,0xffffff,true);
	MV1DrawModel(m_playerHandle);
#if _DEBUG
	if (attack.active)
	{
		DrawSphere3D(VGet(attack.x,attack.y,attack.z),attack.radius,8,0xff0000,0xffffff,false);
	}
#endif
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
	attack.timer = 10.0f;
	attack.count++;
	if (attack.count > 4)
	{
		attack.count = 1;
	}
	printfDx(L"attack.count:%d", attack.count);
	if (m_isDirRight)
	{
		m_vec.x = +kMoveSpeed * 0.5f;
		if (attack.count == 4)
		{
			m_vec.x = +kMoveSpeed;
		}
		attack.x = m_pos.x + 50;
	}
	else
	{
		m_vec.x = -kMoveSpeed * 0.5f;
		if (attack.count == 4)
		{
			m_vec.x = -kMoveSpeed;
		}
		attack.x = m_pos.x - 50;
	}
	attack.y = m_pos.y;
	attack.z = m_pos.z-30;
}
