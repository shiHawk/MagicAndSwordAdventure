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
	int m_stageStart; // 配置するタイルの始点
	int m_stageEnd; // 配置するタイルの終点
	float m_tileSize; // タイルのサイズ
	VECTOR m_tilePos;
	int m_tileModelBase; // タイル1のコピー元
	int m_tileGrateModelBase; // タイル2のコピー元
	std::vector<int> m_tileModelHandles;
	std::vector<int> m_tileGrateModelHandles;
};

