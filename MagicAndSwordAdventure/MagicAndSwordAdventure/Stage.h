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
	int m_tileTotal; // タイルの合計
	int m_wallNum; // 壁の数
	int m_stageStart; // 配置するタイルの始点
	int m_stageEnd; // 配置するタイルの終点
	float m_tileSize; // タイルのサイズ
	float m_wallSize; // 壁のサイズ
	VECTOR m_tilePos;
	VECTOR m_wallPos;
	int m_tileModelBase; // タイル1のコピー元
	int m_tileGrateModelBase; // タイル2のコピー元
	int m_wallModelBase; // 壁のコピー元
	int m_wallCrackedModelBase; // 壁2のコピー元
	std::vector<int> m_tileModelHandles;
	std::vector<int> m_tileGrateModelHandles;
	std::vector<int> m_wallModelHandles;
	std::vector<int> m_wallCrackedModelHandles;
};

