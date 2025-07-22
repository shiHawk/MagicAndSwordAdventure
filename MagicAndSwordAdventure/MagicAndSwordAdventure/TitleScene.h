#pragma once
#include "SceneBase.h"
#include "DxLib.h"
class TitleScene : public SceneBase
{
public:
	TitleScene();
	virtual ~TitleScene() {}
	virtual void Init() override;
	virtual void End() override;

	virtual SceneBase* Update() override;
	virtual void Draw() override;
private:
	int m_titleHandle;
	int m_titleBGHandle;
	float m_time;
	int m_offsetY;
	bool m_isNextScene = false;
	// カメラの位置
	VECTOR m_cameraPos;
	// カメラの注視点(見ている場所)
	VECTOR m_cameraTarget;
	// カメラの視野角
	float m_viewAngle;
};

