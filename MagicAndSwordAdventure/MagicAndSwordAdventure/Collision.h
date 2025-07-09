#pragma once
#include<DxLib.h>
#include "Player.h"
#include "Enemy.h"
#include "NormalSkelton.h"
#include "WizardSkelton.h"
#include<memory>
class Player;
class Enemy;
class NormalSkelton;
class Collision
{
public:
	Collision();
	~Collision();

	void Init(std::shared_ptr<Player> pPlayer, std::shared_ptr<Enemy> pEnemy, 
			  std::shared_ptr<NormalSkelton> pNormalSkelton, std::shared_ptr<WizardSkelton> pWizardSkelton);
	void End();
	void Update();
	void Draw();
private:
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Enemy> m_pEnemy;
	std::shared_ptr<NormalSkelton> m_pNormalSkelton;
	std::shared_ptr<WizardSkelton> m_pWizardSkelton;
	// �v���C���[���U��������������
	bool m_isPlayerHit;
	// �G�l�~�[���U���ɓ���������
	bool m_enemyHit;
	// ���G����
	float m_invincibilityTime;
	float m_playerAttackToNormalSkeltonDistance;
	float m_NormalSkeltonAttackToPlayerDistance;
	VECTOR m_playerToEnemyAttack; // �v���C���[����G�l�~�[�̍U��
	VECTOR m_playerAttackToEnemy; // �v���C���[�̍U������G
};

