#pragma once
#include "DxLib.h"
class SoundManager
{
public:
	SoundManager();
	~SoundManager() {};
	void Init();
	void End();
	void Update();
private:
	int m_titleBgmHandle;
	int m_gameSceneBgmHandle;
	int m_resultBgmHandle;
	int m_playerAttackSoundHandle;
	int m_enemyAttackSoundHandle;
};

