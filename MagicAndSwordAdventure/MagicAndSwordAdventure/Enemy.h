#pragma once
#include <DxLib.h>
#include <memory>
class Collision;
class Enemy
{
public:
	Enemy();
	virtual ~Enemy();

	void Init(std::shared_ptr<Collision> pCollsion);
	void Update();
	void Draw();
	VECTOR GetPos() const { return m_pos; }
	float GetPosX() { return m_pos.x; }
	float GetPosY() { return m_pos.y; }
	float GetPosZ() { return m_pos.z; }
	int GetPosIndex() { return m_posIndex; }
	// エネミーの当たり判定の半径を取得する
	float GetColRadius() const;
	int m_posIndex;
private:
	VECTOR m_pos;
	VECTOR m_vec;
	std::shared_ptr<Collision> m_pCollsion;
	float m_moveSpeed;
	bool m_isDir;
};

