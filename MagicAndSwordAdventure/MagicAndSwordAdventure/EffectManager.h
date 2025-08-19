#pragma once
#include "Collision.h"
#include "DxLib.h"
#include <memory>
#include <vector>
class EffectManager
{
public:
	EffectManager();
	~EffectManager();
	void Init(std::shared_ptr<Collision> pCollision);
	void End();
	void Update();
	void Draw();
	void SetHitEffectPosPlayer();
	void SetHitEffectPosNormalSkelton();
	void SetHitEffectPosWizardSkelton();
private:
	int m_effectHandle;
	int m_playEffectHandle;
	std::shared_ptr<Collision> m_pCollision;
	bool m_wasHitPlayer;
	bool m_isHitPlayer;
};

