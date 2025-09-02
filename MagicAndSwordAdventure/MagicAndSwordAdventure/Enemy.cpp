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
	m_vec = { 0,0,0 };
	m_moveSpeed = 0.0f;
	m_enemyToPlayerDistance = 0.0f;
	m_destroyScore = 0;
	m_preparingTime = 60.0f;
	m_enemyToPlayer = { 0.0f,0.0f,0.0f };
	m_isDirRight = false;
	m_knockbackDir = { 0.0f,0.0f,0.0f };
	m_knockbackDuration = 30.0f;
	m_knockbackSpeed = 0.0f;
	m_knockbackTimer = 0.0f;
}

Enemy::~Enemy()
{
}

void Enemy::Init(std::shared_ptr<Collision> pCollsion, std::shared_ptr<Player> pPlayer)
{
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

float Enemy::GetColRadius() const
{
	return kColRadius;
}
void Enemy::SetModelFramePos(int modelHandle, int index, int setModelHandle)
{
	MATRIX frameMatrix;
	int frameIndex;
	// フレーム名からフレーム番号を取得する
	frameIndex =index;
	frameMatrix = MV1GetFrameLocalWorldMatrix(modelHandle,frameIndex);
	MV1SetMatrix(setModelHandle,frameMatrix);
}
