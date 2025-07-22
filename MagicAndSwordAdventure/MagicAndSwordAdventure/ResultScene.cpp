#include "ResultScene.h"
#include "TitleScene.h"
#include "Pad.h"
#include "game.h"
namespace
{
	// カメラの位置と注視点
	constexpr VECTOR kCameraPos = { 0.0f,200.0f,-840.0f };
	constexpr VECTOR kCameraTarget = { 0.0f,50.0f,0.0f };
	// カメラの視野角
	constexpr float kViewAngle = 0.447f;
}
ResultScene::ResultScene(std::shared_ptr<ScoreManager> pScoreManager) :
	m_cameraPos({ 0.0f,0.0f,0.0f }),
	m_cameraTarget({ 0.0f,0.0f,0.0f }),
	m_viewAngle(0.0f),
	m_pScoreManager(pScoreManager)
{
}

void ResultScene::Init()
{
	// 3D表示の設定
	SetUseZBuffer3D(true);	  // Zバッファを指定する
	SetWriteZBuffer3D(true);  // Zバッファへの書き込みを行う

	SetUseBackCulling(true);  // ポリゴンの裏面を表示しない

	// カメラの位置の初期化を行う

	// カメラ(始点)の位置
	m_cameraPos= kCameraPos;

	// カメラがどこを見ているか(注視点)
	m_cameraTarget= kCameraTarget;

	// カメラの位置と注視点を指定する
	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);

	// カメラの視野角を設定する
	m_viewAngle = kViewAngle;
	SetupCamera_Perspective(m_viewAngle);

	// カメラのnear,farを設定する
	SetCameraNearFar(10.0f, 3000.0f);

}

void ResultScene::End()
{
	m_pScoreManager->End();
}

SceneBase* ResultScene::Update()
{
	UpdateFade();
	if (!m_isNextScene && !IsFadingOut() && Pad::isTrigger(PAD_INPUT_2))
	{
		StartFadeOut();
		m_isNextScene = true;
	}
	if (m_isNextScene && IsFadeComplete())
	{
		return new TitleScene();
	}
	return this;
}

void ResultScene::Draw()
{
	DrawFormatString(500,100,0xffffff,L"撃破スコア:%d",m_pScoreManager->GetDestroyScore());
	DrawFormatString(500,130,0xffffff,L"タイムボーナス:%d",m_pScoreManager->GetTimeBonus());
	DrawFormatString(500,160,0xffffff,L"合計スコア:%d",m_pScoreManager->GetScore());
	if ((int)(GetNowCount() / 500) % 2 == 0)
	{
		DrawFormatString(500, 260, 0x00ffff, L"Press B Title");
	}
	DrawFade();
}
