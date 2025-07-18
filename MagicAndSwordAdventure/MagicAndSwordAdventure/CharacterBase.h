#pragma once
#include "DxLib.h"
#include "Animation.h"
class CharacterBase:public Animation
{
public:
	void Draw() const;
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

