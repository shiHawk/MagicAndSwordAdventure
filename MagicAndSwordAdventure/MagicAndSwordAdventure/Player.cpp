#include "Player.h"
#include "DxLib.h"
#include "Pad.h"
#include <cmath>

namespace
{
	constexpr float kMoveSpeed = 2.5f;
	constexpr float kDashSpeed = 5.0f;
	constexpr float kJumpPower = 13.0f;
	constexpr float kGravity = -0.5f;
	// ˆÚ“®ŒÀŠE
	constexpr float kXLimit = 900.0f;
	constexpr float kBackLimit = 100.0f;
	constexpr float kFrontLimit = -340.0f;
	constexpr float kLeftLimit = -475.0f;
	constexpr float kRightLimit = 472.0f;

	// “–‚½‚è”»’è‚Ì”ÍˆÍ
	constexpr float kColRadius = 50.0f;
	constexpr int kMaxHp = 50;
	// Œ¸‘¬
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
m_isNowButton(false)
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
	
}

void Player::End()
{
}

void Player::Update()
{
	m_isNowButton = Pad::isPress(PAD_INPUT_2);
	if (m_isJump)
	{
		m_vec.y += kGravity;
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
		if (Pad::isPress(PAD_INPUT_3))
		{
			m_vec.z = kDashSpeed;
		}
	}
	if (Pad::isPress(PAD_INPUT_DOWN))
	{
		m_vec.z = -kMoveSpeed;
		if (Pad::isPress(PAD_INPUT_3))
		{
			m_vec.z = -kDashSpeed;
		}
	}
	if (Pad::isTrigger(PAD_INPUT_1) && !m_isJump)
	{
		m_vec.y = kJumpPower;
		m_isJump = true;
	}

	m_vec.x *= kMoveDecRate;
	m_vec.z *= kMoveDecRate;

	m_pos = VAdd(m_pos, m_vec);
	if (m_pos.y < 40.0f)
	{
		m_pos.y = 40.0f;
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

void Player::Draw()
{
	DrawSphere3D(m_pos, kColRadius,8,0xffff00,0xffffff,true);
	if (attack.active)
	{
		DrawSphere3D(VGet(attack.x,attack.y,attack.z),attack.radius,8,0xff0000,0xffffff,false);
	}
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
	if (m_isDirRight)
	{
		attack.x = m_pos.x + 50;
	}
	else
	{
		attack.x = m_pos.x - 50;
	}
	attack.y = m_pos.y;
	attack.z = m_pos.z-30;
}
