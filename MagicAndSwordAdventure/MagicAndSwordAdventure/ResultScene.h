#pragma once
#include "SceneBase.h"
#include "DxLib.h"
#include "ScoreManager.h"
#include <memory>
class ResultScene : public SceneBase
{
public:
	ResultScene(std::shared_ptr<ScoreManager> pScoreManager);
	virtual ~ResultScene(){}
	virtual void Init() override;
	virtual void End() override;

	virtual SceneBase* Update() override;
	virtual void Draw() override;
	virtual SceneID GetSceneID() const override;
private:
	std::shared_ptr<ScoreManager> m_pScoreManager;
	int m_resultHandle;
	bool m_isNextScene = false;
	// �J�����̈ʒu
	VECTOR m_cameraPos;
	// �J�����̒����_(���Ă���ꏊ)
	VECTOR m_cameraTarget;
	// �J�����̎���p
	float m_viewAngle;
	int m_fontHandle;
	int m_skeltonIconHandle; // ���j�X�R�A�̃A�C�R���n���h��
	int m_timerIconHandle; // �^�C���{�[�i�X�̃A�C�R���n���h��
 	int m_HPIconHandle; // HP�{�[�i�X�̃A�C�R���n���h��
	// ������̉���
	int m_destroyScoreWidth;
	int m_timeScoreWidth;
	int m_hpScoreWidth;
	int m_totalScoreWidth;
	// �����N�̃n���h��
	int m_rankSHandle; 
	int m_rankAHandle; 
	int m_rankBHandle; 
};

