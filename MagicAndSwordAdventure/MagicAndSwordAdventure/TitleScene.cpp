#include "TitleScene.h"
#include "DxLib.h"
#include "GameScene.h"
#include "Pad.h"
#include "game.h"

namespace
{
	constexpr int kMaxFadeBright = 255;
	// フェード速度
	constexpr int kFadeSpeed = 8;
	constexpr VECTOR kSecondLight = { -0.577f, -0.577f, 0.577 };
}

TitleScene::TitleScene():
	m_cameraPos({0.0f,0.0f,0.0f}),
	m_cameraTarget({0.0f,0.0f,0.0f}),
	m_viewAngle(0.0f)
{
}

void TitleScene::Init()
{
	m_fadeBright = 0;
	m_fadeSpeed = -kFadeSpeed;
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
	m_cameraTarget.y = 50.0f;
	m_cameraTarget.z = 0.0f;

	// カメラの位置と注視点を指定する
	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);

	// カメラの視野角を設定する
	m_viewAngle = 0.447f;
	SetupCamera_Perspective(m_viewAngle);

	// カメラのnear,farを設定する
	SetCameraNearFar(10.0f, 3000.0f);
}

void TitleScene::End()
{
}

SceneBase* TitleScene::Update()
{
	UpdateFade();
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
	DrawSphere3D(VGet(0,0,500), 40, 16, 0xff00ff, 0xffffff, true);
	DrawFade();
}
