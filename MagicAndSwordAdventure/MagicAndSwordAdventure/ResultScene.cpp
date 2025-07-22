#include "ResultScene.h"
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
}
ResultScene::ResultScene(std::shared_ptr<ScoreManager> pScoreManager) :
	m_cameraPos({ 0.0f,0.0f,0.0f }),
	m_cameraTarget({ 0.0f,0.0f,0.0f }),
	m_viewAngle(0.0f),
	m_pScoreManager(pScoreManager)
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
	SetCameraNearFar(10.0f, 3000.0f);

}

void ResultScene::End()
{
	m_pScoreManager->End();
}

SceneBase* ResultScene::Update()
{
	UpdateFade();
	if (!m_isNextScene && !IsFadingOut() && Pad::isTrigger(PAD_INPUT_2))
	{
		StartFadeOut();
		m_isNextScene = true;
	}
	if (m_isNextScene && IsFadeComplete())
	{
		return new TitleScene();
	}
	return this;
}

void ResultScene::Draw()
{
	DrawFormatString(500,100,0xffffff,L"���j�X�R�A:%d",m_pScoreManager->GetDestroyScore());
	DrawFormatString(500,130,0xffffff,L"�^�C���{�[�i�X:%d",m_pScoreManager->GetTimeBonus());
	DrawFormatString(500,160,0xffffff,L"���v�X�R�A:%d",m_pScoreManager->GetScore());
	if ((int)(GetNowCount() / 500) % 2 == 0)
	{
		DrawFormatString(500, 260, 0x00ffff, L"Press B Title");
	}
	DrawFade();
}
