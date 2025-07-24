#include "UIManager.h"
namespace
{
	// HPゲージ
	constexpr int kHpGaugeWidth = 400;
	constexpr int kHpGaugeLeft = 80;
	constexpr int kHpGaugeTop = 50;
	constexpr int kHpGaugeBottom = 80;
	constexpr int kHpTextPosY = 30;
	constexpr unsigned int kFontColorWhite = 0xffffff;// 文字の色
	constexpr unsigned int kHpGaugeColor = 0x00ff00;// ゲージの色
	// ナビゲーションの位置
	constexpr int kNavigationPosX = 1100;
	// スコアの位置
	constexpr int kScorePosX = 600;
	constexpr int kCharaPosY = 40;
	constexpr unsigned int kScoreColor = 0xffdead;
	// 経過時間の位置
	constexpr int kTimePosX = 780;
	constexpr unsigned int kTimeColor = 0x008b8b;
	// 残り敵数の位置
	constexpr int kEnemyRemainPosX = 960;
	constexpr unsigned int kEnemyRemainColor = 0x4b0082;
}
UIManager::UIManager() :
	m_hpGaugeRate(0.0f),
	m_drawNavigationHandle(-1),
	m_hpGaugeHandle(-1)
{
}

UIManager::~UIManager()
{
}

void UIManager::Init(std::shared_ptr<Player> pPlayer, std::shared_ptr<ScoreManager> pScoreManager)
{
	m_pPlayer = pPlayer;
	m_pScoreManager = pScoreManager;
	m_drawNavigationHandle = LoadGraph("Data/UI/Navigation.png");
	m_hpGaugeHandle = LoadGraph("Data/UI/HP.png");
}

void UIManager::End()
{
	DeleteGraph(m_drawNavigationHandle);
	DeleteGraph(m_hpGaugeHandle);
}

void UIManager::Update()
{
	m_hpGaugeRate = static_cast<float>(m_pPlayer->GetHp()) / m_pPlayer->GetMaxHp();
}

void UIManager::DrawHp()
{
	DrawRectGraph(80,50, 462,293, static_cast<int>(400 * m_hpGaugeRate),30,m_hpGaugeHandle,true);
	DrawFormatString(kHpGaugeWidth, kHpTextPosY,kFontColorWhite,"%d/%d", m_pPlayer->GetHp(), m_pPlayer->GetMaxHp());
	//DrawBox(kHpGaugeLeft, kHpGaugeTop, kHpGaugeLeft + kHpGaugeWidth * m_hpGaugeRate, kHpGaugeBottom, kHpGaugeColor, true);
}

void UIManager::DrawNavigation()
{
	DrawGraph(kNavigationPosX, 0, m_drawNavigationHandle, true);
}

void UIManager::DrawDestroyScore()
{
	DrawFormatString(kScorePosX, kCharaPosY, kScoreColor,"Score\n%d",m_pScoreManager->GetDestroyScore());
}

void UIManager::DrawmElapsedTimeSeconds()
{
	DrawFormatString(kTimePosX, kCharaPosY, kTimeColor,"Time\n%d",m_pScoreManager->GetTime());
}

void UIManager::DrawNumberOfEnemiesRemaining(int remainingCount)
{
	DrawFormatString(kEnemyRemainPosX, kCharaPosY, kEnemyRemainColor, "あと%d体", remainingCount);
}
