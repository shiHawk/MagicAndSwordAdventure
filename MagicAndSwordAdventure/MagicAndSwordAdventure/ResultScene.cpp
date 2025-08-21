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
	constexpr int kCharaPosx = 500;
	constexpr int kDestroyScorePosY = 100;
	constexpr int kTimeScorePosY = 260;
	constexpr int kScorePosY = 420;
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
	constexpr int kPressBPosY = 600;
	// �_�Ŏ���
	constexpr int kBlinkCycleMs = 500;
}
ResultScene::ResultScene(std::shared_ptr<ScoreManager> pScoreManager) :
	m_cameraPos({ 0.0f,0.0f,0.0f }),
	m_cameraTarget({ 0.0f,0.0f,0.0f }),
	m_viewAngle(0.0f),
	m_pScoreManager(pScoreManager),
	m_resultHandle(-1),
	m_fontHandle(-1),
	m_skeltonIconHandle(-1),
	m_timerIconHandle(-1)
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
}

void ResultScene::End()
{
	DeleteGraph(m_resultHandle);
	DeleteGraph(m_skeltonIconHandle);
	DeleteGraph(m_timerIconHandle);
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
	DrawFormatStringToHandle(kCharaPosx, kDestroyScorePosY, kFontColorWhite,m_fontHandle,"Defeat Score: %d",m_pScoreManager->GetDestroyScore());
	DrawFormatStringToHandle(kCharaPosx,kTimeScorePosY, kFontColorWhite, m_fontHandle, "Time Bonus: %d",m_pScoreManager->GetTimeBonus());
	DrawFormatStringToHandle(kCharaPosx,kScorePosY, kFontColorRed, m_fontHandle, "Total Score: %d",m_pScoreManager->GetScore());
	if ((int)(GetNowCount() / kBlinkCycleMs) % 2 == 0)
	{
		DrawFormatStringToHandle(kCharaPosx, kPressBPosY, kFontColorBlue, m_fontHandle,"Press A Title");
	}
	DrawFade();
}

SceneID ResultScene::GetSceneID() const
{
	return SceneID::ResultScene;
}
