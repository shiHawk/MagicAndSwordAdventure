#pragma once
#include "DxLib.h"
#include "Enemy.h"
#include <memory>
class Player
{
public:
	struct AttackSphere
	{
		float x, y, z, radius;
		bool active;
		float timer;
	};
	Player();
	~Player();

	void Init(std::shared_ptr<Enemy> pEnemy);
	void End();
	void Update();
	void Draw();
	VECTOR GetPos() { return m_pos; };
	float GetPosX() { return m_pos.x; };
	float GetPosY() { return m_pos.y; };
	float GetPosZ() { return m_pos.z; };
	// エネミーの当たり判定の半径を取得する
	float GetColRadius() const;
	void OnDamage();
	int GetHp() const { return m_hp; }
	void DoAttack();
private:
	int m_handle;
	VECTOR m_pos;
	VECTOR m_vec;
	int m_damageFrame;
	int m_hp;
	std::shared_ptr<Enemy> m_pEnemy;
	// プレイヤーの回転行列
	MATRIX m_rotMtx;
	// プレイヤーの向いている方向
	float m_angle;
	bool m_isJump;
	bool m_isDir;
};

