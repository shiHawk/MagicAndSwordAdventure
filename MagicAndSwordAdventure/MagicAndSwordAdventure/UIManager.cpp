#include "UIManager.h"
#include <cmath>

namespace
{
	// HP�Q�[�W
	constexpr int kHpGaugeWidth = 400;
	constexpr int kHpGaugeLeft = 80;
	constexpr int kHpGaugeTop = 50;
	constexpr int kHpTextPosY = 14;
	constexpr int kSrcX = 462; // �؂�o���ʒu(X)
	constexpr int kSrcY = 293; // �؂�o���ʒu(Y)
	constexpr int kHpGaugeFrameWidth = 400; // �Q�[�W�̉���
	constexpr unsigned int kHpGaugeColor = 0x00ff00;// �Q�[�W�̐F
	// �i�r�Q�[�V�����̈ʒu
	constexpr int kNavigationPosX = 1100;
	// �X�R�A�̈ʒu
	constexpr int kScorePosX = 650;
	constexpr int kCharaTextPosY = 20;
	constexpr int kCharaPosY = 45;
	constexpr unsigned int kScoreColor = 0xffdead;
	// �o�ߎ��Ԃ̈ʒu
	constexpr int kTimePosX = 830;
	constexpr unsigned int kTimeColor = 0xff4500;
	// �c��G���̈ʒu
	constexpr int kEnemyRemainPosX = 1000;
	constexpr unsigned int kEnemyRemainColor = 0xff4500;
	// �A�C�R���̈ʒu
	constexpr int kIconPos = 10;
	// �_�Ŏ���
	constexpr float kBlinkCycleSeconds = 2.0f;
	constexpr float kFramePerSecond = 60.0f;
	constexpr float kMillisecondsPerSecond = 1000.0f; // �~���b����b�ւ̕ϊ�
	// �t�H���g�̃T�C�Y�A����
	constexpr int kFontTextSize = 20;
	constexpr int kFontSize = 30;
	constexpr int kFontThick = 5;

	constexpr float kSinAmplitudeScale = 0.5f;        // sin�g�̃X�P�[�����O (-1.0 �� -0.5)
	constexpr float kSinOffset = 0.5f;                // sin�g�̃I�t�Z�b�g (-0.5 �� +0.5)
	constexpr int kBlinkAlphaRange = 128;             // �����x�̕ω��͈�
	constexpr int kBlinkAlphaBase = 128;              // �ŏ������x(���S�ɂ͓����ɂȂ�Ȃ�)
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
	m_fontTextHandle = CreateFontToHandle("Noto Serif JP Black", kFontTextSize, kFontThick, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	m_fontHandle = CreateFontToHandle("Noto Serif JP Black", kFontSize, kFontThick, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
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
	m_hpGaugeRate = static_cast<float>(m_pPlayer->GetHp()) / m_pPlayer->GetMaxHp(); // �c��HP�̊������X�V
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
	DrawRectGraph(kHpGaugeLeft, kHpGaugeTop, kSrcX, kSrcY, kHpGaugeFrameWidth, kHpTextPosY, m_hpGaugeFrameHandle, true);
	if (m_pPlayer->IsPinch())
	{
		DrawGraph(kIconPos, kIconPos, m_playerIconPinchHandle, true); // HP��3����1�ȉ��Ȃ�s���`��Ԃ̃A�C�R���ɂ���
		// HP�o�[�̐F��ς���
		DrawRectGraph(kHpGaugeLeft, kHpGaugeTop, kSrcX, kSrcY, static_cast<int>(kHpGaugeWidth * m_hpGaugeRate), kHpTextPosY, m_playerHpGaugePinchHandle, true);
	}
	else
	{ 
		DrawGraph(kIconPos, kIconPos, m_playerIconHandle, true); // �v���C���[�̃A�C�R���̕`��
		DrawRectGraph(kHpGaugeLeft, kHpGaugeTop, kSrcX, kSrcY, static_cast<int>(kHpGaugeWidth * m_hpGaugeRate), kHpTextPosY, m_hpGaugeHandle, true); // HP�o�[�̕`��
	}
	DrawFormatStringToHandle(kHpGaugeWidth, kHpTextPosY, kScoreColor, m_fontHandle,"%d/%d", m_pPlayer->GetHp(), m_pPlayer->GetMaxHp());
}

void UIManager::DrawNavigation()
{
	m_blinkTime = GetNowCount() / kMillisecondsPerSecond;
	m_blinkProgress = fmod(m_blinkTime, kBlinkCycleSeconds) / kBlinkCycleSeconds; // 0~1�͈̔͂Ő��K��
	m_alpha = static_cast<int>((std::sin(m_blinkProgress * DX_PI * 2) * kSinAmplitudeScale + kSinOffset) * kBlinkAlphaRange + kBlinkAlphaBase); // 128~255�ɕϊ�(���S�ɂ͓����ɂȂ�Ȃ�)
	// �A���t�@�u�����h���[�h�ݒ�
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha);
	DrawGraph(kNavigationPosX, 0, m_navigationHandle, true);
	// �u�����h���[�h��߂�
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
	DrawFormatStringToHandle(kEnemyRemainPosX, kCharaTextPosY, kEnemyRemainColor, m_fontTextHandle, "����");
	DrawFormatStringToHandle(kEnemyRemainPosX, kCharaPosY, kEnemyRemainColor, m_fontHandle, "%d��", remainingCount);
}

void UIManager::DrawUIFrame()
{
	DrawRectGraph(620,15,208,180,500,80,m_uiFrameHandle,true);
}
