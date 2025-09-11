#include "Stage.h"

namespace
{
	// �^�C���ƕǂ̈ʒu
	constexpr float kFrontTilePosZ = -300.0f;   // �O��^�C����Z���W
	constexpr float kBackTilePosZ = 100.0f;     // ���^�C����Z���W
	constexpr float kFrontWallPosZ = 350.0f;    // �O��ǂ�Z���W

	// �h�A�ƃQ�[�g�t���[���̈ʒu
	constexpr VECTOR kWallDoorPos = { -2700.0f, 0.0f, 0.0f };       // �h�A�ʒu
	constexpr VECTOR kGateFramePos = { 5300.0f, 0.0f, -150.0f };    // �Q�[�g�ʒu

	// �X�e�[�W�\��
	constexpr int kTileCycle = 3;               // 3�̔{�����Ƃɓ���^�C��
	constexpr int kWallCycle = 6;               // 6�̔{�����Ƃɓ����
	constexpr int kInitialTileCount = 40;       // �����^�C����
	constexpr int kInitialWallCount = 20;       // �����ǐ�

	// �X�e�[�W�͈�
	constexpr int kStageStartPos = -2500;       // �X�e�[�W�J�n�ʒu
	constexpr int kStageEndPos = 5500;          // �X�e�[�W�I���ʒu

	// �T�C�Y
	constexpr float kTileSize = 400.0f;         // �^�C���T�C�Y
	constexpr float kWallSize = 400.0f;         // �ǃT�C�Y

	// ���f���̃X�P�[���Ɖ�]
	constexpr float kWallScaleX = 2.0f;         // �ǂ�X�����X�P�[��
	constexpr float kWallScaleYZ = 1.0f;        // �ǂ�Y�EZ�����X�P�[��
	constexpr float kDoorRotationY = -DX_PI / 2.0f; // �h�A��]�p�x�i90�x�j

	// �����ʒu�I�t�Z�b�g
	constexpr float kTileWallInitialPosY = -10.0f; // �^�C���E�ǂ�Y�����ʒu
	constexpr float kTileWallInitialPosZ = 100.0f; // �^�C���E�ǂ�Z�����ʒu
}

Stage::Stage():
	m_tileTotal(-1),
	m_stageStart(-1),
	m_stageEnd(-1),
	m_tileGrateModelBase(-1),
	m_tileModelBase(-1),
	m_tilePos({0.0f,0.0f,0.0f}),
	m_tileSize(0),
	m_wallCrackedModelBase(-1),
	m_wallDoorModelHandle(-1),
	m_wallModelBase(-1),
	m_wallNum(0),
	m_wallPos({ 0.0f,0.0f,0.0f }),
	m_wallSize(0),
	m_gateFrameModelHandle(-1)
{
}

Stage::~Stage()
{
}

void Stage::Init()
{
	m_tilePos = { 0.0f,kTileWallInitialPosY,kTileWallInitialPosZ };
	m_wallPos = { 0.0f,kTileWallInitialPosY,kTileWallInitialPosZ };
	m_tileTotal = kInitialTileCount;
	m_wallNum = kInitialWallCount;
	m_stageStart = kStageStartPos;
	m_stageEnd = kStageEndPos;
	m_tileSize = kTileSize;
	m_wallSize = kWallSize;
	m_tileModelBase = MV1LoadModel("Data/model/floor_tile_large.mv1");
	m_tileGrateModelBase = MV1LoadModel("Data/model/floor_tile_grate_open.mv1");
	m_wallModelBase = MV1LoadModel("Data/model/wall.mv1");
	m_wallCrackedModelBase = MV1LoadModel("Data/model/wall_cracked.mv1");
	m_wallDoorModelHandle = MV1LoadModel("Data/model/wall_doorway.mv1");
	m_gateFrameModelHandle = MV1LoadModel("Data/model/wall_open_scaffold.mv1");
	//�h�A�Ɩ���g��A��]
	MV1SetScale(m_wallDoorModelHandle, VGet(kWallScaleX, kWallScaleYZ, kWallScaleYZ));
	MV1SetRotationXYZ(m_wallDoorModelHandle, VGet(0.0f, kDoorRotationY, 0.0f));

	MV1SetScale(m_gateFrameModelHandle, VGet(kWallScaleX, kWallScaleYZ, kWallScaleYZ));
	MV1SetRotationXYZ(m_gateFrameModelHandle, VGet(0.0f, kDoorRotationY, 0.0f));

	if (m_tileModelBase == -1 || m_tileGrateModelBase == -1)
	{
		printfDx("�ǂݍ��ݎ��s\n");
		return;
	}

	m_tileModelHandles.resize(m_tileTotal, -1);
	m_tileGrateModelHandles.resize(m_tileTotal, -1);
	m_wallModelHandles.resize(m_wallNum, -1);
	m_wallCrackedModelHandles.resize(m_wallNum, -1);
	
	// �R�s�[�����畡��
	for (int i = 0; i < m_tileTotal;i++)
	{
		if (i % kTileCycle == 0)
		{
			m_tileGrateModelHandles[i] = MV1DuplicateModel(m_tileGrateModelBase);
		}
		else
		{
			m_tileModelHandles[i] = MV1DuplicateModel(m_tileModelBase);
		}
	}
	for (int i = 0; i < m_wallNum;i++)
	{
		if (i % kWallCycle == 0)
		{
			m_wallCrackedModelHandles[i] = MV1DuplicateModel(m_wallCrackedModelBase);
		}
		else
		{
			m_wallModelHandles[i] = MV1DuplicateModel(m_wallModelBase);
		}
	}
}

