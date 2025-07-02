#pragma once
#include "Enemy.h"
#include "Player.h"
#include <memory>
class NormalSkelton : public Enemy
{
public:
	NormalSkelton();
	void Init(std::shared_ptr<Player> pPlayer);
	void End();
	void Update() override;
	void DoAttack();
	void Draw()const;
private:
	float m_serchRange;
	std::shared_ptr<Player> m_pPlayer;
};

