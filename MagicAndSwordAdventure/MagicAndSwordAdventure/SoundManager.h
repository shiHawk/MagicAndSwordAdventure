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
	// �V���O���g���C���X�^���X���擾���郁�\�b�h
	static SoundManager* GetInstance()
	{
		static SoundManager instance; // ��x�����C���X�^���X���Ă΂��
		return &instance;
	}
	// �R�s�[�R���X�g���N�^�Ƒ�����Z�q���֎~
	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;
	
	void Init(SceneManager* pSceneManager);
	void End();
	void Update();
	void PlayBGM();
	void PlayPlayerAttackSE(int attackCount); // SE�̍Đ�
	void PlayEnemyAttackSE();
	int FadeBGMVol(); // �t�F�[�h�ɍ��킹�ĉ��ʂ�����������
	void StopBGM();
private:
	// private�ɂ��邱�ƂŊO������C���X�^���X�̐����E�j�����ł��Ȃ�����
	SoundManager();
	~SoundManager() {};
	SceneManager* m_pSceneManager;
	//Scene m_scene;
	int m_soundVol;
	int m_currentBgmHandle;
	int m_titleBgmHandle;
	int m_gameSceneBgmHandle;
	int m_resultBgmHandle;
	int m_playerFirstAttackSoundHandle; // ��i�ڂ̍U��
	int m_playerSecondAttackSoundHandle; // ��i�ڂ̍U��
	int m_playerThirdAttackSoundHandle; // �O�i�ڂ̍U��
	int m_enemyAttackSoundHandle;
};

