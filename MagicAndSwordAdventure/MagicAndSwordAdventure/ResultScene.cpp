#include "ResultScene.h"
#include "TitleScene.h"
#include "Pad.h"
#include "game.h"

ResultScene::ResultScene() :
	m_cameraPos({ 0.0f,0.0f,0.0f }),
	m_cameraTarget({ 0.0f,0.0f,0.0f }),
	m_viewAngle(0.0f)
{
}

void ResultScene::Init()
{
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

void ResultScene::End()
{
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
	DrawSphere3D(VGet(0, 0, 500), 40, 16, 0xffff00, 0xffffff, true);
	DrawFade();
}
