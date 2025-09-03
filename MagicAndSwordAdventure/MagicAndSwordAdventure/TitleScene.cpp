#include "TitleScene.h"
#include "SoundManager.h"
#include "DxLib.h"
#include "GameScene.h"
#include "Pad.h"
#include "game.h"
#include <cmath>

namespace
{
	constexpr int kMaxFadeBright = 255;
	// フェード速度
	constexpr int kFadeSpeed = 8;
	// タイトルの座標
	constexpr int kTitlePosX = 400;
	constexpr int kTitlePosY = 75;
	constexpr int kTitleSize = 512;

	// カメラの位置と注視点
	constexpr VECTOR kCameraPos = { 0.0f,200.0f,-840.0f };
	constexpr VECTOR kCameraTarget = { 0.0f,50.0f,0.0f };
	// カメラの視野角
	constexpr float kViewAngle = 0.447f;

	// PressAの文字の位置のポジション
	constexpr int kPressAPosX = 570;
	constexpr int kPressAPosY = 600;
	// GameStartの文字の位置のポジション
	constexpr int kStartPosX = 570;
	constexpr int kStartPosY = 660;

	constexpr float kTitleBobFrequency = 2.0f; // タイトルロゴの上下揺れ周期(Hz)
	constexpr float kTitleBobAmplitude = 10.0f; // タイトルロゴの上下揺れ振幅(px)
	constexpr int   kMillisecondsPerSecond = 1000; // ミリ秒→秒換算用

	// nearとfarの位置
	constexpr float kCameraNearClip = 10.0f;
	constexpr float kCameraFarClip = 3000.0f;
	// 点滅周期
	constexpr int kBlinkCycleMs = 500;
	constexpr float kModelScale = 50.0f; // モデルのスケール
}

TitleScene::TitleScene():
	m_cameraPos({0.0f,0.0f,0.0f}),
	m_cameraTarget({0.0f,0.0f,0.0f}),
	m_viewAngle(0.0f),
	m_titleHandle(-1),
	m_time(0.0f),
	m_offsetY(0),
	m_titleBGHandle(-1),
	m_BGHandle(-1),
	m_modelHandle(-1),
	m_playerPos({ 0.0f,0.0f,0.0f }),
	m_fontHandle(-1),
	m_manualHandle(-1),
	m_isManualFlag(false),
	m_isPlayingMovie(true)
{
}

void TitleScene::Init()
{
	m_fadeBright = 0;
	m_fadeSpeed = -kFadeSpeed;
	m_modelHandle = MV1LoadModel("Data/model/Barbarian.mv1");
	m_playerPos = { 0.0f,0.0f,0.0f };
	MV1SetScale(m_modelHandle, VGet(kModelScale, kModelScale, kModelScale));
	MV1SetPosition(m_modelHandle, m_playerPos);
	m_isManualFlag = false;

	// 3D表示の設定
	SetUseZBuffer3D(true);	  // Zバッファを指定する
	SetWriteZBuffer3D(true);  // Zバッファへの書き込みを行う

	SetUseBackCulling(true);  // ポリゴンの裏面を表示しない

	// カメラの位置の初期化を行う

	// カメラ(始点)の位置
	m_cameraPos = kCameraPos;

	// カメラがどこを見ているか(注視点)
	m_cameraTarget = kCameraTarget;

	// カメラの位置と注視点を指定する
	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);

	// カメラの視野角を設定する
	m_viewAngle = kViewAngle;
	SetupCamera_Perspective(m_viewAngle);

	// カメラのnear,farを設定する
	SetCameraNearFar(kCameraNearClip, kCameraFarClip);
	SoundManager::GetInstance()->PlayBGM();
	m_titleHandle = LoadGraph("Data/title/WarriorAdventureTitle.png");
	m_titleBGHandle = LoadGraph("Data/title/TitleBG.png");
	m_manualHandle = LoadGraph("Data/title/WarriorAdventure_manual.png");
	m_fontHandle = CreateFontToHandle("Arial Black", 20, 3, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	m_BGHandle = LoadGraph("Data/title/title_BG.mp4");
	m_isPlayingMovie = true;
}

void TitleScene::End()
{
	DeleteGraph(m_titleHandle);
	DeleteGraph(m_titleBGHandle);
	MV1DeleteModel(m_modelHandle);
	MV1DeleteModel(m_manualHandle);
	DeleteFontToHandle(m_fontHandle);
	DeleteGraph(m_BGHandle);
	SoundManager::GetInstance()->StopBGM();
}

SceneBase* TitleScene::Update()
{
	UpdateFade();
	SoundManager::GetInstance()->Update();
	m_time = GetNowCount() / kMillisecondsPerSecond;
	if (GetMovieStateToGraph(m_BGHandle) == 0)
	{
		SeekMovieToGraph(m_BGHandle, 0);
		PlayMovieToGraph(m_BGHandle);
	}
	PlayMovieToGraph(m_BGHandle);
	
	// タイトルロゴが上下するための位置補正
	m_offsetY = static_cast<int>(sin(m_time * kTitleBobFrequency) * kTitleBobAmplitude);
	// Aボタンを押したらフェードを開始
	if (!m_isNextScene && !IsFadingOut() && Pad::isTrigger(PAD_INPUT_1))
	{
		if (!m_isManualFlag)
		{
			// 最初のAボタンで操作説明を表示
			m_isManualFlag = true;
		}
		else
		{
			// 2回目のAボタンでフェード開始
			StartFadeOut();
			m_isNextScene = true;
		}
	}
	if (IsFadingOut())
	{
		SoundManager::GetInstance()->FadeBGMVol();
	}
	// フェードが終了したら遷移する
	if (m_isNextScene && IsFadeComplete())
	{
		return new GameScene();
	}
	return this;
}

void TitleScene::Draw()
{	
	DrawGraph(0, 0, m_BGHandle, false);
	// 背景を描画
	//DrawGraph(0,0, m_titleBGHandle,false);
	// タイトルロゴを拡大表示
	DrawExtendGraph(kTitlePosX, kTitlePosY + m_offsetY, kTitlePosX+ kTitleSize, kTitlePosY+ +m_offsetY+kTitleSize, m_titleHandle, true);
	if (m_isManualFlag)
	{
		DrawGraph(0, 0, m_manualHandle, false);
	}
	// 点滅させる
	if ((int)(GetNowCount() / kBlinkCycleMs) % 2 == 0)
	{
		if (!m_isManualFlag)
		{
			DrawFormatStringToHandle(kPressAPosX, kPressAPosY, 0x00ffff, m_fontHandle, "Press A Button");
		}
		else
		{
			DrawFormatStringToHandle(kStartPosX, kStartPosY, 0x00ffff, m_fontHandle, "Press A Start");
		}
	}
	
	DrawFade();
}

SceneID TitleScene::GetSceneID() const
{
	return SceneID::TitleScene;
}
