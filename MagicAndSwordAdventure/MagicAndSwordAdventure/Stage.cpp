#include "Stage.h"

namespace
{
	constexpr float kInFrontPosZ = -300.0f;
	constexpr float kInFrontWallPosZ = 350.0f;
	constexpr float kBackPosZ = 100.0f;
}

Stage::Stage():
	m_tileTotal(0),
	m_stageStart(0),
	m_stageEnd(0)
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
	m_tileModelBase = MV1LoadModel(L"Data/model/floor_tile_large.mv1");
	m_tileGrateModelBase = MV1LoadModel(L"Data/model/floor_tile_grate_open.mv1");
	m_wallModelBase = MV1LoadModel(L"Data/model/wall.mv1");
	m_wallCrackedModelBase = MV1LoadModel(L"Data/model/wall_cracked.mv1");

	if (m_tileModelBase == -1 || m_tileGrateModelBase == -1)
	{
		printfDx(L"読み込み失敗\n");
		return;
	}

	m_tileModelHandles.resize(m_tileTotal, -1);
	m_tileGrateModelHandles.resize(m_tileTotal, -1);
	m_wallModelHandles.resize(m_wallNum, -1);
	m_wallCrackedModelHandles.resize(m_wallNum, -1);
	
	// コピー元から複製
	for (int i = 0; i < m_tileTotal;i++)
	{
		if (i % 3 == 0)
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
		if (i % 6 == 0)
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

		if (i % 3 == 0 && m_tileGrateModelHandles[i] != -1)
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
		if (i % 6 == 0 && m_wallCrackedModelHandles[i] != -1)
		{
			MV1SetPosition(m_wallCrackedModelHandles[i], m_wallPos);
		}
		else
		{
			MV1SetPosition(m_wallModelHandles[i], m_wallPos);
		}
	}
}

void Stage::Draw()
{
	for (int i = 0; i < m_tileTotal;i++)
	{
		if (i % 3 == 0 && m_tileGrateModelHandles[i] != -1)
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
		if (i % 6 == 0 && m_wallCrackedModelHandles[i] != -1)
		{
			MV1DrawModel(m_wallCrackedModelHandles[i]);
		}
		else
		{
			MV1DrawModel(m_wallModelHandles[i]);
		}
	}
}
