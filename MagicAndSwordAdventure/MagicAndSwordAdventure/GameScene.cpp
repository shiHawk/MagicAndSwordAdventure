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
	m_pCollision = std::make_shared<Collision>();
	// NormalSkelton �𕡐�����
	for (int i = 0; i < 1; ++i)
	{
		auto normalSkelton = std::make_shared<NormalSkelton>();
		normalSkelton->Init(m_pPlayer);
		m_NormalSkeltons.push_back(normalSkelton);
	}
	// WizardSkelton �𕡐�����
	for (int i = 0; i < 1; ++i)
	{
		auto wizard = std::make_shared<WizardSkelton>();
		wizard->Init(m_pPlayer);
		m_WizardSkeltons.push_back(wizard);
	}
	m_pAnimation = std::make_shared<Animation>();
	m_pNormalSkelton = std::make_shared<NormalSkelton>();
	m_pWizardSkelton = std::make_shared<WizardSkelton>();

	m_pCamera->Init(m_pPlayer);
	m_pPlayer->Init(m_pAnimation);
	m_pNormalSkelton->Init(m_pPlayer);
	m_pWizardSkelton->Init(m_pPlayer);
	m_pCollision->Init(m_pPlayer, m_pEnemy, m_pNormalSkelton, m_pWizardSkelton);
	m_pAnimation->Init();
}

void GameScene::End()
{
	m_pPlayer->End();
	//m_pCollision->End();
	for (auto& normalSkelton : m_NormalSkeltons)
	{
		normalSkelton->End();
	}
	for (auto& wizard : m_WizardSkeltons)
	{
		wizard->End();
	}
	m_pNormalSkelton->End();
	m_pWizardSkelton->End();
}

SceneBase* GameScene::Update()
{
	m_pCamera->Update();
	m_pPlayer->Update();
	for (auto& normalSkelton : m_NormalSkeltons)
	{
		normalSkelton->Update();
	}
	for (auto& wizard : m_WizardSkeltons)
	{
		wizard->Update();
	}
	m_pNormalSkelton->Update();
	m_pWizardSkelton->Update();
	m_pCollision->Update();
	return this;
}

void GameScene::Draw()
{
	m_pPlayer->Draw();
	for (auto& normalSkelton : m_NormalSkeltons)
	{
		normalSkelton->Draw();
	}
	for (auto& wizard : m_WizardSkeltons)
	{
		wizard->Draw();
	}
	m_pNormalSkelton->Draw();
	m_pWizardSkelton->Draw();
	m_pCollision->Draw();
	DrawGrid();
}

void GameScene::DrawGrid() const
{
	// VECTOR�\����
	// 3D���W��\������̂ɕK�v��x,y,z��3�������o�[�Ƃ��Ď��\����

	VECTOR start = VGet(-2700.0f,0.0f,0.0f); // �n�_
	VECTOR end = VGet(2700.0f, 0.0f, 0.0f);   // �I�_

	// �������̃O���b�h��for�����g���ĕ`�悷��
	// �n�_�I�_��XY���W�͕ς��Ȃ�
	// Z���W��for�����g���ĕω�������
	for (int z = -300; z <= 300; z += 100)
	{
		start.z = z;
		end.z = z;

		DrawLine3D(start, end, 0xffffff);
	}

	// ���s�����̃O���b�h�𓯗l�Ɉ���
	start = VGet(0.0f, 0.0f, -300.0f); // �n�_
	end = VGet(0.0f, 0.0f, 300.0f);   // �I�_

	for (int x = -900; x <= 900; x += 100)
	{
		start.x = x;
		end.x = x;

		DrawLine3D(start, end, 0xffffff);
	}
}
