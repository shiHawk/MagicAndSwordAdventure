#pragma once
#include "Enemy.h"
#include "Player.h"
#include "ScoreManager.h"
class WizardSkelton :public Enemy
{
public:
	WizardSkelton();
	void Init(std::shared_ptr<Player> pPlayer,VECTOR pos, std::shared_ptr<ScoreManager> pScoreManager);
	void End();
	void Update() override;
	void DoAttack();
	void OnDamage();
	void OnDeath();
	void Draw()const;
	VECTOR GetAttackPos() { return m_attack.pos; }
	float GetAttackRadius() { return m_attack.radius; }
	bool IsAttackActive() const;
private:
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<ScoreManager> m_pScoreManager;
	AttackSphere m_attack;
	// �v���C���[�Ɍ������x�N�g��
	VECTOR m_toPlayerDir;
	bool m_isAttackEnd;
	int m_attackCount;
	bool m_isCasting; // �\������J�n
	bool m_isCastFinished; // �\������I��
	int m_barrelHandle; // �M�̃n���h��
	float m_rollAngleZ; // ��]
	VECTOR m_attackDir; // �e�̐i�s������ێ�����ϐ�
	int m_homingTimer; // �e���ǔ�����܂ł̎���
	bool m_isTrackFlag;
};

