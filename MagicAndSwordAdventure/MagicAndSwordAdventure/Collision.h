#pragma once
#include<DxLib.h>
#include "Player.h"
#include "Enemy.h"
#include<memory>
class Player;
class Enemy;
class Collision
{
private:
	// 当たり判定用の球のデータを表す構造体
	struct Sphere
	{
		float x, y, z;
		float radius;
	};
public:
	Collision();
	~Collision();

	void Init(std::shared_ptr<Player> pPlayer, std::shared_ptr<Enemy> pEnemy);
	void End();
	void Update();
	void Draw();
	void CheckCol();
	int GetScore() { return m_score; }
private:
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Enemy> m_pEnemy;
	bool m_isColl;
	int m_score;
	int m_fontHandle;
};

