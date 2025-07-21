#include "Enemy.h"
namespace
{
	// �����̑���
	constexpr float kSpeedIncrement = 0.5f;
	// �o���ꏊ
	constexpr float kAppearancePosX = 900.0f;
	constexpr float kBackPosZ = 100.0f;
	constexpr float kFrontPosZ = 230.0f;
	// �ړ����E
	constexpr float kMobilityLimitPosX = -900.0f;
	// �����蔻��̉~
	constexpr float kColRadius = 30.0f;
	// �ő呬�x
	constexpr float kMaxSpeed = 15.0f;
}
Enemy::Enemy()
{
	m_pos = { 0,0,0 };
	m_vec = { 0,0,0 };
	m_moveSpeed = 0.0f;
	m_enemyToPlayerDistance = 0.0f;
	m_destroyScore = 0;
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

void Enemy::OnDamage(int power)
{
	m_hp -= power;
	if (m_hp < 0)
	{
		m_isDead = true;
		m_hp = 0;
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
