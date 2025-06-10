#pragma once
#include "DxLib.h"
#include "Enemy.h"
#include <memory>
class Player
{
public:
	struct AttackSphere
	{
		float x, y, z, radius;
		bool active;
		float timer;
	};
	Player();
	~Player();

	void Init(std::shared_ptr<Enemy> pEnemy);
	void End();
	void Update();
	void Draw() const;
	VECTOR GetPos() { return m_pos; };
	// �G�l�~�[�̓����蔻��̔��a���擾����
	float GetColRadius() const;
	void OnDamage();
	int GetHp() const { return m_hp; }
	void DoAttack();
	AttackSphere attack = { 0,0,0,30,false,0 };
private:
	int m_handle;
	VECTOR m_pos;
	VECTOR m_vec;
	int m_damageFrame;
	int m_hp;
	int m_playerHandle;
	std::shared_ptr<Enemy> m_pEnemy;
	// �v���C���[�̉�]�s��
	MATRIX m_rotMtx;
	// �v���C���[�̌����Ă������
	float m_angle;
	bool m_isJump;
	bool m_isDirRight;
	bool m_isPrevButton;
	bool m_isNowButton;
};

