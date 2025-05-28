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


Player::Player()
{
}

Player::~Player()
{
}

void Player::Init()
{
	m_pos = VGet(0, 40, 0);
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
	return 0.0f;
}

void Player::OnDamage()
{
}
