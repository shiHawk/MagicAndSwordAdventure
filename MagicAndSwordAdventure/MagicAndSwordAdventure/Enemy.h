#pragma once
#include <DxLib.h>
#include <memory>
#include "CharacterBase.h"
#include "Player.h"
class Collision;
class Player;
class Enemy:public CharacterBase
{
public:
	Enemy();
	virtual ~Enemy();

	void Init(std::shared_ptr<Collision> pCollsion, std::shared_ptr<Player> pPlayer);
	virtual void Update(); 
	void OnDamage(int power);
	// エネミーの当たり判定の半径を取得する
	float GetColRadius() const;
	
	bool m_isDirRight;
	float m_moveSpeed;
	bool IsDead() { return m_isDead; }
protected:
	void SetModelFramePos(int modelHandle,int index,int setModelHandle);
	int m_destroyScore;
	// 攻撃を開始するまでの時間
	float m_attackWaitingTime;
	// 死亡時の吹き飛ぶ方向
	VECTOR m_knockbackDir;
	// 吹き飛ぶ速度
	float m_knockbackSpeed;
	// 吹き飛んでいる時間
	float m_knockbackDuration;
	float m_knockbackTimer;
	float m_enemyToPlayerDistance;
	VECTOR m_enemyToPlayer;
private:
	std::shared_ptr<Collision> m_pCollsion;
	std::shared_ptr<Player> m_pPlayer;
};

