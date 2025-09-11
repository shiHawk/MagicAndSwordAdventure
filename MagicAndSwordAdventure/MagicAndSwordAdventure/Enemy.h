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
	bool IsDead() { return m_isDead; }
protected:
	void SetModelFramePos(int modelHandle,int index,int setModelHandle);
	bool m_isDirRight;
	float m_moveSpeed;
	int m_destroyScore;
	// 攻撃を構えている時間
	float m_preparingTime;
	// 死亡時の吹き飛ぶ方向
	VECTOR m_knockbackDir;
	float m_knockbackTimer;
	float m_enemyToPlayerDistance;
	VECTOR m_enemyToPlayer;
private:
	std::shared_ptr<Collision> m_pCollsion;
	std::shared_ptr<Player> m_pPlayer;
};

