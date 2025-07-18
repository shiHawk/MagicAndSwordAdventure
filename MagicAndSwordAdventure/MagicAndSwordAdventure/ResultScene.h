#pragma once
#include "SceneBase.h"
#include "DxLib.h"
class ResultScene : public SceneBase
{
public:
	ResultScene();
	virtual ~ResultScene(){}
	virtual void Init() override;
	virtual void End() override;

	virtual SceneBase* Update() override;
	virtual void Draw() override;
private:
	bool m_isNextScene = false;
	// �J�����̈ʒu
	VECTOR m_cameraPos;
	// �J�����̒����_(���Ă���ꏊ)
	VECTOR m_cameraTarget;
	// �J�����̎���p
	float m_viewAngle;
};

