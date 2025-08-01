#pragma once
#include "DxLib.h"
#include "Animation.h"
class CharacterBase:public Animation
{
public:
	CharacterBase();
	virtual void Update() = 0;
	VECTOR GetPos() { return m_pos; };
	void SetPos(VECTOR nextPos) { m_pos = nextPos; }
	int GetHp() const { return m_hp; }
	int GetPower() const { return m_power; }
	bool GetAttackActive() { return attack.active; }
	VECTOR m_pos;
	VECTOR m_vec;
	int m_modelHandle;
	bool m_isDirRight;
	bool m_isDead;
	bool m_isDying;

	int m_hp;
	int m_power;
	float m_moveSpeed;
	float m_DashSpeed;
	struct AttackSphere
	{
		float radius; // 攻撃の判定の大きさ
		VECTOR pos;
		bool active; // 攻撃状態
		float timer; // 攻撃時間
		int count; // 攻撃回数
		float comboDuration; // 持続時間
		float attackOffSetX; // 攻撃のX座標のオフセット
		float attackOffSetY; // 攻撃のY座標のオフセット
		float attackCoolTime; // 攻撃のクールタイム
	};
	AttackSphere attack;
};

