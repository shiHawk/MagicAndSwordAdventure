#pragma once
#include "SceneBase.h"
#include "Player.h"
#include "Enemy.h"
#include "Collision.h"
#include "Animation.h"
#include <memory>
#include "DxLib.h"

class Player;
class Enemy;
class Collision;
class GameScene :public SceneBase
{
public:
	GameScene();
	virtual ~GameScene() {}
	virtual void Init() override;
	virtual void End() override;

	virtual SceneBase* Update() override;
	virtual void Draw() override;
private:
	void DrawGrid() const;
private:
	// �J�����̈ʒu
	VECTOR m_cameraPos;
	// �J�����̒����_(���Ă���ꏊ)
	VECTOR m_cameraTarget;
	// �J�����̖ڕW�ړ��ʒu
	VECTOR m_cameraMoveTargetPos;
	// �J�����̎���p
	float m_viewAngle;
	// �J�����ړ����o
	float m_cameraMoveAngle;
	int m_CountDownFrame;
	float m_t;

	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Enemy> m_pEnemy;
	std::shared_ptr<Collision> m_pCollision;
	std::shared_ptr<Animation> m_pAnimation;
};

