#include "Stage.h"

namespace
{
	constexpr float kInFrontPosZ = -300.0f;
	constexpr float kInFrontWallPosZ = 350.0f;
	constexpr float kBackPosZ = 100.0f;
	constexpr VECTOR kWallDoorPos = { -2700.0f,0.0f,0.0f };
	constexpr VECTOR kGateFramePos = { 5300.0f,0.0f,-150.0f };
	constexpr int kTileCycle = 3; // 3の倍数になる度タイル2を描画
	constexpr int kWallCycle = 6; // 6の倍数になる度壁2を描画
}

Stage::Stage():
	m_tileTotal(-1),
	m_stageStart(-1),
	m_stageEnd(-1),
	m_tileGrateModelBase(-1),
	m_tileModelBase(-1),
	m_tilePos({0.0f,0.0f,0.0f}),
	m_tileSize(400),
	m_wallCrackedModelBase(-1),
	m_wallDoorModelHandle(-1),
	m_wallModelBase(-1),
	m_wallNum(20),
	m_wallPos({ 0.0f,0.0f,0.0f }),
	m_wallSize(400),
	m_gateFrameModelHandle(-1)
{
}

Stage::~Stage()
{
}

void Stage::Init()
{
	m_tilePos = { 0.0f,-10.0f,100.0f };
	m_wallPos = { 0.0f,-10.0f,100.0f };
	m_tileTotal = 40;
	m_wallNum = 20;
	m_stageStart = -2500;
	m_stageEnd = 5500;
	m_tileSize = 400.0f;
	m_wallSize = 400.0f;
	m_tileModelBase = MV1LoadModel("Data/model/floor_tile_large.mv1");
	m_tileGrateModelBase = MV1LoadModel("Data/model/floor_tile_grate_open.mv1");
	m_wallModelBase = MV1LoadModel("Data/model/wall.mv1");
	m_wallCrackedModelBase = MV1LoadModel("Data/model/wall_cracked.mv1");
	m_wallDoorModelHandle = MV1LoadModel("Data/model/wall_doorway.mv1");
	m_gateFrameModelHandle = MV1LoadModel("Data/model/wall_open_scaffold.mv1");
	// ドアと門の壁を拡大、回転させる
	MV1SetScale(m_wallDoorModelHandle,VGet(2.0f,1.0f,1.0f));
	MV1SetRotationXYZ(m_wallDoorModelHandle, VGet(0.0f, -DX_PI / 2.0f, 0.0f));
	MV1SetScale(m_gateFrameModelHandle, VGet(2.0f, 1.0f, 1.0f));
	MV1SetRotationXYZ(m_gateFrameModelHandle, VGet(0.0f, -DX_PI / 2.0f, 0.0f));

	if (m_tileModelBase == -1 || m_tileGrateModelBase == -1)
	{
		printfDx("読み込み失敗\n");
		return;
	}

	m_tileModelHandles.resize(m_tileTotal, -1);
	m_tileGrateModelHandles.resize(m_tileTotal, -1);
	m_wallModelHandles.resize(m_wallNum, -1);
	m_wallCrackedModelHandles.resize(m_wallNum, -1);
	
	// コピー元から複製
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
	for (int i = 0; i < m_tileTotal;i++)
	{
		if (i < m_tileTotal / 2)
		{
			m_tilePos.x = m_stageStart + i * m_tileSize;
			m_tilePos.z = kBackPosZ;
		}
		// 半分から先は2段目に
		else
		{
			m_tilePos.x = m_stageStart + i * m_tileSize - m_stageEnd + m_stageStart;
			m_tilePos.z = kInFrontPosZ;
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
	for (int i = 0; i < m_wallNum;i++)
	{
		m_wallPos.x = m_stageStart + i * m_wallSize;
		m_wallPos.z = kInFrontWallPosZ;
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
