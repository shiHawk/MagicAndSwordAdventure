#pragma once
#include "SceneBase.h"
#include "Player.h"
#include "Enemy.h"
#include "Collision.h"
#include "Animation.h"
#include "NormalSkelton.h"
#include "Camera.h"
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
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Enemy> m_pEnemy;
	std::shared_ptr<Collision> m_pCollision;
	std::shared_ptr<Animation> m_pAnimation;
	std::shared_ptr<NormalSkelton> m_pNormalSkelton;
	std::shared_ptr<Camera> m_pCamera;
};

