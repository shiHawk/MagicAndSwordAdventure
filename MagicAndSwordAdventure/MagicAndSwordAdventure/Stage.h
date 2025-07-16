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
	int m_stageTileNum; // �^�C��1�̌�
	int m_stageTileModelHandles[100];
	int m_stageGrateNum; // �^�C��2(�i�q)�̌�
	int m_stageGrateModelHandle[20];
	int m_tileTotal; // 2��ނ̃^�C���̍��v
	int m_stageStart; // �z�u����^�C���̎n�_
	int m_stageEnd; // �z�u����^�C���̏I�_
	float m_tileSize; // �^�C���̃T�C�Y
	VECTOR m_tilePos;
	int m_tileModelBase;
	int m_tileGrateModelBase;
	std::vector<int> m_tileModelHandles;
	std::vector<int> m_tileGrateModelHandles;
};

