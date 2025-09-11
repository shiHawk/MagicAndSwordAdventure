#pragma once
#include "DxLib.h"
#include "Enemy.h"
#include "Animation.h"
#include "CharacterBase.h"
#include <memory>

class GameScene;
class Player:public CharacterBase
{
public:
	Player();
	~Player();

	void Init(std::shared_ptr<Animation> pAnimation, GameScene* gameScene);
	void End();
	void Update();
	void Draw() const;
	// �v���C���[�̓����蔻����擾
	float GetColRadius() const ;
	void OnDamage(int enemyPower);
	void OnDeath();
	// �U��
	void OnAttack();
	// ���
	void OnEvade();
	VECTOR GetAttackPos() { return m_attack.pos; }
	int GetAttackCount() { return m_attack.count; }
	float GetAttackRadius() { return m_attack.radius; }
	bool IsAttackActive() const { return m_attack.active; }
	bool IsDirRight() { return m_isDirRight; }
	bool IsMoving() { return m_isMovingFlag; };
	bool IsDead() { return m_isDead; }
	int GetHp() { return m_hp; }
	bool IsPinch();
	int GetMaxHp();
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
	EvadeData m_evadeData = { 0,false,0.0f };
	AttackSphere m_attack;
	// �U�����o�����
	bool m_isAttackDirRight;
	VECTOR m_screenPos;
	int m_damageFrame;
	int m_hp;
	// �W�����v��
	int m_jumpCount;
	std::shared_ptr<Animation> m_pAnimation;
	GameScene* m_pGameScene = nullptr;
	// �v���C���[�̉�]�s��
	MATRIX m_rotMtx;
	// �v���C���[�̌����Ă������
	bool m_isDirRight;
	bool m_isJump;
	// �ړ��O�̈ʒu
	VECTOR m_prevPos;
	VECTOR m_rot = { 0.0f,0.0f,0.0f };
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
	VECTOR m_attackDir;
	bool m_isNowMoving;
	int m_blinkCount; // �_�Ŏ���
	bool m_isVisible; // �`�悷�邩
	// �U��Ԃ邾���ňړ������Ɣ��肳���Ȃ����߂�
	bool m_isMove = false;
	// �A���ňړ��A�j���[�V�������Ă΂Ȃ��悤�ɂ���
	int m_moveCount = 0;
	int m_idleCount = 0;
	bool m_isStartGravity = false;
};

