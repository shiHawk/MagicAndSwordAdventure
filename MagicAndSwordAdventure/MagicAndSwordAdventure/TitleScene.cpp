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
	// �t�F�[�h���x
	constexpr int kFadeSpeed = 8;
	// �^�C�g���̍��W
	constexpr int kTitlePosX = 400;
	constexpr int kTitlePosY = 75;
	constexpr int kTitleSize = 512;

	// �J�����̈ʒu�ƒ����_
	constexpr VECTOR kCameraPos = { 0.0f,200.0f,-840.0f };
	constexpr VECTOR kCameraTarget = { 0.0f,50.0f,0.0f };
	// �J�����̎���p
	constexpr float kViewAngle = 0.447f;

	// PressA�̕����̈ʒu�̃|�W�V����
	constexpr int kPressAPosX = 570;
	constexpr int kPressAPosY = 600;
	// GameStart�̕����̈ʒu�̃|�W�V����
	constexpr int kStartPosX = 570;
	constexpr int kStartPosY = 660;

	constexpr float kTitleBobFrequency = 2.0f; // �^�C�g�����S�̏㉺�h�����(Hz)
	constexpr float kTitleBobAmplitude = 10.0f; // �^�C�g�����S�̏㉺�h��U��(px)
	constexpr int   kMillisecondsPerSecond = 1000; // �~���b���b���Z�p

	// near��far�̈ʒu
	constexpr float kCameraNearClip = 10.0f;
	constexpr float kCameraFarClip = 3000.0f;
	// �_�Ŏ���
	constexpr int kBlinkCycleMs = 500;
	constexpr float kModelScale = 50.0f; // ���f���̃X�P�[��
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

	// 3D�\���̐ݒ�
	SetUseZBuffer3D(true);	  // Z�o�b�t�@���w�肷��
	SetWriteZBuffer3D(true);  // Z�o�b�t�@�ւ̏������݂��s��

	SetUseBackCulling(true);  // �|���S���̗��ʂ�\�����Ȃ�

	// �J�����̈ʒu�̏��������s��

	// �J����(�n�_)�̈ʒu
	m_cameraPos = kCameraPos;

	// �J�������ǂ������Ă��邩(�����_)
	m_cameraTarget = kCameraTarget;

	// �J�����̈ʒu�ƒ����_���w�肷��
	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);

	// �J�����̎���p��ݒ肷��
	m_viewAngle = kViewAngle;
	SetupCamera_Perspective(m_viewAngle);

	// �J������near,far��ݒ肷��
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
	
	// �^�C�g�����S���㉺���邽�߂̈ʒu�␳
	m_offsetY = static_cast<int>(sin(m_time * kTitleBobFrequency) * kTitleBobAmplitude);
	// A�{�^������������t�F�[�h���J�n
	if (!m_isNextScene && !IsFadingOut() && Pad::isTrigger(PAD_INPUT_1))
	{
		if (!m_isManualFlag)
		{
			// �ŏ���A�{�^���ő��������\��
			m_isManualFlag = true;
		}
		else
		{
			// 2��ڂ�A�{�^���Ńt�F�[�h�J�n
			StartFadeOut();
			m_isNextScene = true;
		}
	}
	if (IsFadingOut())
	{
		SoundManager::GetInstance()->FadeBGMVol();
	}
	// �t�F�[�h���I��������J�ڂ���
	if (m_isNextScene && IsFadeComplete())
	{
		return new GameScene();
	}
	return this;
}

void TitleScene::Draw()
{	
	DrawGraph(0, 0, m_BGHandle, false);
	// �w�i��`��
	//DrawGraph(0,0, m_titleBGHandle,false);
	// �^�C�g�����S���g��\��
	DrawExtendGraph(kTitlePosX, kTitlePosY + m_offsetY, kTitlePosX+ kTitleSize, kTitlePosY+ +m_offsetY+kTitleSize, m_titleHandle, true);
	if (m_isManualFlag)
	{
		DrawGraph(0, 0, m_manualHandle, false);
	}
	// �_�ł�����
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
