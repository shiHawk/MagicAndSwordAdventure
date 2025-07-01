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
	void Update();
	void Draw();
	int GetPosIndex() { return m_posIndex; }
	// �G�l�~�[�̓����蔻��̔��a���擾����
	float GetColRadius() const;
	int m_posIndex;
private:
	std::shared_ptr<Collision> m_pCollsion;
	std::shared_ptr<Player> m_pPlayer;
	float m_moveSpeed;
	float m_enemyToPlayerDistance;
	bool m_isDirRight;
};

