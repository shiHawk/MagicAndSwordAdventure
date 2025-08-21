#pragma once
#include "DxLib.h"
#include "Enemy.h"
#include "Animation.h"
#include "CharacterBase.h"
#include <memory>
enum  class Dir
{
	Right,
	Left,
	Front,
	Back
};
class Player:public CharacterBase
{
public:
	Player();
	~Player();

	void Init(std::shared_ptr<Animation> pAnimation);
	void End();
	void Update();
	void Draw() const;
	// �v���C���[�̓����蔻����擾
	float GetColRadius() const ;
	void OnDamage(int enemyPower);
	void OnDeath();
	// �U��
	void DoAttack();
	// ���
	void DoEvade();
	VECTOR GetAttackPos() { return attack.pos; }
	int GetAttackCount() { return attack.count; }
	float GetAttackRadius() { return attack.radius; }
	bool IsAttackActive() const { return attack.active; }
	bool IsDirRight() { return m_isDirRight; }
	bool IsMoving();
	bool IsDead() { return m_isDead; }
	int GetHp() { return m_hp; }
	bool IsPinch();
	int GetMaxHp();
	AttackSphere attack = { 30,{-500,0,0},false,0.0f,0,30.0f,60.0f,40.0f};
	void AddPos(VECTOR offset);
private:
	// �ړ�
	void DoMove();
	void HandleJump();
	void HandleEvade();
	void UpdateCombo();
	void HandleInput();
	struct EvadeData
	{
		int evadeCount; // �����
		bool active; // ������
		float timer; // �������
	};
	EvadeData evadeData = { 0,false,0.0f };
	// �U�����o�����
	bool m_isAttackDirRight;
	Dir m_playerDir;
	VECTOR m_screenPos;
	int m_damageFrame;
	int m_hp;
	// �W�����v��
	int m_jumpCount;
	std::shared_ptr<Animation> m_pAnimation;
	// �v���C���[�̉�]�s��
	MATRIX m_rotMtx;
	// �v���C���[�̌����Ă������
	bool m_isDirRight;
	bool m_isJump;
	// �ړ��O�̈ʒu
	VECTOR m_prevPos;
	// �ړ��O�ƈړ���̋���
	float m_distanceAfterMoving;
	// ���O�ɉ�����Ă��邩
	bool m_isPrevButton;
	// ���݉�����Ă��邩
	bool m_isNowButton;
	// �ړ��������ǂ���
	bool m_isMovingFlag;
	// �O������Ă������
	bool m_isPrevDirRight;
	// ���݌����Ă������
	bool m_isNowDirRight;
	bool m_isAttackingAnim;
	bool m_isDamageAnim;
};

