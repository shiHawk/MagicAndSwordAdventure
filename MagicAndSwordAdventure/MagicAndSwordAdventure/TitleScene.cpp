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
	m_modelHandle(-1),
	m_playerPos({ 0.0f,0.0f,0.0f })
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
	MV1DeleteModel(m_modelHandle);
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
	//// ���_�z��iPolygonNum �~ 3�� = 2�~3=6�j
	//VERTEX3D vertices[6];

	//// ����A�E��A�E���A�����i�l�p�`�̍��W�j
	//VECTOR posLT = VGet(-640.0f, 360.0f, 800.0f); // ����
	//VECTOR posRT = VGet(640.0f, 360.0f, 800.0f);  // �E��
	//VECTOR posRB = VGet(640.0f, -400.0f, 800.0f); // �E��
	//VECTOR posLB = VGet(-640.0f, -400.0f, 800.0f);// ����

	//// �O�p�`1�F����A�E��A�E��
	//vertices[0].pos = posLT; vertices[0].norm = VGet(0, 0, -1); vertices[0].dif = GetColorU8(255, 255, 255, 255); vertices[0].spc = GetColorU8(0, 0, 0, 0); vertices[0].u = 0.0f; vertices[0].v = 0.0f;
	//vertices[1].pos = posRT; vertices[1].norm = VGet(0, 0, -1); vertices[1].dif = GetColorU8(255, 255, 255, 255); vertices[1].spc = GetColorU8(0, 0, 0, 0); vertices[1].u = 1.0f; vertices[1].v = 0.0f;
	//vertices[2].pos = posRB; vertices[2].norm = VGet(0, 0, -1); vertices[2].dif = GetColorU8(255, 255, 255, 255); vertices[2].spc = GetColorU8(0, 0, 0, 0); vertices[2].u = 1.0f; vertices[2].v = 1.0f;

	//// �O�p�`2�F�E���A�����A����
	//vertices[3].pos = posRB; vertices[3].norm = VGet(0, 0, -1); vertices[3].dif = GetColorU8(255, 255, 255, 255); vertices[3].spc = GetColorU8(0, 0, 0, 0); vertices[3].u = 1.0f; vertices[3].v = 1.0f;
	//vertices[4].pos = posLB; vertices[4].norm = VGet(0, 0, -1); vertices[4].dif = GetColorU8(255, 255, 255, 255); vertices[4].spc = GetColorU8(0, 0, 0, 0); vertices[4].u = 0.0f; vertices[4].v = 1.0f;
	//vertices[5].pos = posLT; vertices[5].norm = VGet(0, 0, -1); vertices[5].dif = GetColorU8(255, 255, 255, 255); vertices[5].spc = GetColorU8(0, 0, 0, 0); vertices[5].u = 0.0f; vertices[5].v = 0.0f;

	//DrawPolygon3D(vertices, 2, m_titleBGHandle, TRUE);
	MV1DrawModel(m_modelHandle);
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
