#pragma once
#include<DxLib.h>
#include "Player.h"
#include "Enemy.h"
#include "NormalSkelton.h"
#include "WizardSkelton.h"
#include<memory>
#include <vector>
class Player;
class Enemy;
class NormalSkelton;
class Collision
{
public:
	Collision();
	~Collision();

	void Init(std::shared_ptr<Player> pPlayer, std::shared_ptr<Enemy> pEnemy, 
		std::vector<std::shared_ptr<NormalSkelton>> normalSkeltons,
		std::vector<std::shared_ptr<WizardSkelton>> wizardSkeltons);
	void End();
	void Update();
	void PlayerHit(float enemyAttackToPlayer,float playerRadius,float enemyAttackRadius,bool enemyAttackActive);
	void EnemyHit(float playerAttackToEnemy, float playerRadius, float enemyAttackRadius,bool playerAttackActive,bool enemyHit);
	void Draw();
	bool GetIsPlayerHit() { return m_isPlayerHit; }
private:
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Enemy> m_pEnemy;
	/*std::shared_ptr<NormalSkelton> m_pNormalSkelton;
	std::shared_ptr<WizardSkelton> m_pWizardSkelton;*/
	std::vector<std::shared_ptr<NormalSkelton>> m_normalSkeltons;
	std::vector<std::shared_ptr<WizardSkelton>> m_wizardSkeltons;
	// �v���C���[���U���ɓ���������
	bool m_isPlayerHit;
	// NormalSkelton���U���ɓ���������
	bool m_normalSkeltonHit;
	// WizardSkelton���U���ɓ���������
	bool m_wizardSkeltonHit;
	// ���G����
	float m_invincibilityTime;
	float m_playerAttackToNormalSkeltonDistance; // �v���C���[�̍U������NormalSkelton�܂ł̋����̑傫��
	float m_NormalSkeltonAttackToPlayerDistance; // NormalSkelton�̍U������v���C���[�܂ł̋����̑傫��
	float m_playerAttackToWizardSkeltonDistance; // �v���C���[�̍U������WizardSkelton�܂ł̋����̑傫��
	float m_WizardSkeltonAttackToPlayerDistance; // WizardSkelton�̍U������v���C���[�܂ł̋����̑傫��
	VECTOR m_playerToNormalSkeltonAttack; // �v���C���[����NormalSkelton�̍U��
	VECTOR m_playerAttackToNormalSkelton; // �v���C���[�̍U������NormalSkelton
	VECTOR m_playerToWizardSkeltonAttack; // �v���C���[����WizardSkelton�̍U��
	VECTOR m_playerAttackToWizardSkelton; // �v���C���[�̍U������WizardSkelton
};

