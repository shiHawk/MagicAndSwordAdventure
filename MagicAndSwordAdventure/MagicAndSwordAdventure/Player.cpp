#include "Player.h"
#include "DxLib.h"
#include "Pad.h"
#include <cmath>

namespace
{
	constexpr float kMoveSpeed = 2.5f;
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
m_isDir(true)
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
	if (m_isJump)
	{
		m_vec.y += kGravity;
	}
	
	if (Pad::isPress(PAD_INPUT_RIGHT))
	{
		m_vec.x = kMoveSpeed;
		m_isDir = true;
	}
	if (Pad::isPress(PAD_INPUT_LEFT))
	{
		m_vec.x = -kMoveSpeed;
		m_isDir = false;
	}
	if (Pad::isPress(PAD_INPUT_UP))
	{
		m_vec.z = kMoveSpeed;
	}
	if (Pad::isPress(PAD_INPUT_DOWN))
	{
		m_vec.z = -kMoveSpeed;
	}
	if (Pad::isPress(PAD_INPUT_1) && !m_isJump)
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
}

void Player::Draw()
{
	DrawSphere3D(m_pos, kColRadius,8,0xffff00,0xffffff,true);
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
	AttackSphere attack = { 0,0,0,30,false,0 };
	attack.active = true;
	attack.timer = 10.0f;
	if (m_isDir)
	{
		attack.x = m_pos.x + 30;
	}
	else
	{
		attack.x = m_pos.x - 30;
	}
	attack.y = m_pos.y;
	attack.z = m_pos.z;
}
