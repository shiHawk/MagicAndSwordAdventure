#include "SceneManager.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "Pad.h"

SceneManager::SceneManager()
{
	m_pScene = nullptr;
}

SceneManager::~SceneManager()
{
}

void SceneManager::Init()
{
	m_pScene = new GameScene;
	m_pScene->Init();
}

void SceneManager::End()
{
	if (!m_pScene)	return;

	m_pScene->End();
	delete m_pScene;
}

void SceneManager::Update()
{
	if (!m_pScene)	return;

	Pad::Update();
	SceneBase* pScene = m_pScene->Update();
	if (pScene != m_pScene)
	{
		// �O�̃V�[���̏I������
		m_pScene->End();
		delete m_pScene;

		m_pScene = pScene;
		m_pScene->Init();
	}
}

void SceneManager::Draw()
{
	if (!m_pScene)	return;

	m_pScene->Draw();
}
