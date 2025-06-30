#include "GameScene.h"
#include "DxLib.h"
#include <cmath>
#include "game.h"
namespace
{
	constexpr float kLerpSpeed = 0.01f;
}
GameScene::GameScene()
{
}

void GameScene::Init()
{
	m_pCamera = std::make_shared<Camera>();
	m_pPlayer = std::make_shared<Player>();
	m_pEnemy = std::make_shared<Enemy>();
	m_pCollision = std::make_shared<Collision>();
	m_pAnimation = std::make_shared<Animation>();

	m_pCamera->Init(m_pPlayer);
	m_pPlayer->Init(m_pEnemy,m_pAnimation);
	m_pEnemy->Init(m_pCollision);
	m_pCollision->Init(m_pPlayer, m_pEnemy);
	m_pAnimation->Init();
}

void GameScene::End()
{
	m_pPlayer->End();
	m_pCollision->End();
}

SceneBase* GameScene::Update()
{
	m_pCamera->Update();
	m_pPlayer->Update();
	m_pEnemy->Update();
	m_pCollision->Update();
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

	//// 奥行方向のグリッドを同様に引く
	//start = VGet(0.0f, 0.0f, -300.0f); // 始点
	//end = VGet(0.0f, 0.0f, 300.0f);   // 終点

	//for (int x = -900; x <= 900; x += 100)
	//{
	//	start.x = x;
	//	end.x = x;

	//	DrawLine3D(start, end, 0xffffff);
	//}
}
