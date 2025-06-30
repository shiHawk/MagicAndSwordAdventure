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

	void Init(std::shared_ptr<Enemy> pEnemy, std::shared_ptr<Animation> pAnimation);
	void End();
	void Update();
	void Draw() const;
	VECTOR GetScreenPos();
	// エネミーの当たり判定の半径を取得する
	float GetColRadius() const ;
	void OnDamage();
	
	void DoAttack();
	// 回避
	void DoEvade();
	AttackSphere attack = { 0,0,0,30,false,0,0,30 };
private:
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
	std::shared_ptr<Enemy> m_pEnemy;
	std::shared_ptr<Animation> m_pAnimation;
	// プレイヤーの回転行列
	MATRIX m_rotMtx;
	// プレイヤーの向いている方向
	bool m_isDirRight;
	bool m_isJump;

	// 直前に押されているか
	bool m_isPrevButton;
	// 現在押されているか
	bool m_isNowButton;
};

