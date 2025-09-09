#include "GameScene.h"
#include "SoundManager.h"
#include "DxLib.h"
#include <cmath>
#include "game.h"
#include "Pad.h"
#include "ResultScene.h"
namespace
{
	constexpr float kGroundPosY = 20.0f;
	// �L�����̈ʒu�ƃT�C�Y����e�̒��_�z������
	constexpr float kLeftShadow = -30.0f;
	constexpr float kRightShadow = 40.0f;
	constexpr float kFrontShadow = 25.0f;
	constexpr float kBackShadow = -25.0f;
}
GameScene::GameScene():
	m_isNextScene(false),
	m_remainingEnemysCount(30),
	m_isHitPlayer(false),
	m_wasHitPlayer(false),
	m_isHitNormalSkelton(false),
	m_isHitWizardSkelton(false),
	m_wasHitNormalSkelton(false),
	m_wasHitWizardSkelton(false),
	m_shadowGraphHandle(-1),
	m_shadowPos({0.0f,0.0f,0.0f}),
	m_shadowAlpha(0)
{
}

void GameScene::LoadEnemyData(const std::string fileName, std::vector<std::shared_ptr<NormalSkelton>>& normalSkeltons, std::vector<std::shared_ptr<WizardSkelton>>& wizardSkeltons, 
							  std::shared_ptr<Player> pPlayer, std::shared_ptr<ScoreManager> pScoreManager)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		printfDx("CSV�t�@�C�����J���܂���ł���: %s\n", fileName.c_str());
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
	m_pEffectManager = std::make_shared<EffectManager>();
	m_pUIManager = std::make_unique<UIManager>();
	m_pCamera->Init(m_pPlayer);
	m_pScoreManager->Init();
	m_pStage->Init();
	m_pPlayer->Init(m_pAnimation);
	m_pBattleArea->Init(m_pPlayer, m_pCamera);
	m_pBattleArea->SetEnemys(m_NormalSkeltons,m_WizardSkeltons);
	m_pUIManager->Init(m_pPlayer, m_pScoreManager,m_pCamera);
	m_pCollision->Init(m_pPlayer,m_NormalSkeltons, m_WizardSkeltons);
	m_pEffectManager->Init(m_pCollision);
	m_pAnimation->Init();
	SoundManager::GetInstance()->PlayBGM();
	m_remainingEnemysCount = 0;
	m_isHitNormalSkelton = false;
	m_isHitWizardSkelton = false;
	m_isHitPlayer = false;
	m_shadowGraphHandle = LoadGraph("Data/UI/Shadow.png");
	m_shadowPos = { 0.0f,0.0f,0.0f };
	m_shadowAlpha = 255;
}

void GameScene::End()
{
	SoundManager::GetInstance()->StopBGM();
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
	m_pEffectManager->End();
	DeleteGraph(m_shadowGraphHandle);
}

SceneBase* GameScene::Update()
{
	SoundManager::GetInstance()->Update();
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
	m_pEffectManager->Update();
	UpdateFade();

	// �v���C���[�̔�eSE
	m_isHitPlayer = m_pCollision->GetIsPlayerHit();
	if (m_isHitPlayer && !m_wasHitPlayer)
	{
		SoundManager::GetInstance()->PlayEnemyAttackSE();
	}
	m_wasHitPlayer = m_isHitPlayer;
	
	// NormalSkelton�̔�eSE
	const auto& hitResults = m_pCollision->GetNormalSkeltonHit();
	m_isHitNormalSkelton = false;
	for (bool hit : hitResults)
	{
		if (hit)
		{
			m_isHitNormalSkelton = true;
			break; // ��ł�true��������΃��[�v�𔲂���
		}
	}
	if (m_isHitNormalSkelton && !m_wasHitNormalSkelton)
	{
		SoundManager::GetInstance()->PlayPlayerAttackSE(m_pPlayer->GetAttackCount());
	}
	m_wasHitNormalSkelton = m_isHitNormalSkelton; // �q�b�g���̍X�V

	// WizardSkelton�̔�eSE
	const auto& hitWizardResults = m_pCollision->GetWizardSkeltonHit();
	m_isHitWizardSkelton = false;
	for (bool hit : hitWizardResults)
	{
		if (hit)
		{
			m_isHitWizardSkelton = true;
			break; // ��ł�true��������΃��[�v�𔲂���
		}
	}
	if (m_isHitWizardSkelton && !m_wasHitWizardSkelton)
	{
		SoundManager::GetInstance()->PlayPlayerAttackSE(m_pPlayer->GetAttackCount());
	}
	m_wasHitWizardSkelton = m_isHitWizardSkelton; // �q�b�g���̍X�V

	if (m_pPlayer->IsDead()) // �v���C���[�����S������
	{
		m_pScoreManager->SetIsPlayerDead(true);
	}
	if (!m_isNextScene && !IsFadingOut() && (m_pPlayer->IsDead() || IsAreAllEnemiesDefeated()))
	{
		StartFadeOut();
		
		m_isNextScene = true;
	}
	if (IsFadingOut())
	{
		SoundManager::GetInstance()->FadeBGMVol(); // �t�F�[�h�A�E�g�ɍ��킹�ĉ��ʂ�������
	}
	if (m_isNextScene && IsFadeComplete())
	{
		return new ResultScene(m_pScoreManager); // �t�F�[�h���I������烊�U���g�V�[���ֈڍs
	}
	m_pScoreManager->HpBonus(m_pPlayer->GetHp());
	m_shadowPos = VGet(m_pPlayer->GetPos().x, kGroundPosY, m_pPlayer->GetPos().z);
	return this;
}

