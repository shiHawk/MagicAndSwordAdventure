#include "SoundManager.h"
#include "SceneManager.h"
namespace
{
	constexpr int kMaxVol = 255;
}
SoundManager::SoundManager():
	m_titleBgmHandle(-1),
	m_gameSceneBgmHandle(-1),
	m_resultBgmHandle(-1),
	m_playerAttackSoundHandle(-1),
	m_enemyAttackSoundHandle(-1),
	m_soundVol(0),
	m_currentBgmHandle(-1)
{
}

void SoundManager::Init(SceneManager* pSceneManager)
{
	m_pSceneManager = pSceneManager;
	m_titleBgmHandle = LoadSoundMem("Data/sound/titlebgm.mp3");
	m_gameSceneBgmHandle = LoadSoundMem("Data/sound/gamebgm.mp3");
	m_resultBgmHandle = LoadSoundMem("Data/sound/resultbgm.mp3");
}

void SoundManager::End()
{
	DeleteSoundMem(m_titleBgmHandle);
	DeleteSoundMem(m_gameSceneBgmHandle);
	DeleteSoundMem(m_resultBgmHandle);
}

void SoundManager::Update()
{
	if (m_currentBgmHandle != -1)
	{
		ChangeVolumeSoundMem(kMaxVol * 0.8 - m_soundVol, m_currentBgmHandle);
	}
}

void SoundManager::PlayBGM()
{
	m_soundVol = 0;
	if (m_pSceneManager->GetCurrentSceneID() == SceneID::TitleScene)
	{
		m_currentBgmHandle = m_titleBgmHandle;
	}
	if (m_pSceneManager->GetCurrentSceneID() == SceneID::GameScene)
	{
		m_currentBgmHandle = m_gameSceneBgmHandle;
	}
	if (m_pSceneManager->GetCurrentSceneID() == SceneID::ResultScene)
	{
		m_currentBgmHandle = m_resultBgmHandle;
	}
	if (m_currentBgmHandle != -1)
	{
		PlaySoundMem(m_currentBgmHandle, DX_PLAYTYPE_LOOP, true);
	}
}

int SoundManager::FadeBGMVol()
{
	m_soundVol += 8;
	if (m_soundVol > kMaxVol * 0.8)
	{
		m_soundVol = kMaxVol * 0.8;
	}
	return m_soundVol;
}

void SoundManager::StopBGM()
{
	if (m_currentBgmHandle != -1) {
		StopSoundMem(m_currentBgmHandle);  // © ‚±‚Á‚¿‚ğg‚¤
		m_currentBgmHandle = -1;           // ƒNƒŠƒA‚µ‚Ä‚¨‚­
	}
}