void Stage::End()
{
	for (int tileModelHandle : m_tileModelHandles)
	{
		if (tileModelHandle != -1) MV1DeleteModel(tileModelHandle);
	}
	for (int tileGrateModelHandle : m_tileGrateModelHandles)
	{
		if (tileGrateModelHandle != -1) MV1DeleteModel(tileGrateModelHandle);
	}
	for (int wallModelHandle : m_wallModelHandles)
	{
		if (wallModelHandle != -1) MV1DeleteModel(wallModelHandle);
	}
	for (int wallCrackedModelHandle : m_wallCrackedModelHandles)
	{
		if (wallCrackedModelHandle != -1) MV1DeleteModel(wallCrackedModelHandle);
	}

	m_tileModelHandles.clear();
	m_tileGrateModelHandles.clear();
	m_wallModelHandles.clear();
	m_wallCrackedModelHandles.clear();
	if (m_tileModelBase != -1) MV1DeleteModel(m_tileModelBase);
	if (m_tileGrateModelBase != -1) MV1DeleteModel(m_tileGrateModelBase);
	if (m_wallModelBase != -1) MV1DeleteModel(m_wallModelBase);
	if (m_wallCrackedModelBase != -1) MV1DeleteModel(m_wallCrackedModelBase);
	MV1DeleteModel(m_wallDoorModelHandle);
	MV1DeleteModel(m_gateFrameModelHandle);
}

void Stage::Updata()
{
	// �ǂ�pos�̐ݒ�
	for (int i = 0; i < m_tileTotal;i++)
	{
		if (i < m_tileTotal / 2)
		{
			m_tilePos.x = m_stageStart + i * m_tileSize;
			m_tilePos.z = kBackTilePosZ;
		}
		// ����������2�i�ڂ�
		else
		{
			m_tilePos.x = m_stageStart + i * m_tileSize - m_stageEnd + m_stageStart;
			m_tilePos.z = kFrontTilePosZ;
		}

		if (i % kTileCycle == 0 && m_tileGrateModelHandles[i] != -1)
		{
			MV1SetPosition(m_tileGrateModelHandles[i], m_tilePos);
		}
		else if(m_tileModelHandles[i] != -1)
		{
			MV1SetPosition(m_tileModelHandles[i], m_tilePos);
		}
	}
	// ����pos�̐ݒ�
	for (int i = 0; i < m_wallNum;i++)
	{
		m_wallPos.x = m_stageStart + i * m_wallSize;
		m_wallPos.z = kFrontWallPosZ;
		if (i % kWallCycle == 0 && m_wallCrackedModelHandles[i] != -1)
		{
			MV1SetPosition(m_wallCrackedModelHandles[i], m_wallPos);
		}
		else
		{
			MV1SetPosition(m_wallModelHandles[i], m_wallPos);
		}
	}
	MV1SetPosition(m_wallDoorModelHandle, kWallDoorPos);
	MV1SetPosition(m_gateFrameModelHandle, kGateFramePos);
}

void Stage::Draw()
{
	for (int i = 0; i < m_tileTotal;i++)
	{
		if (i % kTileCycle == 0 && m_tileGrateModelHandles[i] != -1)
		{
			MV1DrawModel(m_tileGrateModelHandles[i]);
		}
		else if (m_tileModelHandles[i] != -1)
		{
			MV1DrawModel(m_tileModelHandles[i]);
		}
	}
	for (int i = 0; i < m_wallNum;i++)
	{
		if (i % kWallCycle == 0 && m_wallCrackedModelHandles[i] != -1)
		{
			MV1DrawModel(m_wallCrackedModelHandles[i]);
		}
		else
		{
			MV1DrawModel(m_wallModelHandles[i]);
		}
	}
	MV1DrawModel(m_wallDoorModelHandle);
	MV1DrawModel(m_gateFrameModelHandle);
}
