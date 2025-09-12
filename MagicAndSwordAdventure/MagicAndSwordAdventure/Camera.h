#pragma once
#include "DxLib.h"
#include <memory>
#include "Player.h"
class Camera
{
public:
	Camera();
	~Camera();
	void Init(std::shared_ptr<Player> pPlayer);
	void End();
	void Update();
	void ChangeBattleCamera(VECTOR cameraTarget);
	void ChangeNormalCamera() { m_isBattleCamera = false; }
	bool IsBattleCamera() { return m_isBattleCamera; }
private:
	int m_lightHandle;
	// カメラの位置
	VECTOR m_cameraPos;
	// カメラの注視点(見ている場所)
	VECTOR m_cameraTarget;
	// カメラの目標移動位置
	VECTOR m_cameraMoveTargetPos;
	bool m_isBattleCamera;

	std::shared_ptr<Player> m_pPlayer;
};

