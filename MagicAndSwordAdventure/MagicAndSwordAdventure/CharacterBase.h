#pragma once
#include "DxLib.h"
class CharacterBase
{
public:
	void Draw() const;
	void UpdateMove();
	virtual void Update() = 0;
	VECTOR GetPos() { return m_pos; };
	int GetHp() const { return m_hp; }
	VECTOR m_pos;
	VECTOR m_vec;
	int m_modelHandle;
	bool m_isDirRight;
	int m_hp;
	float m_moveSpeed;
	float m_DashSpeed;
	struct AttackSphere
	{
		float radius; // �U���̔���
		VECTOR pos;
		bool active; // �U�����
		float timer; // �U������
		int count; // �U����
		int comboDuration; // ��������
	};
	AttackSphere attack;
};

