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
	// カメラの位置
	VECTOR m_cameraPos;
	// カメラの注視点(見ている場所)
	VECTOR m_cameraTarget;
	// カメラの視野角
	float m_viewAngle;
	int m_fontHandle;
	int m_skeltonIconHandle; // 撃破スコアのアイコンハンドル
	int m_timerIconHandle; // タイムボーナスのアイコンハンドル
 	int m_HPIconHandle; // HPボーナスのアイコンハンドル
	// 文字列の横幅
	int m_destroyScoreWidth;
	int m_timeScoreWidth;
	int m_hpScoreWidth;
	int m_totalScoreWidth;
	// ランクのハンドル
	int m_rankSHandle; 
	int m_rankAHandle; 
	int m_rankBHandle; 
};

