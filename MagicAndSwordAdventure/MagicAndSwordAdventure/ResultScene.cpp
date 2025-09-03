#include "ResultScene.h"
#include "SoundManager.h"
#include "TitleScene.h"
#include "Pad.h"
#include "game.h"
namespace
{
	// �J�����̈ʒu�ƒ����_
	constexpr VECTOR kCameraPos = { 0.0f,200.0f,-840.0f };
	constexpr VECTOR kCameraTarget = { 0.0f,50.0f,0.0f };
	// �J�����̎���p
	constexpr float kViewAngle = 0.447f;
	// �����̈ʒu
	constexpr int kCharaPosX = 500;
	constexpr int kDestroyScorePosY = 100; // ���j�X�R�A
	constexpr int kTimeScorePosY = 260; // �^�C���{�[�i�X
	constexpr int kHpScorePosY = 420; // HP�{�[�i�X
	constexpr int kScorePosY = 580; // ���v�X�R�A
	// �w�i�̈ʒu
	constexpr int kBGPosX = -350;
	constexpr int kBGPosY = -150;

	// near��far�̈ʒu
	constexpr float kCameraNearClip = 10.0f;
	constexpr float kCameraFarClip = 3000.0f;
	// �F
	constexpr int kFontColorWhite = 0xffffff;
	constexpr int kFontColorBlue = 0x00bfff;
	constexpr int kFontColorRed = 0xff6347;
	// �{�^���̈ʒu
	constexpr int kPressBPosY = 660;
	// �_�Ŏ���
	constexpr int kBlinkCycleMs = 500;
	// �����N����̊�X�R�A
	constexpr int kRankScoreS = 23500;  // S�����N�ȏ�
	constexpr int kRankScoreA = 16950;  // A�����N�ȏ�
	constexpr int kRankScoreB = 0;      // B�����N�ȏ�
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
	// 3D�\���̐ݒ�
	SetUseZBuffer3D(true);	  // Z�o�b�t�@���w�肷��
	SetWriteZBuffer3D(true);  // Z�o�b�t�@�ւ̏������݂��s��

	SetUseBackCulling(true);  // �|���S���̗��ʂ�\�����Ȃ�

	// �J�����̈ʒu�̏��������s��

	// �J����(�n�_)�̈ʒu
	m_cameraPos= kCameraPos;

	// �J�������ǂ������Ă��邩(�����_)
	m_cameraTarget= kCameraTarget;

	// �J�����̈ʒu�ƒ����_���w�肷��
	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);

	// �J�����̎���p��ݒ肷��
	m_viewAngle = kViewAngle;
	SetupCamera_Perspective(m_viewAngle);

	// �J������near,far��ݒ肷��
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
