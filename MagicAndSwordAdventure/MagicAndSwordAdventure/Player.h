#pragma once
#include "DxLib.h"
#include "Enemy.h"
#include "Animation.h"
#include "CharacterBase.h"
#include <memory>
class Player:public CharacterBase
{
public:
	Player();
	~Player();

	void Init(std::shared_ptr<Animation> pAnimation);
	void End();
	void Update();
	void Draw() const;
	VECTOR GetScreenPos();
	// �G�l�~�[�̓����蔻��̔��a���擾����
	float GetColRadius() const ;
	void OnDamage();
	
	// �U��
	void DoAttack();
	// ���
	void DoEvade();
	VECTOR GetAttackPos() { return attack.pos; }
	float GetAttackRadius() { return attack.radius; }
	bool GetDirRight() { return m_isDirRight; }
	bool GetIsMoving();
	AttackSphere attack = { 30,{-500,0,0},false,0,0,30,60.0,40.0};
private:
	// �ړ�
	void DoMove();
	struct EvadeData
	{
		int evadeCount; // �����
		bool active; // ������
		float timer; // �������
	};
	EvadeData evadeData = { 0,false,0.0f };
	// �U�����o�����
	bool m_isAttackDirRight;
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
	VECTOR m_playerPosHead;
	VECTOR m_playerPosFoot;
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
};

