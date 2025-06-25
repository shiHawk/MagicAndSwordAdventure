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

	void Init(std::shared_ptr<Enemy> pEnemy, std::shared_ptr<Animation> pAnimation);
	void End();
	void Update();
	void Draw() const;
	VECTOR GetScreenPos();
	// �G�l�~�[�̓����蔻��̔��a���擾����
	float GetColRadius() const ;
	void OnDamage();
	
	void DoAttack();
	// ���
	void DoEvade();
	AttackSphere attack = { 0,0,0,30,false,0,0,30 };
private:
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
	//int m_modelHandle;
	// �W�����v��
	int m_jumpCount;
	std::shared_ptr<Enemy> m_pEnemy;
	std::shared_ptr<Animation> m_pAnimation;
	// �v���C���[�̉�]�s��
	MATRIX m_rotMtx;
	// �v���C���[�̌����Ă������
	float m_angle;
	bool m_isJump;
	bool m_isDirRight;

	// ���O�ɉ�����Ă��邩
	bool m_isPrevButton;
	// ���݉�����Ă��邩
	bool m_isNowButton;
};

