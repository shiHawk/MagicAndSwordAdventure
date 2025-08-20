#pragma once
#include "SceneBase.h"
#include "DxLib.h"
#include <memory>
class TitleScene : public SceneBase
{
public:
	TitleScene();
	virtual ~TitleScene() {}
	virtual void Init() override;
	virtual void End() override;

	virtual SceneBase* Update() override;
	virtual void Draw() override;
	virtual SceneID GetSceneID() const override;
private:
	//std::shared_ptr<SoundManager> m_pSoundManager;
	int m_titleHandle;
	int m_titleBGHandle;
	int m_modelHandle;
	float m_time;
	int m_offsetY;
	bool m_isNextScene = false;
	VECTOR m_playerPos;
	// カメラの位置
	VECTOR m_cameraPos;
	// カメラの注視点(見ている場所)
	VECTOR m_cameraTarget;
	// カメラの視野角
	float m_viewAngle;
};

