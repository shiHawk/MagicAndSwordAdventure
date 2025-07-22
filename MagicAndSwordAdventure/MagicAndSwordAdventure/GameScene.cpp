#include "GameScene.h"
#include "DxLib.h"
#include <cmath>
#include "game.h"
#include "Pad.h"
#include "ResultScene.h"
GameScene::GameScene():
	m_isNextScene(false),
	m_remainingEnemysCount(30)
{
}

void GameScene::LoadEnemyData(const std::string fileName, std::vector<std::shared_ptr<NormalSkelton>>& normalSkeltons, std::vector<std::shared_ptr<WizardSkelton>>& wizardSkeltons, 
							  std::shared_ptr<Player> pPlayer, std::shared_ptr<ScoreManager> pScoreManager)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		printfDx(L"CSV�t�@�C�����J���܂���ł���: %s\n", fileName.c_str());
		return;
	}
	std::string line;
	std::getline(file, line); // �ŏ��̍s��ǂݔ�΂�
	while (std::getline(file, line))
	{
		std::istringstream stream(line); // ��s�����X�g���[���ɕϊ�����
		std::string type, strX, strY, strZ;

		if (!std::getline(stream, type, ',')) continue; // �G�̎��
		if (!std::getline(stream, strX, ',')) continue;	// x���W
		if (!std::getline(stream, strY, ',')) continue;	// y���W
		if (!std::getline(stream, strZ, ',')) continue;	// z���W

		// �������float�ɕϊ�����
		float x = std::stof(strX);
		float y = std::stof(strY);
		float z = std::stof(strZ);
		VECTOR enemyPos = VGet(x, y, z); // VECTOR�^�ɕϊ�����

		// �G�̎�ނɉ����Đ������A���X�g�ɒǉ�
		if (type == "normalSkelton")
		{
			auto normalSkelton = std::make_shared<NormalSkelton>();
			normalSkelton->Init(pPlayer, enemyPos, pScoreManager);
			normalSkeltons.push_back(normalSkelton);
		}
		if (type == "wizardSkelton")
		{
			auto wizardSkelton = std::make_shared<WizardSkelton>();
			wizardSkelton->Init(pPlayer, enemyPos,pScoreManager);
			wizardSkeltons.push_back(wizardSkelton);
		}
	}
}

void GameScene::Init()
{
	m_pCamera = std::make_shared<Camera>();
	m_pScoreManager = std::make_shared<ScoreManager>();
	m_pStage = std::make_shared<Stage>();
	m_pPlayer = std::make_shared<Player>();
	m_pCollision = std::make_shared<Collision>();
	m_pBattleArea = std::make_unique<BattleAreaManager>();
	LoadEnemyData("Data/enemyData/enemyPositionData.csv",m_NormalSkeltons,m_WizardSkeltons,m_pPlayer,m_pScoreManager);
	m_pAnimation = std::make_shared<Animation>();
	m_pUIManager = std::make_unique<UIManager>();
	m_pCamera->Init(m_pPlayer);
	m_pScoreManager->Init();
	m_pStage->Init();
	m_pPlayer->Init(m_pAnimation);
	m_pBattleArea->Init(m_pPlayer, m_pCamera);
	m_pBattleArea->SetEnemys(m_NormalSkeltons,m_WizardSkeltons);
	m_pUIManager->Init(m_pPlayer, m_pScoreManager);
	m_pCollision->Init(m_pPlayer,m_NormalSkeltons, m_WizardSkeltons);
	m_pAnimation->Init();
	m_remainingEnemysCount = 0;
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
	m_pScoreManager->Update();
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
	m_pUIManager->Update();
	UpdateFade();
	if (m_pPlayer->IsDead())
	{
		m_pScoreManager->SetIsPlayerDead(true);
	}
	if (!m_isNextScene && !IsFadingOut() && (m_pPlayer->IsDead() || IsAreAllEnemiesDefeated()))
	{
		StartFadeOut();
		m_isNextScene = true;
	}
	if (m_isNextScene && IsFadeComplete())
	{
		return new ResultScene(m_pScoreManager);
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
	m_pUIManager->DrawHp();
	m_pUIManager->DrawDestroyScore();
	m_pUIManager->DrawmElapsedTimeSeconds();
	m_pUIManager->DrawNumberOfEnemiesRemaining(GetRemainingEnemies());
	if (!m_pBattleArea->IsInBattle())
	{
		m_pUIManager->DrawNavigation();
	}
	DrawFade();
	//m_pBattleArea->DebugDraw();
	//DrawGrid();
}

int GameScene::GetRemainingEnemies()
{
	m_remainingEnemysCount = 0;
	for (auto& normalSkelton : m_NormalSkeltons)
	{
		if (!normalSkelton->IsDead()) ++m_remainingEnemysCount;
	}
	for (auto& wizardSkelton : m_WizardSkeltons)
	{
		if (!wizardSkelton->IsDead()) ++m_remainingEnemysCount;
	}
	return m_remainingEnemysCount;
}

void GameScene::DrawGrid() const
{
	VECTOR start = VGet(-2700.0f,0.0f,0.0f); // �n�_
	VECTOR end = VGet(2700.0f, 0.0f, 0.0f);  // �I�_

	// �������̃O���b�h��for�����g���ĕ`�悷��
	// �n�_�I�_��XY���W�͕ς��Ȃ�
	// Z���W��for�����g���ĕω�������
	for (float z = -300.0f; z <= 300.0f; z += 100.0f)
	{
		start.z = z;
		end.z = z;
		DrawLine3D(start, end, 0xffffff);
	}

	// ���s�����̃O���b�h�𓯗l�Ɉ���
	start = VGet(0.0f, 0.0f, -300.0f); // �n�_
	end = VGet(0.0f, 0.0f, 300.0f);   // �I�_

	for (float x = -900.0f; x <= 900.0f; x += 100.0f)
	{
		start.x = x;
		end.x = x;
		DrawLine3D(start, end, 0xffffff);
	}
}

bool GameScene::IsAreAllEnemiesDefeated()
{
	for (auto& normalSkelton : m_NormalSkeltons)
	{
		if (!normalSkelton->IsDead()) return false;
	}
	for (auto& wizardSkelton : m_WizardSkeltons)
	{
		if (!wizardSkelton->IsDead()) return false;
	}
	return true;
}
