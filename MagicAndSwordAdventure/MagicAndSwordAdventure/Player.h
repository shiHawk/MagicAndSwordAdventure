#pragma once
#include "DxLib.h"
#include "Enemy.h"
#include <memory>
class Player
{
public:
	struct AttackSphere
	{
		float x, y, z, radius;
		bool active;
		float timer;
		int count;
		int comboDuration;
	};
	Player();
	~Player();

	void Init(std::shared_ptr<Enemy> pEnemy);
	void End();
	void Update();
	void Draw() const;
	VECTOR GetPos() { return m_pos; };
	VECTOR GetScreenPos();
	// エネミーの当たり判定の半径を取得する
	float GetColRadius() const;
	void OnDamage();
	int GetHp() const { return m_hp; }
	void DoAttack();
	// 回避
	void DoEvade();
	AttackSphere attack = { 0,0,0,30,false,0,0,30 };
private:
	struct AnimData 
	{
		int attachNo;		// アタッチ番号
		float count;		// カウンタ
		bool isLoop;	// 終わった時にループするか

		bool isEnd;		// 非ループアニメが終了した
		// true:ループする false:最後のフレームで停止
	};
	AnimData animData = { -1,0.0f,false,false };
	// アニメーションのアタッチ
	void AttachAnime(AnimData& data, const char* animName, bool isLoop);
	void UpdateAnime();
	// 攻撃が出る方向
	bool m_isAttackDirRight;
	int m_handle;
	VECTOR m_pos;
	VECTOR m_vec;
	VECTOR m_screenPos;
	int m_damageFrame;
	int m_hp;
	int m_playerHandle;
	// ジャンプ回数
	int m_jumpCount;
	// 回避回数
	int m_evadeCount;
	std::shared_ptr<Enemy> m_pEnemy;
	// プレイヤーの回転行列
	MATRIX m_rotMtx;
	// プレイヤーの向いている方向
	float m_angle;
	bool m_isJump;
	bool m_isDirRight;
	// 回避状態
	bool m_isEvade;
	bool m_isPrevButton;
	bool m_isNowButton;
	int m_attachIndex;
	float m_animTotalTime;
};

