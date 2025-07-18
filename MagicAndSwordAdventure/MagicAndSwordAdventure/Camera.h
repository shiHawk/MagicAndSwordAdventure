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
private:
	int m_lightHandle;
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

	bool m_isBattleCamera;

	std::shared_ptr<Player> m_pPlayer;
};

