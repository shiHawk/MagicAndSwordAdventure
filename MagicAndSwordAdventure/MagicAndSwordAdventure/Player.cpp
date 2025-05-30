#include "Player.h"
#include "DxLib.h"
#include "Pad.h"
#include <cmath>

namespace
{
	constexpr float kMoveSpeed = 2.5f;
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
m_angle(0.0f)
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
}

void Player::Draw()
{
}

float Player::GetColRadius() const
{
	return kColRadius;
}

void Player::OnDamage()
{
}
