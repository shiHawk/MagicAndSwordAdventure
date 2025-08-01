#pragma once
#include "SceneBase.h"
#include "SoundManager.h"
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
	std::shared_ptr<SoundManager> m_pSoundManager;
	int m_titleHandle;
	int m_titleBGHandle;
	float m_time;
	int m_offsetY;
	bool m_isNextScene = false;
	// �J�����̈ʒu
	VECTOR m_cameraPos;
	// �J�����̒����_(���Ă���ꏊ)
	VECTOR m_cameraTarget;
	// �J�����̎���p
	float m_viewAngle;
};

