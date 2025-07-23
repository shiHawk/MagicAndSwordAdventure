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
	// 文字の位置
	constexpr int kCharaPosx = 500;
	constexpr int kDestroyScorePosY = 100;
	constexpr int kTimeScorePosY = 130;
	constexpr int kScorePosY = 160;

	// nearとfarの位置
	constexpr float kCameraNearClip = 10.0f;
	constexpr float kCameraFarClip = 3000.0f;
	// 色
	constexpr int kFontColorWhite = 0xffffff;
	constexpr int kFontColorCyan = 0x00ffff;
	// ボタンの位置
	constexpr int kPressBPosY = 260;
	// 点滅周期
	constexpr int kBlinkCycleMs = 500;
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
	SetCameraNearFar(kCameraNearClip, kCameraFarClip);

}

void ResultScene::End()
{
	m_pScoreManager->End();
}

SceneBase* ResultScene::Update()
{
	UpdateFade();
	if (!m_isNextScene && !IsFadingOut() && Pad::isTrigger(PAD_INPUT_1))
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
	DrawFormatString(kCharaPosx, kDestroyScorePosY, kFontColorWhite,"撃破スコア:%d",m_pScoreManager->GetDestroyScore());
	DrawFormatString(kCharaPosx,kTimeScorePosY, kFontColorWhite,"タイムボーナス:%d",m_pScoreManager->GetTimeBonus());
	DrawFormatString(kCharaPosx,kScorePosY, kFontColorWhite,"合計スコア:%d",m_pScoreManager->GetScore());
	if ((int)(GetNowCount() / kBlinkCycleMs) % 2 == 0)
	{
		DrawFormatString(kCharaPosx, kPressBPosY, kFontColorCyan, "Press A Title");
	}
	DrawFade();
}
