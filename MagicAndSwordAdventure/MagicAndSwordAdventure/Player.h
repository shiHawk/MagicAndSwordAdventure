#pragma once
#include "DxLib.h"
#include <memory>
class Player
{
public:
	Player();
	~Player();

	void Init();
	void End();
	void Update();
	void Draw();
	VECTOR GetPos() { return m_pos; };
	float GetPosX() { return m_pos.x; };
	float GetPosY() { return m_pos.y; };
	float GetPosZ() { return m_pos.z; };
	// �G�l�~�[�̓����蔻��̔��a���擾����
	float GetColRadius() const;
	void OnDamage();
	int GetHp() const { return m_hp; }
private:
	int m_handle;
	VECTOR m_pos;
	VECTOR m_vec;
	int m_damageFrame;
	int m_hp;
	// �v���C���[�̉�]�s��
	MATRIX m_rotMtx;
	// �v���C���[�̌����Ă������
	float m_angle;
};

