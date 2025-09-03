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
	constexpr int kCharaPosX = 500;
	constexpr int kDestroyScorePosY = 100; // 撃破スコア
	constexpr int kTimeScorePosY = 260; // タイムボーナス
	constexpr int kHpScorePosY = 420; // HPボーナス
	constexpr int kScorePosY = 580; // 合計スコア
	// 背景の位置
	constexpr int kBGPosX = -350;
	constexpr int kBGPosY = -150;

	// nearとfarの位置
	constexpr float kCameraNearClip = 10.0f;
	constexpr float kCameraFarClip = 3000.0f;
	// 色
	constexpr int kFontColorWhite = 0xffffff;
	constexpr int kFontColorBlue = 0x00bfff;
	constexpr int kFontColorRed = 0xff6347;
	// ボタンの位置
	constexpr int kPressBPosY = 660;
	// 点滅周期
	constexpr int kBlinkCycleMs = 500;
	// ランク判定の基準スコア
	constexpr int kRankScoreS = 23500;  // Sランク以上
	constexpr int kRankScoreA = 16950;  // Aランク以上
	constexpr int kRankScoreB = 0;      // Bランク以上
}
ResultScene::ResultScene(std::shared_ptr<ScoreManager> pScoreManager) :
	m_cameraPos({ 0.0f,0.0f,0.0f }),
	m_cameraTarget({ 0.0f,0.0f,0.0f }),
	m_viewAngle(0.0f),
	m_pScoreManager(pScoreManager),
	m_resultHandle(-1),
	m_fontHandle(-1),
	m_skeltonIconHandle(-1),
	m_timerIconHandle(-1),
	m_HPIconHandle(-1),
	m_rankSHandle(-1),
	m_rankAHandle(-1),
	m_rankBHandle(-1)
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
	m_fontHandle = CreateFontToHandle("Arial Black", 30, 5, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	m_skeltonIconHandle = LoadGraph("Data/UI/skeltonIcon.png");
	m_timerIconHandle = LoadGraph("Data/UI/timerIcon.png");
	m_HPIconHandle = LoadGraph("Data/UI/hpIcon.png");
	m_rankSHandle = LoadGraph("Data/UI/rank_s.png");
	m_rankAHandle = LoadGraph("Data/UI/rank_a.png");
	m_rankBHandle = LoadGraph("Data/UI/rank_b.png");
}

void ResultScene::End()
{
	DeleteGraph(m_resultHandle);
	DeleteGraph(m_skeltonIconHandle);
	DeleteGraph(m_timerIconHandle);
	DeleteGraph(m_HPIconHandle);
	DeleteGraph(m_rankSHandle);
	DeleteGraph(m_rankAHandle);
	DeleteGraph(m_rankBHandle);
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
	DrawGraph(440, 95,m_skeltonIconHandle,true);
	DrawGraph(440, 255,m_timerIconHandle,true);
	DrawGraph(440, 415,m_HPIconHandle,true);
	DrawFormatStringToHandle(kCharaPosX, kDestroyScorePosY, kFontColorWhite,m_fontHandle,"Destroy Score: %d",m_pScoreManager->GetDestroyScore());
	DrawFormatStringToHandle(kCharaPosX,kTimeScorePosY, kFontColorWhite, m_fontHandle, "Time Bonus: %d",m_pScoreManager->GetTimeBonus());
	DrawFormatStringToHandle(kCharaPosX, kHpScorePosY, kFontColorWhite, m_fontHandle, "HP Bonus: %d",m_pScoreManager->GetHpBonus());
	DrawFormatStringToHandle(kCharaPosX,kScorePosY, kFontColorRed, m_fontHandle, "Total Score: %d",m_pScoreManager->GetScore());
	if ((int)(GetNowCount() / kBlinkCycleMs) % 2 == 0)
	{
		DrawFormatStringToHandle(kCharaPosX, kPressBPosY, kFontColorBlue, m_fontHandle,"Press A Title");
	}
	
	if (m_pScoreManager->GetScore() >= kRankScoreS)
	{
		DrawGraph(840, 170, m_rankSHandle, true);
	}
	else if (m_pScoreManager->GetScore() > kRankScoreA)
	{
		DrawGraph(840, 170, m_rankAHandle, true);
	}
	else
	{
		DrawGraph(840, 170, m_rankBHandle, true);
	}
	DrawFade();
}

SceneID ResultScene::GetSceneID() const
{
	return SceneID::ResultScene;
}
