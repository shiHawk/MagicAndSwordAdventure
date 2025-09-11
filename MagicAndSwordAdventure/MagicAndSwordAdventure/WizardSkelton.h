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
	// プレイヤーに向かうベクトル
	VECTOR m_toPlayerDir;
	bool m_isAttackEnd;
	int m_attackCount;
	bool m_isCasting; // 予備動作開始
	bool m_isCastFinished; // 予備動作終了
	int m_barrelHandle; // 樽のハンドル
	float m_rollAngleZ; // 回転
	VECTOR m_attackDir; // 弾の進行方向を保持する変数
	int m_homingTimer; // 弾が追尾するまでの時間
	bool m_isTrackFlag;
};

