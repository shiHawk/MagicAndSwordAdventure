#include "UIManager.h"

UIManager::UIManager() :
	m_hpGaugeRate(0.0f),
	m_drawNavigationHandle(-1)
{
}

UIManager::~UIManager()
{
}

void UIManager::Init(std::shared_ptr<Player> pPlayer, std::shared_ptr<ScoreManager> pScoreManager)
{
	m_pPlayer = pPlayer;
	m_pScoreManager = pScoreManager;
	m_drawNavigationHandle = LoadGraph(L"Data/UI/Navigation.png");
}

void UIManager::End()
{
}

void UIManager::Update()
{
	m_hpGaugeRate = static_cast<float>(m_pPlayer->GetHp()) / m_pPlayer->GetMaxHp();
}

void UIManager::DrawHp()
{
	DrawBox(80, 50, 80 + 400 * m_hpGaugeRate, 80, 0x00ff00, true);
}

void UIManager::DrawNavigation()
{
	DrawGraph(1100, 0, m_drawNavigationHandle, true);
}

void UIManager::DrawDestroyScore()
{
	DrawFormatString(600,40,0xffdead,L"Score\n%d",m_pScoreManager->GetDestroyScore());
}

void UIManager::DrawmElapsedTimeSeconds()
{
	DrawFormatString(780,40,0x008b8b,L"Time\n%d",m_pScoreManager->GetTime());
}

void UIManager::DrawNumberOfEnemiesRemaining(int remainingCount)
{
	DrawFormatString(960, 40, 0xee82ee, L"‚ ‚Æ%d‘Ì", remainingCount);
}
