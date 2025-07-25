#include "UIManager.h"
#include <cmath>
namespace
{
	// HP�Q�[�W
	constexpr int kHpGaugeWidth = 400;
	constexpr int kHpGaugeLeft = 80;
	constexpr int kHpGaugeTop = 50;
	constexpr int kHpTextPosY = 30;
	constexpr int kSrcX = 462; // �؂�o���ʒu(X)
	constexpr int kSrcY = 293; // �؂�o���ʒu(Y)
	constexpr unsigned int kFontColorWhite = 0xffffff;// �����̐F
	constexpr unsigned int kHpGaugeColor = 0x00ff00;// �Q�[�W�̐F
	// �i�r�Q�[�V�����̈ʒu
	constexpr int kNavigationPosX = 1100;
	// �X�R�A�̈ʒu
	constexpr int kScorePosX = 600;
	constexpr int kCharaPosY = 40;
	constexpr unsigned int kScoreColor = 0xffdead;
	// �o�ߎ��Ԃ̈ʒu
	constexpr int kTimePosX = 780;
	constexpr unsigned int kTimeColor = 0x008b8b;
	// �c��G���̈ʒu
	constexpr int kEnemyRemainPosX = 960;
	constexpr unsigned int kEnemyRemainColor = 0x4b0082;
	// �_�Ŏ���
	constexpr float kBlinkCycleSeconds = 2.0f;
	constexpr float kFramePerSecond = 60.0f;
	constexpr float kMillisecondsPerSecond = 1000.0f; // �~���b����b�ւ̕ϊ�
}
UIManager::UIManager() :
	m_hpGaugeRate(0.0f),
	m_navigationHandle(-1),
	m_hpGaugeHandle(-1),
	m_playerIconHandle(-1),
	m_playerIconPinchHandle(-1),
	m_blinkTime(0.0f),
	m_blinkProgress(0.0f),
	m_alpha(0)
{
}

UIManager::~UIManager()
{
}

void UIManager::Init(std::shared_ptr<Player> pPlayer, std::shared_ptr<ScoreManager> pScoreManager)
{
	m_pPlayer = pPlayer;
	m_pScoreManager = pScoreManager;
	m_navigationHandle = LoadGraph("Data/UI/Navigation.png");
	m_hpGaugeHandle = LoadGraph("Data/UI/HP.png");
	m_playerIconHandle = LoadGraph("Data/UI/Player_Icon.png");
	m_playerIconPinchHandle = LoadGraph("Data/UI/Player_Icon_Pinch.png");
}

void UIManager::End()
{
	DeleteGraph(m_navigationHandle);
	DeleteGraph(m_hpGaugeHandle);
	DeleteGraph(m_playerIconHandle);
	DeleteGraph(m_playerIconPinchHandle);
}

void UIManager::Update()
{
	m_hpGaugeRate = static_cast<float>(m_pPlayer->GetHp()) / m_pPlayer->GetMaxHp();
}

void UIManager::DrawHp()
{
	if (m_pPlayer->IsPinch())
	{
		DrawGraph(10, 10, m_playerIconPinchHandle, true); // HP��3����1�ȉ��Ȃ�s���`��Ԃ̃A�C�R���ɂ���
	}
	else
	{
		DrawGraph(10, 10, m_playerIconHandle, true);
	}
	DrawRectGraph(kHpGaugeLeft, kHpGaugeTop, kSrcX, kSrcY, static_cast<int>(kHpGaugeWidth * m_hpGaugeRate), kHpTextPosY,m_hpGaugeHandle,true);
	DrawFormatString(kHpGaugeWidth, kHpTextPosY,kFontColorWhite,"%d/%d", m_pPlayer->GetHp(), m_pPlayer->GetMaxHp());
}

void UIManager::DrawNavigation()
{
	m_blinkTime = GetNowCount() / kMillisecondsPerSecond;
	m_blinkProgress = fmod(m_blinkTime, kBlinkCycleSeconds) / kBlinkCycleSeconds; // 0~1�͈̔͂Ő��K��
	m_alpha = static_cast<int>((std::sin(m_blinkProgress * DX_PI * 2) * 0.5f + 0.5f) * 128+128); // 128~255�ɕϊ�(���S�ɂ͓����ɂȂ�Ȃ�)
	// �A���t�@�u�����h���[�h�ݒ�
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha);
	DrawGraph(kNavigationPosX, 0, m_navigationHandle, true);
	// �u�����h���[�h��߂�
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
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
	DrawFormatString(kEnemyRemainPosX, kCharaPosY, kEnemyRemainColor, "����%d��", remainingCount);
}
