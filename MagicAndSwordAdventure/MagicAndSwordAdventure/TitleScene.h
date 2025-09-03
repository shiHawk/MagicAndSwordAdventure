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
	int m_titleHandle;
	int m_titleBGHandle;
	int m_BGHandle;
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
	int m_fontHandle;
	int m_manualHandle;
	bool m_isManualFlag; // 操作説明を表示するか
	bool m_isPlayingMovie;
};

