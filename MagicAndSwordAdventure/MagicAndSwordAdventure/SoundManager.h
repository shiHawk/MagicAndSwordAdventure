#pragma once
#include "DxLib.h"
#include <memory>
enum class Scene
{
	TitleScene,
	GameScene,
	ResultScene
};
class SceneManager;
class SoundManager
{
public:
	// シングルトンインスタンスを取得するメソッド
	static SoundManager* GetInstance()
	{
		static SoundManager instance; // 一度だけインスタンスが呼ばれる
		return &instance;
	}
	// コピーコンストラクタと代入演算子を禁止
	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;
	
	void Init(SceneManager* pSceneManager);
	void End();
	void Update();
	void PlayBGM();
	int FadeBGMVol(); // フェードに合わせて音量を小さくする
	void StopBGM();
private:
	// privateにすることで外部からインスタンスの生成・破棄をできなくする
	SoundManager();
	~SoundManager() {};
	SceneManager* m_pSceneManager;
	//Scene m_scene;
	int m_soundVol;
	int m_currentBgmHandle;
	int m_titleBgmHandle;
	int m_gameSceneBgmHandle;
	int m_resultBgmHandle;
	int m_playerAttackSoundHandle;
	int m_enemyAttackSoundHandle;
};

