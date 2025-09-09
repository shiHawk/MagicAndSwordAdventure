#include "UIManager.h"
#include <cmath>

namespace
{
	// HPゲージ
	constexpr int kHpGaugeWidth = 400;
	constexpr int kHpGaugeLeft = 80;
	constexpr int kHpGaugeTop = 50;
	constexpr int kHpTextPosY = 14;
	constexpr int kSrcX = 462; // 切り出し位置(X)
	constexpr int kSrcY = 293; // 切り出し位置(Y)
	constexpr unsigned int kHpGaugeColor = 0x00ff00;// ゲージの色
	// ナビゲーションの位置
	constexpr int kNavigationPosX = 1100;
	// スコアの位置
	constexpr int kScorePosX = 650;
	constexpr int kCharaTextPosY = 20;
	constexpr int kCharaPosY = 45;
	constexpr unsigned int kScoreColor = 0xffdead;
	// 経過時間の位置
	constexpr int kTimePosX = 830;
	constexpr unsigned int kTimeColor = 0xff4500;
	// 残り敵数の位置
	constexpr int kEnemyRemainPosX = 1000;
	constexpr unsigned int kEnemyRemainColor = 0xff4500;
	// 点滅周期
	constexpr float kBlinkCycleSeconds = 2.0f;
	constexpr float kFramePerSecond = 60.0f;
	constexpr float kMillisecondsPerSecond = 1000.0f; // ミリ秒から秒への変換

}
UIManager::UIManager() :
	m_hpGaugeRate(0.0f),
	m_navigationHandle(-1),
	m_hpGaugeHandle(-1),
	m_hpGaugeFrameHandle(-1),
	m_playerIconHandle(-1),
	m_playerIconPinchHandle(-1),
	m_blinkTime(0.0f),
	m_blinkProgress(0.0f),
	m_alpha(0),
	m_playerHpGaugePinchHandle(-1),
	m_fontTextHandle(-1),
	m_fontHandle(-1),
	m_uiFrameHandle(-1)
{
}

UIManager::~UIManager()
{
}

void UIManager::Init(std::shared_ptr<Player> pPlayer, std::shared_ptr<ScoreManager> pScoreManager, std::shared_ptr<Camera> pCamera)
{
	m_pPlayer = pPlayer;
	m_pScoreManager = pScoreManager;
	m_pCamera = pCamera;
	m_navigationHandle = LoadGraph("Data/UI/Navigation.png");
	m_hpGaugeHandle = LoadGraph("Data/UI/HP.png");
	m_hpGaugeFrameHandle = LoadGraph("Data/UI/HPGaugeFrame.png");
	m_playerIconHandle = LoadGraph("Data/UI/Player_Icon.png");
	m_playerIconPinchHandle = LoadGraph("Data/UI/Player_Icon_Pinch.png");
	m_playerHpGaugePinchHandle = LoadGraph("Data/UI/HP_pinch.png");
	m_fontTextHandle = CreateFontToHandle("Noto Serif JP Black", 20, 5, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	m_fontHandle = CreateFontToHandle("Noto Serif JP Black",30, 5, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	m_uiFrameHandle = LoadGraph("Data/UI/UI_Frame.png");
	
}

void UIManager::End()
{
	DeleteGraph(m_navigationHandle);
	DeleteGraph(m_hpGaugeHandle);
	DeleteGraph(m_hpGaugeFrameHandle);
	DeleteGraph(m_playerIconHandle);
	DeleteGraph(m_playerIconPinchHandle);
	DeleteGraph(m_playerHpGaugePinchHandle);
	DeleteFontToHandle(m_fontHandle);
	DeleteFontToHandle(m_fontTextHandle);
}

void UIManager::Update()
{
	m_hpGaugeRate = static_cast<float>(m_pPlayer->GetHp()) / m_pPlayer->GetMaxHp();
}

void UIManager::Draw()
{
	DrawUIFrame();
	DrawHp();
	if (!m_pCamera->IsBattleCamera())
	{
		DrawNavigation();
	}
	DrawDestroyScore();
	DrawElapsedTimeSeconds();
}

void UIManager::DrawHp()
{
	DrawRectGraph(kHpGaugeLeft, kHpGaugeTop, kSrcX, kSrcY, 400, kHpTextPosY, m_hpGaugeFrameHandle, true);
	if (m_pPlayer->IsPinch())
	{
		DrawGraph(10, 10, m_playerIconPinchHandle, true); // HPが3分の1以下ならピンチ状態のアイコンにする
		// HPバーの色を変える
		DrawRectGraph(kHpGaugeLeft, kHpGaugeTop, kSrcX, kSrcY, static_cast<int>(kHpGaugeWidth * m_hpGaugeRate), kHpTextPosY, m_playerHpGaugePinchHandle, true);
	}
	else
	{
		SetDrawBright(255, 255, 255); 
		DrawGraph(10, 10, m_playerIconHandle, true);
		DrawRectGraph(kHpGaugeLeft, kHpGaugeTop, kSrcX, kSrcY, static_cast<int>(kHpGaugeWidth * m_hpGaugeRate), kHpTextPosY, m_hpGaugeHandle, true);
	}
	DrawFormatStringToHandle(kHpGaugeWidth, kHpTextPosY, kScoreColor, m_fontHandle,"%d/%d", m_pPlayer->GetHp(), m_pPlayer->GetMaxHp());
	SetDrawBright(255, 255, 255);
}

void UIManager::DrawNavigation()
{
	m_blinkTime = GetNowCount() / kMillisecondsPerSecond;
	m_blinkProgress = fmod(m_blinkTime, kBlinkCycleSeconds) / kBlinkCycleSeconds; // 0~1の範囲で正規化
	m_alpha = static_cast<int>((std::sin(m_blinkProgress * DX_PI * 2) * 0.5f + 0.5f) * 128+128); // 128~255に変換(完全には透明にならない)
	// アルファブレンドモード設定
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha);
	DrawGraph(kNavigationPosX, 0, m_navigationHandle, true);
	// ブレンドモードを戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void UIManager::DrawDestroyScore()
{
	DrawFormatStringToHandle(kScorePosX, kCharaTextPosY, kScoreColor, m_fontTextHandle, "Score");
	DrawFormatStringToHandle(kScorePosX, kCharaPosY, kScoreColor, m_fontHandle, "%d",m_pScoreManager->GetDestroyScore());
}

void UIManager::DrawElapsedTimeSeconds()
{
	DrawFormatStringToHandle(kTimePosX, kCharaTextPosY, kTimeColor, m_fontTextHandle, "Time");
	DrawFormatStringToHandle(kTimePosX, kCharaPosY, kTimeColor, m_fontHandle, "%d",m_pScoreManager->GetTime());
}

void UIManager::DrawNumberOfEnemiesRemaining(int remainingCount)
{
	DrawFormatStringToHandle(kEnemyRemainPosX, kCharaTextPosY, kEnemyRemainColor, m_fontTextHandle, "あと");
	DrawFormatStringToHandle(kEnemyRemainPosX, kCharaPosY, kEnemyRemainColor, m_fontHandle, "%d体", remainingCount);
}

void UIManager::DrawUIFrame()
{
	DrawRectGraph(620,15,208,180,500,80,m_uiFrameHandle,true);
}
