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
		float radius; // �U���̔���
		VECTOR pos;
		bool active; // �U�����
		float timer; // �U������
		int count; // �U����
		int comboDuration; // ��������
		float attackOffSetX; // �U����X���W�̃I�t�Z�b�g
		float attackOffSetY; // �U����Y���W�̃I�t�Z�b�g
		float attackCoolTime; // �U���̃N�[���^�C��
	};
	AttackSphere attack;
};

