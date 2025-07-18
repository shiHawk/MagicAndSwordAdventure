#include "GameScene.h"
#include "DxLib.h"
#include <cmath>
#include "game.h"
#include "Pad.h"
#include "ResultScene.h"
GameScene::GameScene():
	m_isNextScene(false)
{
}

void GameScene::LoadEnemyData(const std::string fileName, std::vector<std::shared_ptr<NormalSkelton>>& normalSkeltons, std::vector<std::shared_ptr<WizardSkelton>>& wizardSkeltons, std::shared_ptr<Player> pPlayer)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		printfDx(L"CSVファイルを開けませんでした: %s\n", fileName.c_str());
		return;
	}
	std::string line;
	std::getline(file, line); // 最初の行を読み飛ばす
	while (std::getline(file, line))
	{
		std::istringstream stream(line); // 一行分をストリームに変換する
		std::string type, strX, strY, strZ;

		if (!std::getline(stream, type, ',')) continue; // 敵の種類
		if (!std::getline(stream, strX, ',')) continue;	// x座標
		if (!std::getline(stream, strY, ',')) continue;	// y座標
		if (!std::getline(stream, strZ, ',')) continue;	// z座標

		// 文字列をfloatに変換する
		float x = std::stof(strX);
		float y = std::stof(strY);
		float z = std::stof(strZ);
		VECTOR enemyPos = VGet(x, y, z); // VECTOR型に変換する

		// 敵の種類に応じて生成し、リストに追加
		if (type == "normalSkelton")
		{
			auto normalSkelton = std::make_shared<NormalSkelton>();
			normalSkelton->Init(pPlayer, enemyPos);
			normalSkeltons.push_back(normalSkelton);
		}
		if (type == "wizardSkelton")
		{
			auto wizardSkelton = std::make_shared<WizardSkelton>();
			wizardSkelton->Init(pPlayer, enemyPos);
			wizardSkeltons.push_back(wizardSkelton);
		}
	}
}

void GameScene::Init()
{
	m_pCamera = std::make_shared<Camera>();
	m_pStage = std::make_shared<Stage>();
	m_pPlayer = std::make_shared<Player>();
	m_pCollision = std::make_shared<Collision>();
	m_pBattleArea = std::make_unique<BattleAreaManager>();
	LoadEnemyData("Data/enemyData/enemyPositionData.csv",m_NormalSkeltons,m_WizardSkeltons,m_pPlayer);
	m_pAnimation = std::make_shared<Animation>();
	m_pCamera->Init(m_pPlayer);
	m_pStage->Init();
	m_pPlayer->Init(m_pAnimation);
	m_pBattleArea->Init(m_pPlayer, m_pCamera);
	m_pBattleArea->SetEnemys(m_NormalSkeltons,m_WizardSkeltons);
	m_pCollision->Init(m_pPlayer,m_NormalSkeltons, m_WizardSkeltons);
	m_pAnimation->Init();
}

void GameScene::End()
{
	m_pPlayer->End();
	m_pCollision->End();
	m_pStage->End();
	m_pCamera->End();
	for (auto& normalSkelton : m_NormalSkeltons)
	{
		normalSkelton->End();
	}
	for (auto& wizardSkelton : m_WizardSkeltons)
	{
		wizardSkelton->End();
	}
}

SceneBase* GameScene::Update()
{
	m_pCamera->Update();
	m_pStage->Updata();
	m_pPlayer->Update();
	for (auto& normalSkelton : m_NormalSkeltons)
	{
		normalSkelton->Update();
	}
	for (auto& wizardSkelton : m_WizardSkeltons)
	{
		wizardSkelton->Update();
	}
	m_pBattleArea->Updata(m_NormalSkeltons, m_WizardSkeltons);
	m_pCollision->Update();
	UpdateFade();
	if (!m_isNextScene && !IsFadingOut() && CheckHitKey(KEY_INPUT_RETURN))
	{
		StartFadeOut();
		m_isNextScene = true;
	}
	if (m_isNextScene && IsFadeComplete())
	{
		return new ResultScene();
	}
	return this;
}

void GameScene::Draw()
{
	m_pPlayer->Draw();
	for (auto& normalSkelton : m_NormalSkeltons)
	{
		normalSkelton->Draw();
	}
	for (auto& wizardSkelton : m_WizardSkeltons)
	{
		wizardSkelton->Draw();
	}
	m_pStage->Draw();
	DrawFade();
	//m_pBattleArea->DebugDraw();
	//DrawGrid();
}

void GameScene::DrawGrid() const
{
	VECTOR start = VGet(-2700.0f,0.0f,0.0f); // 始点
	VECTOR end = VGet(2700.0f, 0.0f, 0.0f);  // 終点

	// 横方向のグリッドをfor文を使って描画する
	// 始点終点のXY座標は変わらない
	// Z座標のfor文を使って変化させる
	for (float z = -300.0f; z <= 300.0f; z += 100.0f)
	{
		start.z = z;
		end.z = z;
		DrawLine3D(start, end, 0xffffff);
	}

	// 奥行方向のグリッドを同様に引く
	start = VGet(0.0f, 0.0f, -300.0f); // 始点
	end = VGet(0.0f, 0.0f, 300.0f);   // 終点

	for (float x = -900.0f; x <= 900.0f; x += 100.0f)
	{
		start.x = x;
		end.x = x;
		DrawLine3D(start, end, 0xffffff);
	}
}
