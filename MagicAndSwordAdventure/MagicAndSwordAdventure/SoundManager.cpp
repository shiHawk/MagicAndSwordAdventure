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
	m_playerFirstAttackSoundHandle(-1),
	m_playerSecondAttackSoundHandle(-1),
	m_playerThirdAttackSoundHandle(-1),
	m_enemyAttackSoundHandle(-1),
	m_soundVol(0),
	m_currentBgmHandle(-1)
{
}

void SoundManager::Init(SceneManager* pSceneManager)
{
	m_pSceneManager = pSceneManager;
	// 各シーンのBGMをロード
	m_titleBgmHandle = LoadSoundMem("Data/sound/titlebgm.mp3");
	m_gameSceneBgmHandle = LoadSoundMem("Data/sound/gamebgm.mp3");
	m_resultBgmHandle = LoadSoundMem("Data/sound/resultbgm.mp3");
	m_playerFirstAttackSoundHandle = LoadSoundMem("Data/sound/firstattack.mp3");
	m_playerSecondAttackSoundHandle = LoadSoundMem("Data/sound/secondattack.mp3");
	m_playerThirdAttackSoundHandle = LoadSoundMem("Data/sound/thirdattack.mp3");
	m_enemyAttackSoundHandle = LoadSoundMem("Data/sound/enemyatack.mp3");
}

void SoundManager::End()
{
	// BGMのハンドルを削除
	DeleteSoundMem(m_titleBgmHandle);
	DeleteSoundMem(m_gameSceneBgmHandle);
	DeleteSoundMem(m_resultBgmHandle);
	DeleteSoundMem(m_playerFirstAttackSoundHandle);
	DeleteSoundMem(m_playerSecondAttackSoundHandle);
	DeleteSoundMem(m_playerThirdAttackSoundHandle);
	DeleteSoundMem(m_enemyAttackSoundHandle);
}

void SoundManager::Update()
{
	if (m_currentBgmHandle != -1)
	{
		ChangeVolumeSoundMem(kMaxVol * 0.8 - m_soundVol, m_currentBgmHandle); // 音量を調整
	}
}

void SoundManager::PlayBGM()
{
	m_soundVol = 0;
	// 各シーンに合わせてm_currentBgmHandleを変更する
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
		PlaySoundMem(m_currentBgmHandle, DX_PLAYTYPE_LOOP, true);// 現在のBGMを再生
	}
}

void SoundManager::PlayPlayerAttackSE(int attackCount)
{
	switch (attackCount)
	{
	case 1:
		PlaySoundMem(m_playerFirstAttackSoundHandle, DX_PLAYTYPE_BACK);
		break;
	case 2:
		PlaySoundMem(m_playerSecondAttackSoundHandle, DX_PLAYTYPE_BACK);
		break;
	case 3:
		PlaySoundMem(m_playerThirdAttackSoundHandle, DX_PLAYTYPE_BACK);
		break;
	}
}

void SoundManager::PlayEnemyAttackSE()
{
	PlaySoundMem(m_enemyAttackSoundHandle, DX_PLAYTYPE_BACK);
}

int SoundManager::FadeBGMVol()
{
	// 徐々に音量を下げる
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
		StopSoundMem(m_currentBgmHandle);
		m_currentBgmHandle = -1; // クリアしておく
	}
}
