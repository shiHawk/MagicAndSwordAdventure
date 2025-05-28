#pragma once
#include "SceneBase.h"
#include <memory>
#include "DxLib.h"
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
	// �J�����̎���p
	float m_viewAngle;
	// �J�����ړ����o
	float m_cameraMoveAngle;
	int m_CountDownFrame;
};

