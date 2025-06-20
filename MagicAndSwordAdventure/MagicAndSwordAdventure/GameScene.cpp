#include "GameScene.h"
#include "DxLib.h"
#include <cmath>
#include "game.h"
namespace
{
	constexpr float kLerpSpeed = 0.1f;
}
GameScene::GameScene() :
	m_cameraMoveAngle(0.0f),
	m_viewAngle(DX_PI_F / 3.0f),
	m_cameraPos(VGet(0, 0, 0)),
	m_cameraTarget(VGet(0, 0, 0)),
	m_CountDownFrame(220),
	m_t(0.1f)
{
}

void GameScene::Init()
{
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
	m_viewAngle = DX_PI_F / 3.0f;	// 60度
	SetupCamera_Perspective(m_viewAngle);

	// カメラのnear,farを設定する
	// 画面に表示される距離の範囲を設定する
	// カメラからnear以上離れていてfarより近くにあるものが
	// ゲーム画面に表示される
	// farはあまり大きすぎる数字を設定しないように気を付ける(表示バグに繋がる)
	SetCameraNearFar(10.0f, 1000.0f);

	CreateDirLightHandle(VGet(-0.577f,-0.577f,0.577));

	m_pPlayer = std::make_shared<Player>();
	m_pEnemy = std::make_shared<Enemy>();
	m_pCollision = std::make_shared<Collision>();

	m_pPlayer->Init(m_pEnemy);
	m_pEnemy->Init(m_pCollision);
	m_pCollision->Init(m_pPlayer, m_pEnemy);
}

void GameScene::End()
{
	m_pPlayer->End();
	m_pCollision->End();
}

SceneBase* GameScene::Update()
{
	m_pPlayer->Update();
	m_pEnemy->Update();
	m_pCollision->Update();
	if (m_pPlayer->GetScreenPos().x > Game::kScreenWidth * 0.5f)
	{
		m_cameraMoveTargetPos.x += m_pPlayer->GetScreenPos().x - Game::kScreenWidth * 0.5f;
		//m_cameraTarget.x += m_pPlayer->GetScreenPos().x - Game::kScreenWidth * 0.5f;
	}
	else if (m_pPlayer->GetScreenPos().x < 500.0f)
	{
		m_cameraMoveTargetPos.x += m_pPlayer->GetScreenPos().x - 500.0f;
		//m_cameraTarget.x += m_pPlayer->GetScreenPos().x - Game::kScreenWidth * 0.4f;
	}
	m_cameraPos.x = std::lerp(m_cameraPos.x, m_cameraMoveTargetPos.x, kLerpSpeed);
	m_cameraTarget.x = std::lerp(m_cameraPos.x, m_cameraMoveTargetPos.x, kLerpSpeed);
	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);
	return this;
}

void GameScene::Draw()
{
	m_pPlayer->Draw();
	m_pEnemy->Draw();
	m_pCollision->Draw();
	DrawGrid();
}

void GameScene::DrawGrid() const
{
	// VECTOR構造体
	// 3D座標を表示するのに必要なx,y,zの3つをメンバーとして持つ構造体

	VECTOR start = VGet(-2700.0f,0.0f,0.0f); // 始点
	VECTOR end = VGet(2700.0f, 0.0f, 0.0f);   // 終点

	// 横方向のグリッドをfor文を使って描画する
	// 始点終点のXY座標は変わらない
	// Z座標のfor文を使って変化させる
	for (int z = -300; z <= 300; z += 100)
	{
		start.z = z;
		end.z = z;

		DrawLine3D(start, end, 0xffffff);
	}

	// 奥行方向のグリッドを同様に引く
	start = VGet(0.0f, 0.0f, -300.0f); // 始点
	end = VGet(0.0f, 0.0f, 300.0f);   // 終点

	for (int x = -900; x <= 900; x += 100)
	{
		start.x = x;
		end.x = x;

		DrawLine3D(start, end, 0xffffff);
	}
}
