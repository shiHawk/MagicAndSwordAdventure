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
	// 3D表示の設定
	SetUseZBuffer3D(true);	  // Zバッファを指定する
	SetWriteZBuffer3D(true);  // Zバッファへの書き込みを行う

	SetUseBackCulling(true);  // ポリゴンの裏面を表示しない

	// カメラの位置の初期化を行う

	// カメラ(始点)の位置
	m_cameraPos.x = 0.0f;
	m_cameraPos.y = 200.0f;
	m_cameraPos.z = -840.0f;

	// カメラがどこを見ているか(注視点)
	m_cameraTarget.x = 0.0f;
	m_cameraTarget.y = 90.0f;
	m_cameraTarget.z = 0.0f;

	// カメラの位置と注視点を指定する
	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);

	// カメラの視野角を設定する
	m_viewAngle = 0.447f;	
	SetupCamera_Perspective(m_viewAngle);

	// カメラのnear,farを設定する
	// 画面に表示される距離の範囲を設定する
	// カメラからnear以上離れていてfarより近くにあるものが
	// ゲーム画面に表示される
	// farはあまり大きすぎる数字を設定しないように気を付ける(表示バグに繋がる)
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
	// 右を向いているとき
	if (m_pPlayer->GetDirRight() && m_pPlayer->GetIsMoving())
	{
		m_cameraMoveTargetPos.x = m_pPlayer->GetPos().x + kOffSetPos;
	}
	// 左を向いているとき
	else if(!m_pPlayer->GetDirRight() && m_pPlayer->GetIsMoving())
	{
		m_cameraMoveTargetPos.x = m_pPlayer->GetPos().x - kOffSetPos;
	}
	m_cameraPos.x = std::lerp(m_cameraPos.x, m_cameraMoveTargetPos.x, kLerpSpeed);
	m_cameraTarget.x = std::lerp(m_cameraPos.x, m_cameraMoveTargetPos.x, kLerpSpeed);
	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);
}
