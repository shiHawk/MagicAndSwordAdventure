#pragma once
#include "DxLib.h"
#include "Enemy.h"
#include "Animation.h"
#include "CharacterBase.h"
#include <memory>

class GameScene;
class Player:public CharacterBase
{
public:
	Player();
	~Player();

	void Init(std::shared_ptr<Animation> pAnimation, GameScene* gameScene);
	void End();
	void Update();
	void Draw() const;
	// プレイヤーの当たり判定を取得
	float GetColRadius() const ;
	void OnDamage(int enemyPower);
	void OnDeath();
	// 攻撃
	void OnAttack();
	// 回避
	void OnEvade();
	VECTOR GetAttackPos() { return m_attack.pos; }
	int GetAttackCount() { return m_attack.count; }
	float GetAttackRadius() { return m_attack.radius; }
	bool IsAttackActive() const { return m_attack.active; }
	bool IsDirRight() { return m_isDirRight; }
	bool IsMoving() { return m_isMovingFlag; };
	bool IsDead() { return m_isDead; }
	int GetHp() { return m_hp; }
	bool IsPinch();
	int GetMaxHp();
	void AddPos(VECTOR offset);
private:
	// 移動
	void DoMove();
	void HandleJump();
	void HandleEvade();
	void UpdateCombo();
	void HandleInput();
	struct EvadeData
	{
		int evadeCount; // 回避回数
		bool active; // 回避状態
		float timer; // 回避時間
	};
	EvadeData m_evadeData = { 0,false,0.0f };
	AttackSphere m_attack;
	// 攻撃が出る方向
	bool m_isAttackDirRight;
	VECTOR m_screenPos;
	int m_damageFrame;
	int m_hp;
	// ジャンプ回数
	int m_jumpCount;
	std::shared_ptr<Animation> m_pAnimation;
	GameScene* m_pGameScene = nullptr;
	// プレイヤーの回転行列
	MATRIX m_rotMtx;
	// プレイヤーの向いている方向
	bool m_isDirRight;
	bool m_isJump;
	// 移動前の位置
	VECTOR m_prevPos;
	VECTOR m_rot = { 0.0f,0.0f,0.0f };
	// 移動前と移動後の距離
	float m_distanceAfterMoving;
	// 直前に押されているか
	bool m_isPrevButton;
	// 現在押されているか
	bool m_isNowButton;
	// 移動したかどうか
	bool m_isMovingFlag;
	// 前回向いている方向
	bool m_isPrevDirRight;
	// 現在向いている方向
	bool m_isNowDirRight;
	bool m_isAttackingAnim;
	bool m_isDamageAnim;
	VECTOR m_attackDir;
	bool m_isNowMoving;
	int m_blinkCount; // 点滅時間
	bool m_isVisible; // 描画するか
	// 振り返るだけで移動したと判定させないために
	bool m_isMove = false;
	// 連続で移動アニメーションを呼ばないようにする
	int m_moveCount = 0;
	int m_idleCount = 0;
	bool m_isStartGravity = false;
};

