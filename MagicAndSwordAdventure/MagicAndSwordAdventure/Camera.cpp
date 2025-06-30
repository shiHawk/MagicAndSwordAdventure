#include "Camera.h"
#include "game.h"
#include <cmath>
#include "Pad.h"
namespace
{
	constexpr float kLerpSpeed = 0.05f;
	const float kOffSetPos = 10.0f;
}
Camera::Camera():
	m_cameraMoveAngle(0.0f),
	m_viewAngle(DX_PI_F / 3.0f),
	m_cameraPos(VGet(0, 0, 0)),
	m_cameraTarget(VGet(0, 0, 0)),
	m_CountDownFrame(220),
	m_t(0.1f),
	m_cameraMoveTargetPos({0,0,0})
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
	// 600 * 600のグリッドが画面中央あたりに表示されるカメラを設定する

	// カメラ(始点)の位置
	m_cameraPos.x = 0.0f;
	m_cameraPos.y = 200.0f;
	m_cameraPos.z = -600.0f;

	// カメラがどこを見ているか(注視点)
	m_cameraTarget.x = 0.0f;
	m_cameraTarget.y = 0.0f;
	m_cameraTarget.z = 0.0f;

	// カメラの位置と注視点を指定する
	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);

	// カメラの視野角を設定する
	m_viewAngle = DX_PI_F / 3.0f;	// 30度
	SetupCamera_Perspective(m_viewAngle);

	// カメラのnear,farを設定する
	// 画面に表示される距離の範囲を設定する
	// カメラからnear以上離れていてfarより近くにあるものが
	// ゲーム画面に表示される
	// farはあまり大きすぎる数字を設定しないように気を付ける(表示バグに繋がる)
	SetCameraNearFar(10.0f, 1000.0f);

	CreateDirLightHandle(VGet(-0.577f, -0.577f, 0.577));
}

void Camera::Update()
{
	if (m_pPlayer->GetScreenPos().x > Game::kScreenWidth * 0.5f)
	{
		m_cameraMoveTargetPos.x = m_pPlayer->GetPos().x + kOffSetPos;
	}
	if (m_pPlayer->GetScreenPos().x < Game::kScreenWidth * 0.45f)
	{
		m_cameraMoveTargetPos.x = m_pPlayer->GetPos().x + kOffSetPos;
	}
	m_cameraPos.x = std::lerp(m_cameraPos.x, m_cameraMoveTargetPos.x, kLerpSpeed);
	m_cameraTarget.x = std::lerp(m_cameraPos.x, m_cameraMoveTargetPos.x, kLerpSpeed);
	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);
}
