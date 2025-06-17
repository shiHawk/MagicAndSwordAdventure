#include "GameScene.h"
#include "DxLib.h"
#include <cmath>
GameScene::GameScene() :
	m_cameraMoveAngle(0.0f),
	m_viewAngle(DX_PI_F / 3.0f),
	m_cameraPos(VGet(0, 0, 0)),
	m_cameraTarget(VGet(0, 0, 0)),
	m_CountDownFrame(220)
{
}

void GameScene::Init()
{
	// 3D�\���̐ݒ�
	SetUseZBuffer3D(true);	  // Z�o�b�t�@���w�肷��
	SetWriteZBuffer3D(true);  // Z�o�b�t�@�ւ̏������݂��s��

	SetUseBackCulling(true);  // �|���S���̗��ʂ�\�����Ȃ�

	// �J�����̈ʒu�̏��������s��
	// 600 * 600�̃O���b�h����ʒ���������ɕ\�������J������ݒ肷��

	// �J����(�n�_)�̈ʒu
	m_cameraPos.x = 0.0f;
	m_cameraPos.y = 200.0f;
	m_cameraPos.z = -600.0f;

	// �J�������ǂ������Ă��邩(�����_)
	m_cameraTarget.x = 0.0f;
	m_cameraTarget.y = 0.0f;
	m_cameraTarget.z = 0.0f;

	// �J�����̈ʒu�ƒ����_���w�肷��
	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);

	// �J�����̎���p��ݒ肷��
	m_viewAngle = DX_PI_F / 3.0f;	// 60�x
	SetupCamera_Perspective(m_viewAngle);

	// �J������near,far��ݒ肷��
	// ��ʂɕ\������鋗���͈̔͂�ݒ肷��
	// �J��������near�ȏ㗣��Ă���far���߂��ɂ�����̂�
	// �Q�[����ʂɕ\�������
	// far�͂��܂�傫�����鐔����ݒ肵�Ȃ��悤�ɋC��t����(�\���o�O�Ɍq����)
	SetCameraNearFar(10.0f, 1000.0f);

	CreateDirLightHandle(VGet(-0.577f,-0.577f,0.577));

	m_pPlayer = std::make_shared<Player>();
	m_pEnemy = std::make_shared<Enemy>();
	m_pCollision = std::make_shared<Collision>();

	m_pPlayer->Init(m_pEnemy);
	m_pEnemy->Init(m_pCollision);
	m_pCollision->Init(m_pPlayer, m_pEnemy);
}

void GameScene::End()
{
	m_pPlayer->End();
	m_pCollision->End();
}

SceneBase* GameScene::Update()
{
	m_pPlayer->Update();
	m_pEnemy->Update();
	m_pCollision->Update();
	return this;
}

void GameScene::Draw()
{
	m_pPlayer->Draw();
	m_pEnemy->Draw();
	m_pCollision->Draw();
	DrawGrid();
}

void GameScene::DrawGrid() const
{
	// VECTOR�\����
	// 3D���W��\������̂ɕK�v��x,y,z��3�������o�[�Ƃ��Ď��\����

	VECTOR start = VGet(-900.0f,0.0f,0.0f); // �n�_
	VECTOR end = VGet(900.0f, 0.0f, 0.0f);   // �I�_

	// �������̃O���b�h��for�����g���ĕ`�悷��
	// �n�_�I�_��XY���W�͕ς��Ȃ�
	// Z���W��for�����g���ĕω�������
	for (int z = -300; z <= 300; z += 100)
	{
		start.z = z;
		end.z = z;

		DrawLine3D(start, end, 0xffffff);
	}

	// ���s�����̃O���b�h�𓯗l�Ɉ���
	start = VGet(0.0f, 0.0f, -300.0f); // �n�_
	end = VGet(0.0f, 0.0f, 300.0f);   // �I�_

	for (int x = -900; x <= 900; x += 100)
	{
		start.x = x;
		end.x = x;

		DrawLine3D(start, end, 0xffffff);
	}
}
