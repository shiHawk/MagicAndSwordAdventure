#include "Camera.h"
#include "game.h"
#include <cmath>
#include "Pad.h"
namespace
{
	constexpr float kLerpSpeed = 0.015f;
	constexpr float kOffSetPos = 200.0f;
	constexpr VECTOR kSecondLight = { -0.577f, -0.577f, 0.577 };
	constexpr float kRightLimitCamera = 4807.0f;
	constexpr float kLeftLimitCamera = -2355.0f;
	// �J�����̈ʒu�ƒ����_
	constexpr VECTOR kCameraPos = { -4800.0f,200.0f,-840.0f };
	constexpr VECTOR kCameraTarget = { -4800.0f,50.0f,0.0f };
	// �J�����̎���p
	constexpr float kViewAngle = 0.447f;
	// near��far�̈ʒu
	constexpr float kCameraNearClip = 10.0f;
	constexpr float kCameraFarClip = 3000.0f;
	// ���C�g�̃J���[
	constexpr float kRed = 1.0f;
	constexpr float kGreen = 0.647f;
	constexpr float kBlue = 0.0f;
}
Camera::Camera():
	m_cameraMoveAngle(0.0f),
	m_viewAngle(DX_PI_F / 3.0f),
	m_cameraPos({ -400.0f,0.0f,0.0f }),
	m_cameraTarget({ -400.0f,0.0f,0.0f }),
	m_CountDownFrame(220),
	m_cameraMoveTargetPos({ -400.0f,0.0f,0.0f }),
	m_isBattleCamera(false),
	m_lightHandle(-1)
{
}

Camera::~Camera()
{
}

void Camera::Init(std::shared_ptr<Player> pPlayer)
{
	m_pPlayer = pPlayer;
	m_isBattleCamera = false;
	// 3D�\���̐ݒ�
	SetUseZBuffer3D(true);	  // Z�o�b�t�@���w�肷��
	SetWriteZBuffer3D(true);  // Z�o�b�t�@�ւ̏������݂��s��

	SetUseBackCulling(true);  // �|���S���̗��ʂ�\�����Ȃ�

	// �J�����̐F��ύX����
	SetLightSpcColor(GetColorF(kRed, kGreen, kBlue, 0.0f));

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
	// ��ʂɕ\������鋗���͈̔͂�ݒ肷��
	// �J��������near�ȏ㗣��Ă���far���߂��ɂ�����̂�
	// �Q�[����ʂɕ\�������
	// far�͂��܂�傫�����鐔����ݒ肵�Ȃ��悤�ɋC��t����(�\���o�O�Ɍq����)
	SetCameraNearFar(kCameraNearClip, kCameraFarClip);

	m_lightHandle = CreateDirLightHandle(kSecondLight);
	SetLightDifColorHandle(m_lightHandle, GetColorF(kRed, kGreen, kBlue, 0.0f));
}

void Camera::End()
{
	DeleteLightHandle(m_lightHandle);
}

void Camera::Update()
{
	//printfDx(L"m_viewAngle:%f\nm_cameraPos.z:%f\n",m_viewAngle,m_cameraPos.z);
	if (!m_isBattleCamera)
	{
		// �E�������Ă���Ƃ�
		if (m_pPlayer->IsDirRight() && m_pPlayer->IsMoving())
		{
			m_cameraMoveTargetPos.x = m_pPlayer->GetPos().x + kOffSetPos;
		}
		// ���������Ă���Ƃ�
		else if (!m_pPlayer->IsDirRight() && m_pPlayer->IsMoving())
		{
			m_cameraMoveTargetPos.x = m_pPlayer->GetPos().x - kOffSetPos;
		}
		// ���X�ɖڕW�ʒu�܂ŋ߂Â���
		m_cameraPos.x = std::lerp(m_cameraPos.x, m_cameraMoveTargetPos.x, kLerpSpeed);
		m_cameraTarget.x = std::lerp(m_cameraPos.x, m_cameraMoveTargetPos.x, kLerpSpeed);
	}
	// �X�e�[�W�̍��[�܂ōs������J����������ȏ��ɍs���Ȃ��l�ɂ���
	if (m_cameraPos.x < kLeftLimitCamera)
	{
		m_cameraPos.x = kLeftLimitCamera;
		m_cameraTarget.x = kLeftLimitCamera;
	}
	// �X�e�[�W�̉E�[�܂ōs������J����������ȏ��ɍs���Ȃ��l�ɂ���
	if (m_cameraPos.x > kRightLimitCamera)
	{
		m_cameraPos.x = kRightLimitCamera;
		m_cameraTarget.x = kRightLimitCamera;
	}
	
	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);
	//printfDx(L"m_cameraTarget.x:%f\nm_cameraPos.x:%f\n", m_cameraTarget.x,m_cameraPos.x);
}

void Camera::ChangeBattleCamera(VECTOR cameraTarget)
{
	m_cameraMoveTargetPos.x = cameraTarget.x;
	// ���X�ɖڕW�ʒu�܂ŋ߂Â���
	m_cameraPos.x = std::lerp(m_cameraPos.x, m_cameraMoveTargetPos.x, kLerpSpeed);
	m_cameraTarget.x = std::lerp(m_cameraPos.x, m_cameraMoveTargetPos.x, kLerpSpeed);
	//printfDx(L"m_cameraTarget.x:%f\nm_cameraPos.x:%f\n", m_cameraTarget.x,m_cameraPos.x);
	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);
	m_isBattleCamera = true; 
}
