#pragma once
#include <DxLib.h>
#include <memory>
#include "CharacterBase.h"
#include "Player.h"
class Collision;
class Player;
class Enemy:public CharacterBase
{
public:
	Enemy();
	virtual ~Enemy();

	void Init(std::shared_ptr<Collision> pCollsion, std::shared_ptr<Player> pPlayer);
	virtual void Update(); 
	void OnDamage(int power);
	// �G�l�~�[�̓����蔻��̔��a���擾����
	float GetColRadius() const;
	
	bool m_isDirRight;
	float m_moveSpeed;
	bool IsDead() { return m_isDead; }
protected:
	void SetModelFramePos(int modelHandle,int index,int setModelHandle);
	int m_destroyScore;
	// �U�����J�n����܂ł̎���
	float m_attackWaitingTime;
	// ���S���̐�����ԕ���
	VECTOR m_knockbackDir;
	// ������ԑ��x
	float m_knockbackSpeed;
	// �������ł��鎞��
	float m_knockbackDuration;
	float m_knockbackTimer;
	float m_enemyToPlayerDistance;
	VECTOR m_enemyToPlayer;
private:
	std::shared_ptr<Collision> m_pCollsion;
	std::shared_ptr<Player> m_pPlayer;
};

