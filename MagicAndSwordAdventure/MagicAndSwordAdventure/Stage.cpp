#include "Stage.h"

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
	m_tileTotal = 40;
	m_stageStart = -2500;
	m_stageEnd = 5500;
	m_tileSize = 400.0f;
	m_tileModelBase = MV1LoadModel(L"Data/model/floor_tile_large.mv1");
	m_tileGrateModelBase = MV1LoadModel(L"Data/model/floor_tile_grate_open.mv1");

	if (m_tileModelBase == -1 || m_tileGrateModelBase == -1)
	{
		printfDx(L"ì«Ç›çûÇ›é∏îs\n");
		return;
	}

	m_tileModelHandles.resize(m_tileTotal, -1);
	m_tileGrateModelHandles.resize(m_tileTotal, -1);
	
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

	m_tileModelHandles.clear();
	m_tileGrateModelHandles.clear();
	if (m_tileModelBase != -1) MV1DeleteModel(m_tileModelBase);
	if (m_tileGrateModelBase != -1) MV1DeleteModel(m_tileGrateModelBase);
}

void Stage::Updata()
{
	for (int i = 0; i < m_tileTotal;i++)
	{
		if (i < m_tileTotal / 2)
		{
			m_tilePos.x = m_stageStart + i * m_tileSize;
			m_tilePos.z = 100.0f;
		}
		// îºï™Ç©ÇÁêÊÇÕ2íiñ⁄Ç…
		else
		{
			m_tilePos.x = m_stageStart + i * m_tileSize - m_stageEnd + m_stageStart;
			m_tilePos.z = -300.0f;
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
}
