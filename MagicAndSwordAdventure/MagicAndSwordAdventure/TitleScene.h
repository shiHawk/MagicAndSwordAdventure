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
	// �J�����̈ʒu
	VECTOR m_cameraPos;
	// �J�����̒����_(���Ă���ꏊ)
	VECTOR m_cameraTarget;
	// �J�����̎���p
	float m_viewAngle;
	int m_fontHandle;
	int m_manualHandle;
	bool m_isManualFlag; // ���������\�����邩
	bool m_isPlayingMovie;
};

