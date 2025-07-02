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
	void Update();
private:
	// カメラの位置
	VECTOR m_cameraPos;
	// カメラの注視点(見ている場所)
	VECTOR m_cameraTarget;
	// カメラの目標移動位置
	VECTOR m_cameraMoveTargetPos;
	// カメラの視野角
	float m_viewAngle;
	// カメラ移動演出
	float m_cameraMoveAngle;
	int m_CountDownFrame;

	std::shared_ptr<Player> m_pPlayer;
};

