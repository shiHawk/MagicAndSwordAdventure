#include "TitleScene.h"
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

	constexpr float kTitleBobFrequency = 2.0f; // �^�C�g�����S�̏㉺�h������iHz�j
	constexpr float kTitleBobAmplitude = 5.0f; // �^�C�g�����S�̏㉺�h��U���ipx�j
	constexpr int   kMillisecondsPerSecond = 1000; // �~���b���b���Z�p
}

TitleScene::TitleScene():
	m_cameraPos({0.0f,0.0f,0.0f}),
	m_cameraTarget({0.0f,0.0f,0.0f}),
	m_viewAngle(0.0f),
	m_titleHandle(-1),
	m_time(0.0f),
	m_offsetY(0)
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
	m_cameraPos.x = 0.0f;
	m_cameraPos.y = 200.0f;
	m_cameraPos.z = -840.0f;

	// �J�������ǂ������Ă��邩(�����_)
	m_cameraTarget.x = 0.0f;
	m_cameraTarget.y = 50.0f;
	m_cameraTarget.z = 0.0f;

	// �J�����̈ʒu�ƒ����_���w�肷��
	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);

	// �J�����̎���p��ݒ肷��
	m_viewAngle = 0.447f;
	SetupCamera_Perspective(m_viewAngle);

	// �J������near,far��ݒ肷��
	SetCameraNearFar(10.0f, 3000.0f);

	m_titleHandle = LoadGraph(L"Data/title/WarriorAdventureTitle.png");
}

void TitleScene::End()
{
}

SceneBase* TitleScene::Update()
{
	UpdateFade();
	m_time = GetNowCount() / kMillisecondsPerSecond;
	m_offsetY = static_cast<int>(sin(m_time * kTitleBobFrequency) * kTitleBobAmplitude);
	if(!m_isNextScene && !IsFadingOut() && Pad::isTrigger(PAD_INPUT_2))
	{
		StartFadeOut();
		m_isNextScene = true;
	}
	if (m_isNextScene && IsFadeComplete())
	{
		return new GameScene();
	}
	return this;
}

void TitleScene::Draw()
{	
	DrawExtendGraph(kTitlePosX, kTitlePosY + m_offsetY, kTitlePosX+ kTitleSize, kTitlePosY+ +m_offsetY+kTitleSize, m_titleHandle, true);
	DrawFade();
}
