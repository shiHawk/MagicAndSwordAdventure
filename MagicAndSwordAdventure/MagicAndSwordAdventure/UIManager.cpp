#include "UIManager.h"

UIManager::UIManager() :
	m_hpGaugeRate(0.0f)
{
}

UIManager::~UIManager()
{
}

void UIManager::Init(std::shared_ptr<Player> pPlayer)
{
	m_pPlayer = pPlayer;
}

void UIManager::End()
{
}

void UIManager::Update()
{
	m_hpGaugeRate = static_cast<float>(m_pPlayer->GetHp()) / m_pPlayer->GetMaxHp();
}

void UIManager::Draw()
{
	DrawBox(80, 50, 80 + 100 * m_hpGaugeRate, 80, 0x00ff00, true);
}
