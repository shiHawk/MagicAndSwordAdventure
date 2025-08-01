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

	// GameStart�̕����̈ʒu�̃|�W�V����
	constexpr int kStartPos = 600;

	constexpr float kTitleBobFrequency = 2.0f; // �^�C�g�����S�̏㉺�h�����(Hz)
	constexpr float kTitleBobAmplitude = 10.0f; // �^�C�g�����S�̏㉺�h��U��(px)
	constexpr int   kMillisecondsPerSecond = 1000; // �~���b���b���Z�p

	// near��far�̈ʒu
	constexpr float kCameraNearClip = 10.0f;
	constexpr float kCameraFarClip = 3000.0f;
	// �_�Ŏ���
	constexpr int kBlinkCycleMs = 500;
}

TitleScene::TitleScene():
	m_cameraPos({0.0f,0.0f,0.0f}),
	m_cameraTarget({0.0f,0.0f,0.0f}),
	m_viewAngle(0.0f),
	m_titleHandle(-1),
	m_time(0.0f),
	m_offsetY(0),
	m_titleBGHandle(-1)
{
}

void TitleScene::Init()
{
	m_fadeBright = 0;
	m_fadeSpeed = -kFadeSpeed;
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
}

void TitleScene::End()
{
	DeleteGraph(m_titleHandle);
	DeleteGraph(m_titleBGHandle);
	SoundManager::GetInstance()->StopBGM();
}

SceneBase* TitleScene::Update()
{
	UpdateFade();
	SoundManager::GetInstance()->Update();
	m_time = GetNowCount() / kMillisecondsPerSecond;
	// �^�C�g�����S���㉺���邽�߂̈ʒu�␳
	m_offsetY = static_cast<int>(sin(m_time * kTitleBobFrequency) * kTitleBobAmplitude);
	// A�{�^������������t�F�[�h���J�n
	if(!m_isNextScene && !IsFadingOut() && Pad::isTrigger(PAD_INPUT_1))
	{
		StartFadeOut();
		m_isNextScene = true;
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
	// �w�i��`��
	DrawGraph(0,0, m_titleBGHandle,false);
	// �^�C�g�����S���g��\��
	DrawExtendGraph(kTitlePosX, kTitlePosY + m_offsetY, kTitlePosX+ kTitleSize, kTitlePosY+ +m_offsetY+kTitleSize, m_titleHandle, true);
	// �_�ł�����
	if ((int)(GetNowCount() / kBlinkCycleMs) % 2 == 0)
	{
		DrawFormatString(kStartPos, kStartPos, 0x00ffff,"Press A Start");
	}
	DrawFade();
}

SceneID TitleScene::GetSceneID() const
{
	return SceneID::TitleScene;
}
