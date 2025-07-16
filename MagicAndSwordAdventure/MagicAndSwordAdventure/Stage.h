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
	int m_tileTotal; // �^�C���̍��v
	int m_stageStart; // �z�u����^�C���̎n�_
	int m_stageEnd; // �z�u����^�C���̏I�_
	float m_tileSize; // �^�C���̃T�C�Y
	VECTOR m_tilePos;
	int m_tileModelBase; // �^�C��1�̃R�s�[��
	int m_tileGrateModelBase; // �^�C��2�̃R�s�[��
	std::vector<int> m_tileModelHandles;
	std::vector<int> m_tileGrateModelHandles;
};

