#pragma once
#include "SceneBase.h"
#include "Player.h"
#include "Enemy.h"
#include "Collision.h"
#include "Animation.h"
#include "NormalSkelton.h"
#include "WizardSkelton.h"
#include "Camera.h"
#include "Stage.h"
#include "SceneManager.h"
#include "BattleAreaManager.h"
#include "UIManager.h"
#include "ScoreManager.h"
#include <memory>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include "DxLib.h"

class GameScene :public SceneBase
{
public:
	GameScene();
	virtual ~GameScene() {}
	void LoadEnemyData(const std::string fileName, std::vector<std::shared_ptr<NormalSkelton>>& normalSkeltons,
		std::vector<std::shared_ptr<WizardSkelton>>& wizardSkeltons, std::shared_ptr<Player> pPlayer,
		std::shared_ptr<ScoreManager> pScoreManager);
	virtual void Init() override;
	virtual void End() override;

	virtual SceneBase* Update() override;
	virtual void Draw() override;
	virtual SceneID GetSceneID() const override;
	int GetRemainingEnemies();
private:
	bool IsAreAllEnemiesDefeated(); // 全滅しているかどうか
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Enemy> m_pEnemy;
	std::shared_ptr<Collision> m_pCollision;
	std::shared_ptr<Animation> m_pAnimation;
	std::vector<std::shared_ptr<NormalSkelton>> m_NormalSkeltons;
	std::vector<std::shared_ptr<WizardSkelton>> m_WizardSkeltons;
	std::shared_ptr<Camera> m_pCamera;
	std::shared_ptr<Stage> m_pStage;
	std::shared_ptr<ScoreManager> m_pScoreManager;
	std::unique_ptr<BattleAreaManager> m_pBattleArea;
	std::unique_ptr<UIManager> m_pUIManager;
	bool m_isNextScene;
	int m_remainingEnemysCount;
};

