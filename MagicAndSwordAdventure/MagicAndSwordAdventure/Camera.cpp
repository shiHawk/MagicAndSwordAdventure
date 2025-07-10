#include "Camera.h"
#include "game.h"
#include <cmath>
#include "Pad.h"
namespace
{
	constexpr float kLerpSpeed = 0.015f;
	constexpr float kOffSetPos = 200.0f;
	constexpr VECTOR kSecondLight = { -0.577f, -0.577f, 0.577 };
}
Camera::Camera():
	m_cameraMoveAngle(0.0f),
	m_viewAngle(DX_PI_F / 3.0f),
	m_cameraPos({ 0.0f,0.0f,0.0f }),
	m_cameraTarget({ 0.0f,0.0f,0.0f }),
	m_CountDownFrame(220),
	m_cameraMoveTargetPos({ 0.0f,0.0f,0.0f })
{
}

Camera::~Camera()
{
}

void Camera::Init(std::shared_ptr<Player> pPlayer)
{
	m_pPlayer = pPlayer;
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
	m_cameraTarget.y = 90.0f;
	m_cameraTarget.z = 0.0f;

	// �J�����̈ʒu�ƒ����_���w�肷��
	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);

	// �J�����̎���p��ݒ肷��
	m_viewAngle = 0.447f;	
	SetupCamera_Perspective(m_viewAngle);

	// �J������near,far��ݒ肷��
	// ��ʂɕ\������鋗���͈̔͂�ݒ肷��
	// �J��������near�ȏ㗣��Ă���far���߂��ɂ�����̂�
	// �Q�[����ʂɕ\�������
	// far�͂��܂�傫�����鐔����ݒ肵�Ȃ��悤�ɋC��t����(�\���o�O�Ɍq����)
	SetCameraNearFar(10.0f, 3000.0f);

	CreateDirLightHandle(kSecondLight);
}

void Camera::Update()
{
	if (CheckHitKey(KEY_INPUT_Q))
	{
		m_viewAngle += 0.01;
	}
	if (CheckHitKey(KEY_INPUT_W))
	{
		m_viewAngle -= 0.01;
	}
	if (CheckHitKey(KEY_INPUT_E))
	{
		m_cameraPos.z += 10;
	}
	if (CheckHitKey(KEY_INPUT_R))
	{
		m_cameraPos.z -= 10;
	}
	SetupCamera_Perspective(m_viewAngle);
	//printfDx(L"m_viewAngle:%f\nm_cameraPos.z:%f\n",m_viewAngle,m_cameraPos.z);
	// �E�������Ă���Ƃ�
	if (m_pPlayer->GetDirRight() && m_pPlayer->GetIsMoving())
	{
		m_cameraMoveTargetPos.x = m_pPlayer->GetPos().x + kOffSetPos;
	}
	// ���������Ă���Ƃ�
	else if(!m_pPlayer->GetDirRight() && m_pPlayer->GetIsMoving())
	{
		m_cameraMoveTargetPos.x = m_pPlayer->GetPos().x - kOffSetPos;
	}
	m_cameraPos.x = std::lerp(m_cameraPos.x, m_cameraMoveTargetPos.x, kLerpSpeed);
	m_cameraTarget.x = std::lerp(m_cameraPos.x, m_cameraMoveTargetPos.x, kLerpSpeed);
	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);
}
