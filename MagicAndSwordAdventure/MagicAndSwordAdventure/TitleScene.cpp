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
	// フェード速度
	constexpr int kFadeSpeed = 8;
	// タイトルの座標
	constexpr int kTitlePosX = 400;
	constexpr int kTitlePosY = 75;
	constexpr int kTitleSize = 512;

	// カメラの位置と注視点
	constexpr VECTOR kCameraPos = { 0.0f,200.0f,-840.0f };
	constexpr VECTOR kCameraTarget = { 0.0f,50.0f,0.0f };
	// カメラの視野角
	constexpr float kViewAngle = 0.447f;

	// GameStartの文字の位置のポジション
	constexpr int kStartPos = 600;

	constexpr float kTitleBobFrequency = 2.0f; // タイトルロゴの上下揺れ周期(Hz)
	constexpr float kTitleBobAmplitude = 10.0f; // タイトルロゴの上下揺れ振幅(px)
	constexpr int   kMillisecondsPerSecond = 1000; // ミリ秒→秒換算用

	// nearとfarの位置
	constexpr float kCameraNearClip = 10.0f;
	constexpr float kCameraFarClip = 3000.0f;
	// 点滅周期
	constexpr int kBlinkCycleMs = 500;
	constexpr float kModelScale = 50.0f; // モデルのスケール
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
	// タイトルロゴが上下するための位置補正
	m_offsetY = static_cast<int>(sin(m_time * kTitleBobFrequency) * kTitleBobAmplitude);
	// Aボタンを押したらフェードを開始
	if(!m_isNextScene && !IsFadingOut() && Pad::isTrigger(PAD_INPUT_1))
	{
		StartFadeOut();
		m_isNextScene = true;
	}
	if (IsFadingOut())
	{
		SoundManager::GetInstance()->FadeBGMVol();
	}
	// フェードが終了したら遷移する
	if (m_isNextScene && IsFadeComplete())
	{
		
		return new GameScene();
	}
	return this;
}

void TitleScene::Draw()
{	
	//// 頂点配列（PolygonNum × 3個 = 2×3=6個）
	//VERTEX3D vertices[6];

	//// 左上、右上、右下、左下（四角形の座標）
	//VECTOR posLT = VGet(-640.0f, 360.0f, 800.0f); // 左上
	//VECTOR posRT = VGet(640.0f, 360.0f, 800.0f);  // 右上
	//VECTOR posRB = VGet(640.0f, -400.0f, 800.0f); // 右下
	//VECTOR posLB = VGet(-640.0f, -400.0f, 800.0f);// 左下

	//// 三角形1：左上、右上、右下
	//vertices[0].pos = posLT; vertices[0].norm = VGet(0, 0, -1); vertices[0].dif = GetColorU8(255, 255, 255, 255); vertices[0].spc = GetColorU8(0, 0, 0, 0); vertices[0].u = 0.0f; vertices[0].v = 0.0f;
	//vertices[1].pos = posRT; vertices[1].norm = VGet(0, 0, -1); vertices[1].dif = GetColorU8(255, 255, 255, 255); vertices[1].spc = GetColorU8(0, 0, 0, 0); vertices[1].u = 1.0f; vertices[1].v = 0.0f;
	//vertices[2].pos = posRB; vertices[2].norm = VGet(0, 0, -1); vertices[2].dif = GetColorU8(255, 255, 255, 255); vertices[2].spc = GetColorU8(0, 0, 0, 0); vertices[2].u = 1.0f; vertices[2].v = 1.0f;

	//// 三角形2：右下、左下、左上
	//vertices[3].pos = posRB; vertices[3].norm = VGet(0, 0, -1); vertices[3].dif = GetColorU8(255, 255, 255, 255); vertices[3].spc = GetColorU8(0, 0, 0, 0); vertices[3].u = 1.0f; vertices[3].v = 1.0f;
	//vertices[4].pos = posLB; vertices[4].norm = VGet(0, 0, -1); vertices[4].dif = GetColorU8(255, 255, 255, 255); vertices[4].spc = GetColorU8(0, 0, 0, 0); vertices[4].u = 0.0f; vertices[4].v = 1.0f;
	//vertices[5].pos = posLT; vertices[5].norm = VGet(0, 0, -1); vertices[5].dif = GetColorU8(255, 255, 255, 255); vertices[5].spc = GetColorU8(0, 0, 0, 0); vertices[5].u = 0.0f; vertices[5].v = 0.0f;

	//DrawPolygon3D(vertices, 2, m_titleBGHandle, TRUE);
	MV1DrawModel(m_modelHandle);
	// 背景を描画
	DrawGraph(0,0, m_titleBGHandle,false);
	// タイトルロゴを拡大表示
	DrawExtendGraph(kTitlePosX, kTitlePosY + m_offsetY, kTitlePosX+ kTitleSize, kTitlePosY+ +m_offsetY+kTitleSize, m_titleHandle, true);
	// 点滅させる
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
