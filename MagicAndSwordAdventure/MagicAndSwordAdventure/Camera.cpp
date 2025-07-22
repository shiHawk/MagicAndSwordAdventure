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
	// カメラの位置と注視点
	constexpr VECTOR kCameraPos = { -4800.0f,200.0f,-840.0f };
	constexpr VECTOR kCameraTarget = { -4800.0f,50.0f,0.0f };
	// カメラの視野角
	constexpr float kViewAngle = 0.447f;
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
	// 3D表示の設定
	SetUseZBuffer3D(true);	  // Zバッファを指定する
	SetWriteZBuffer3D(true);  // Zバッファへの書き込みを行う

	SetUseBackCulling(true);  // ポリゴンの裏面を表示しない

	// カメラの位置の初期化を行う

	// カメラ(始点)の位置
	m_cameraPos = kCameraPos;

	// カメラがどこを見ているか(注視点)
	m_cameraTarget = kCameraTarget;

	// カメラの位置と注視点を指定する
	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);

	// カメラの視野角を設定する
	m_viewAngle = kViewAngle;
	SetupCamera_Perspective(m_viewAngle);

	// カメラのnear,farを設定する
	// 画面に表示される距離の範囲を設定する
	// カメラからnear以上離れていてfarより近くにあるものが
	// ゲーム画面に表示される
	// farはあまり大きすぎる数字を設定しないように気を付ける(表示バグに繋がる)
	SetCameraNearFar(10.0f, 3000.0f);

	m_lightHandle = CreateDirLightHandle(kSecondLight);
}

void Camera::End()
{
	DeleteLightHandle(m_lightHandle);
}

void Camera::Update()
{
	/*if (CheckHitKey(KEY_INPUT_Q))
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
	SetupCamera_Perspective(m_viewAngle);*/
	//printfDx(L"m_viewAngle:%f\nm_cameraPos.z:%f\n",m_viewAngle,m_cameraPos.z);
	if (!m_isBattleCamera)
	{
		// 右を向いているとき
		if (m_pPlayer->IsDirRight() && m_pPlayer->IsMoving())
		{
			m_cameraMoveTargetPos.x = m_pPlayer->GetPos().x + kOffSetPos;
		}
		// 左を向いているとき
		else if (!m_pPlayer->IsDirRight() && m_pPlayer->IsMoving())
		{
			m_cameraMoveTargetPos.x = m_pPlayer->GetPos().x - kOffSetPos;
		}
		m_cameraPos.x = std::lerp(m_cameraPos.x, m_cameraMoveTargetPos.x, kLerpSpeed);
		m_cameraTarget.x = std::lerp(m_cameraPos.x, m_cameraMoveTargetPos.x, kLerpSpeed);
	}
	if (m_cameraPos.x < kLeftLimitCamera)
	{
		m_cameraPos.x = kLeftLimitCamera;
		m_cameraTarget.x = kLeftLimitCamera;
	}
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
	m_cameraPos.x = std::lerp(m_cameraPos.x, m_cameraMoveTargetPos.x, kLerpSpeed);
	m_cameraTarget.x = std::lerp(m_cameraPos.x, m_cameraMoveTargetPos.x, kLerpSpeed);
	//printfDx(L"m_cameraTarget.x:%f\nm_cameraPos.x:%f\n", m_cameraTarget.x,m_cameraPos.x);
	/*m_cameraPos = VGet(cameraTarget.x, m_cameraPos.y,m_cameraPos.z);
	m_cameraMoveTargetPos = cameraTarget;*/
	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);
	m_isBattleCamera = true; 
}
