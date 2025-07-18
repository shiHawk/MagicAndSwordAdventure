#include "Enemy.h"
namespace
{
	// 速さの増分
	constexpr float kSpeedIncrement = 0.5f;
	// 出現場所
	constexpr float kAppearancePosX = 900.0f;
	constexpr float kBackPosZ = 100.0f;
	constexpr float kFrontPosZ = 230.0f;
	// 移動限界
	constexpr float kMobilityLimitPosX = -900.0f;
	// 当たり判定の円
	constexpr float kColRadius = 30.0f;
	// 最大速度
	constexpr float kMaxSpeed = 15.0f;
}
Enemy::Enemy()
{
	m_pos = { 0,0,0 };
	m_vec = VGet(0, 0, 0);
	m_moveSpeed = 0.0f;
	m_enemyToPlayerDistance = 0.0f;
}

Enemy::~Enemy()
{
}

void Enemy::Init(std::shared_ptr<Collision> pCollsion, std::shared_ptr<Player> pPlayer)
{
	m_pos = VGet(680, 80, 0);
	m_moveSpeed = 5.0f;
	m_pCollsion = pCollsion;
	m_pPlayer = pPlayer;
}

void Enemy::Update()
{
	m_enemyToPlayerDistance = VSize(VSub(m_pos, m_pPlayer->GetPos()));
}

void Enemy::OnDamage(int hp, int power)
{
	hp -= power;
	if (hp < 0)
	{
		m_isDead = true;
		hp = 0;
	}
}

void Enemy::Draw()
{
	DrawSphere3D(m_pos, kColRadius, 32, 0xff0000, 0xffffff, TRUE);
}

float Enemy::GetColRadius() const
{
	return kColRadius;
}
