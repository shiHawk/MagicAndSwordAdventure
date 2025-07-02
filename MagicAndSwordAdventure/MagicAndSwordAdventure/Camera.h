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
	// �J�����̈ʒu
	VECTOR m_cameraPos;
	// �J�����̒����_(���Ă���ꏊ)
	VECTOR m_cameraTarget;
	// �J�����̖ڕW�ړ��ʒu
	VECTOR m_cameraMoveTargetPos;
	// �J�����̎���p
	float m_viewAngle;
	// �J�����ړ����o
	float m_cameraMoveAngle;
	int m_CountDownFrame;

	std::shared_ptr<Player> m_pPlayer;
};

