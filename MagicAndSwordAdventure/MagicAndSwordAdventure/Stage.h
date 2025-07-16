#pragma once
#include "DxLib.h"
#include <vector>
class Stage
{
public:
	Stage();
	~Stage();
	void Init();
	void End();
	void Updata();
	void Draw();
private:
	int m_stageTileNum; // タイル1の個数
	int m_stageTileModelHandles[100];
	int m_stageGrateNum; // タイル2(格子)の個数
	int m_stageGrateModelHandle[20];
	int m_tileTotal; // 2種類のタイルの合計
	int m_stageStart; // 配置するタイルの始点
	int m_stageEnd; // 配置するタイルの終点
	float m_tileSize; // タイルのサイズ
	VECTOR m_tilePos;
	int m_tileModelBase;
	int m_tileGrateModelBase;
	std::vector<int> m_tileModelHandles;
	std::vector<int> m_tileGrateModelHandles;
};

