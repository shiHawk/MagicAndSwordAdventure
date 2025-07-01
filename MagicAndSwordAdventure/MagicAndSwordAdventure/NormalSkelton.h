#pragma once
#include "Enemy.h"
class NormalSkelton : public Enemy
{
public:
	void Init();
	void Update() override;
	void DoAttack();
private:
	float m_serchRange;
};

