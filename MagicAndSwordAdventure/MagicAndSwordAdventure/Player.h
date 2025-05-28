#pragma once
#include "DxLib.h"
#include <memory>
class Player
{
public:
	Player();
	~Player();

	void Init();
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
private:
	int m_handle;
	VECTOR m_pos;
	VECTOR m_vec;
	int m_damageFrame;
	int m_hp;
	// プレイヤーの回転行列
	MATRIX m_rotMtx;
	// プレイヤーの向いている方向
	float m_angle;
};

