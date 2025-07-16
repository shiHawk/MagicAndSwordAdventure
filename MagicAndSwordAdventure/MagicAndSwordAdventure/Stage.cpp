#include "Stage.h"

Stage::Stage():
	m_stageTileNum(0),
	m_stageTileModelHandle(0),
	m_stageGrateNum(0),
	m_stageGrateModelHandle(0),
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
	m_tilePos = { 0.0f,0.0f,100.0f };
	m_stageTileNum = 40;
	m_stageGrateNum = 20;
	m_tileTotal = 20;
	m_stageStart = -2500;
	m_stageEnd = 5500;
	m_tileSize = 400.0f;
	m_stageTileModelHandle[0] = MV1LoadModel(L"Data/model/floor_tile_large.mv1");
	m_stageGrateModelHandle[0] = MV1LoadModel(L"Data/model/floor_tile_grate_open.mv1");
	for (int i = 1; i < m_stageTileNum;i++)
	{
		m_stageTileModelHandle[i] = MV1DuplicateModel(m_stageTileModelHandle[0]);
	}
	for (int i = 1; i < m_stageGrateNum;i++)
	{
		m_stageGrateModelHandle[i] = MV1DuplicateModel(m_stageGrateModelHandle[0]);
	}
}

void Stage::End()
{
	for (int i = 0; i < m_stageTileNum;i++)
	{
		MV1DeleteModel(m_stageTileModelHandle[i]);
	}
	for (int i = 0; i < m_stageGrateNum;i++)
	{
		MV1DeleteModel(m_stageGrateModelHandle[i]);
	}
}

void Stage::Updata()
{
	for (int i = 0; i < m_tileTotal;i++)
	{
		if (i % 3 == 0)
		{
			m_tilePos.x = m_stageStart + i * m_tileSize;
			MV1SetPosition(m_stageGrateModelHandle[i], m_tilePos);
		}
		else
		{
			m_tilePos.x = m_stageStart + i * m_tileSize;
			MV1SetPosition(m_stageTileModelHandle[i], m_tilePos);
		}
	}
}

void Stage::Draw()
{
	for (int i = 0; i < m_tileTotal;i++)
	{
		if (i % 3 == 0)
		{
			MV1DrawModel(m_stageGrateModelHandle[i]);
		}
		else
		{
			MV1DrawModel(m_stageTileModelHandle[i]);
		}
	}
}
