#pragma once
#include "SceneBase.h"
class SceneManager 
{
public:
	SceneManager();
	virtual ~SceneManager();

	void Init();
	void End();

	void Update();
	void Draw();
	SceneID GetCurrentSceneID()const;
private:
	SceneBase* m_pScene;
};

