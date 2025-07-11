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

	void Init(std::shared_ptr<Animation> pAnimation);
	void End();
	void Update();
	void Draw() const;
	VECTOR GetScreenPos();
	// エネミーの当たり判定の半径を取得する
	float GetColRadius() const ;
	void OnDamage();
	
	// 攻撃
	void DoAttack();
	// 回避
	void DoEvade();
	VECTOR GetAttackPos() { return attack.pos; }
	float GetAttackRadius() { return attack.radius; }
	bool GetDirRight() { return m_isDirRight; }
	bool GetIsMoving();
	AttackSphere attack = { 30,{-500,0,0},false,0,0,30,60.0,40.0};
private:
	// 移動
	void DoMove();
	struct EvadeData
	{
		int evadeCount; // 回避回数
		bool active; // 回避状態
		float timer; // 回避時間
	};
	EvadeData evadeData = { 0,false,0.0f };
	// 攻撃が出る方向
	bool m_isAttackDirRight;
	VECTOR m_screenPos;
	int m_damageFrame;
	int m_hp;
	// ジャンプ回数
	int m_jumpCount;
	std::shared_ptr<Animation> m_pAnimation;
	// プレイヤーの回転行列
	MATRIX m_rotMtx;
	// プレイヤーの向いている方向
	bool m_isDirRight;
	bool m_isJump;
	VECTOR m_playerPosHead;
	VECTOR m_playerPosFoot;
	// 移動前の位置
	VECTOR m_prevPos;
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
};

