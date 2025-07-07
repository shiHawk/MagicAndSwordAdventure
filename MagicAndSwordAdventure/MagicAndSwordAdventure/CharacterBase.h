#pragma once
#include "DxLib.h"
#include "Animation.h"
class CharacterBase:public Animation
{
public:
	void Draw() const;
	void UpdateMove();
	virtual void Update() = 0;
	VECTOR GetPos() { return m_pos; };
	int GetHp() const { return m_hp; }
	bool GetAttackActive() { return attack.active; }
	VECTOR m_pos;
	VECTOR m_vec;
	int m_modelHandle;
	bool m_isDirRight;
	int m_hp;
	float m_moveSpeed;
	float m_DashSpeed;
	struct AttackSphere
	{
		float radius; // 攻撃の判定
		VECTOR pos;
		bool active; // 攻撃状態
		float timer; // 攻撃時間
		int count; // 攻撃回数
		int comboDuration; // 持続時間
		float attackOffSetX; // 攻撃のX座標のオフセット
		float attackOffSetY; // 攻撃のY座標のオフセット
		float attackCoolTime; // 攻撃のクールタイム
	};
	AttackSphere attack;
};

