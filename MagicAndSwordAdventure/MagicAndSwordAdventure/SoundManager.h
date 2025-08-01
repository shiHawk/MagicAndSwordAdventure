#pragma once
#include "DxLib.h"
#include "SceneManager.h"
#include <memory>
enum class Scene
{
	TitleScene,
	GameScene,
	ResultScene
};
class SoundManager
{
public:
	SoundManager();
	~SoundManager() {};
	void Init(SceneManager pSceneManager);
	void End();
	void Update();
	void PlayBGM();
	int FadeBGMVol(); // フェードに合わせて音量を小さくする
	void StopBGM();
private:
	SceneManager m_SceneManager;
	//Scene m_scene;
	int m_soundVol;
	int m_currentBgmHandle;
	int m_titleBgmHandle;
	int m_gameSceneBgmHandle;
	int m_resultBgmHandle;
	int m_playerAttackSoundHandle;
	int m_enemyAttackSoundHandle;
};

