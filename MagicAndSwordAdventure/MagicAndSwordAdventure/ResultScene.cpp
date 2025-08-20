#include "ResultScene.h"
#include "SoundManager.h"
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
	constexpr int kTimeScorePosY = 260;
	constexpr int kScorePosY = 420;
	// 背景の位置
	constexpr int kBGPosX = -350;
	constexpr int kBGPosY = -150;

	// nearとfarの位置
	constexpr float kCameraNearClip = 10.0f;
	constexpr float kCameraFarClip = 3000.0f;
	// 色
	constexpr int kFontColorWhite = 0xffffff;
	constexpr int kFontColorCyan = 0x00ffff;
	// ボタンの位置
	constexpr int kPressBPosY = 600;
	// 点滅周期
	constexpr int kBlinkCycleMs = 500;
}
ResultScene::ResultScene(std::shared_ptr<ScoreManager> pScoreManager) :
	m_cameraPos({ 0.0f,0.0f,0.0f }),
	m_cameraTarget({ 0.0f,0.0f,0.0f }),
	m_viewAngle(0.0f),
	m_pScoreManager(pScoreManager),
	m_resultHandle(-1),
	m_fontHandle(-1)
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
	SoundManager::GetInstance()->PlayBGM();
	m_resultHandle = LoadGraph("Data/UI/result_seat.png");
	m_fontHandle = CreateFontToHandle(NULL,20,5, DX_FONTTYPE_NORMAL);
}

void ResultScene::End()
{
	DeleteGraph(m_resultHandle);
	SoundManager::GetInstance()->StopBGM();
	m_pScoreManager->End();
}

SceneBase* ResultScene::Update()
{
	UpdateFade();
	SoundManager::GetInstance()->Update();
	if (!m_isNextScene && !IsFadingOut() && Pad::isTrigger(PAD_INPUT_1))
	{
		StartFadeOut();
		m_isNextScene = true;
	}
	if (IsFadingOut())
	{
		SoundManager::GetInstance()->FadeBGMVol();
	}
	if (m_isNextScene && IsFadeComplete())
	{
		
		return new TitleScene();
	}
	return this;
}

void ResultScene::Draw()
{
	DrawGraph(kBGPosX, kBGPosY,m_resultHandle,true);
	DrawFormatStringToHandle(kCharaPosx, kDestroyScorePosY, kFontColorWhite,m_fontHandle,"撃破スコア:%d",m_pScoreManager->GetDestroyScore());
	DrawFormatStringToHandle(kCharaPosx,kTimeScorePosY, kFontColorWhite, m_fontHandle, "タイムボーナス:%d",m_pScoreManager->GetTimeBonus());
	DrawFormatStringToHandle(kCharaPosx,kScorePosY, kFontColorWhite, m_fontHandle, "合計スコア:%d",m_pScoreManager->GetScore());
	if ((int)(GetNowCount() / kBlinkCycleMs) % 2 == 0)
	{
		DrawFormatString(kCharaPosx, kPressBPosY, kFontColorCyan, "Press A Title");
	}
	DrawFade();
}

SceneID ResultScene::GetSceneID() const
{
	return SceneID::ResultScene;
}