void GameScene::Draw()
{
	m_pStage->Draw();
	
	DrawCharacterShadow();
	m_pPlayer->Draw();
	for (auto& normalSkelton : m_NormalSkeltons)
	{
		normalSkelton->Draw();
	}
	for (auto& wizardSkelton : m_WizardSkeltons)
	{
		wizardSkelton->Draw();
	}
	m_pUIManager->Draw();
	m_pUIManager->DrawNumberOfEnemiesRemaining(GetRemainingEnemies());
	m_pEffectManager->Draw();
	if (!m_pBattleArea->IsInBattle())
	{
		m_pUIManager->DrawNavigation();
	}
	DrawFade();
	//m_pBattleArea->DebugDraw();
}

SceneID GameScene::GetSceneID() const
{
	return SceneID::GameScene;
}

int GameScene::GetRemainingEnemies()
{
	m_remainingEnemysCount = 0;
	for (auto& normalSkelton : m_NormalSkeltons)
	{
		if (!normalSkelton->IsDead()) ++m_remainingEnemysCount; // ����ł��Ȃ�NormalSkelton�̐����J�E���g
	}
	for (auto& wizardSkelton : m_WizardSkeltons)
	{
		if (!wizardSkelton->IsDead()) ++m_remainingEnemysCount; // ����ł��Ȃ�WizardSkelton�̐����J�E���g
	}
	return m_remainingEnemysCount;
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

void GameScene::MakeShadowVertex(const VECTOR& pos, VERTEX3D* vertex)
{
	// ����
	vertex[0].pos = VGet(pos.x + kLeftShadow, 0.0f, pos.z + kFrontShadow);
	vertex[0].u = 0.0f; vertex[0].v = 0.0f;

	// �E��
	vertex[1].pos = VGet(pos.x + kRightShadow, 0.0f, pos.z + kBackShadow);
	vertex[1].u = 1.0f; vertex[1].v = 1.0f;

	// ����
	vertex[2].pos = VGet(pos.x + kLeftShadow, 0.0f, pos.z + kBackShadow);
	vertex[2].u = 0.0f; vertex[2].v = 1.0f;

	// ����(2��)
	vertex[3].pos = VGet(pos.x + kLeftShadow, 0.0f, pos.z + kFrontShadow);
	vertex[3].u = 0.0f; vertex[3].v = 0.0f;

	// �E��
	vertex[4].pos = VGet(pos.x + kRightShadow, 0.0f, pos.z + kFrontShadow);
	vertex[4].u = 1.0f; vertex[4].v = 0.0f;

	// �E��(2��)
	vertex[5].pos = VGet(pos.x + kRightShadow, 0.0f, pos.z + kBackShadow);
	vertex[5].u = 1.0f; vertex[5].v = 1.0f;

	for (int i = 0; i < 6; i++)
	{
		vertex[i].norm = VGet(0.0f, 1.0f, 0.0f);
		vertex[i].dif = GetColorU8(255, 255, 255, m_shadowAlpha);
		vertex[i].spc = GetColorU8(0, 0, 0, 0);
	}
}

void GameScene::DrawCharacterShadow()
{
	VERTEX3D vertex[6];

	// �v���C���[�̉e
	MakeShadowVertex(m_pPlayer->GetPos(), vertex);
	DrawPolygon3D(vertex, 2, m_shadowGraphHandle, TRUE);

	// �m�[�}���X�P���g���̉e
	for (auto& normalSkelton : m_NormalSkeltons)
	{
		if (normalSkelton->IsDead()) continue;
		MakeShadowVertex(normalSkelton->GetPos(), vertex);
		DrawPolygon3D(vertex, 2, m_shadowGraphHandle, TRUE);
	}

	// �E�B�U�[�h�X�P���g���̉e
	for (auto& wizardSkelton : m_WizardSkeltons)
	{
		if (wizardSkelton->IsDead()) continue;
		MakeShadowVertex(wizardSkelton->GetPos(), vertex);
		DrawPolygon3D(vertex, 2, m_shadowGraphHandle, TRUE);
	}
}
