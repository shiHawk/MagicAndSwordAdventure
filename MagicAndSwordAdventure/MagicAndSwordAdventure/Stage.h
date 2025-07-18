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
	int m_wallNum; // �ǂ̐�
	int m_stageStart; // �z�u����^�C���̎n�_
	int m_stageEnd; // �z�u����^�C���̏I�_
	float m_tileSize; // �^�C���̃T�C�Y
	float m_wallSize; // �ǂ̃T�C�Y
	VECTOR m_tilePos;
	VECTOR m_wallPos;
	int m_tileModelBase; // �^�C��1�̃R�s�[��
	int m_tileGrateModelBase; // �^�C��2�̃R�s�[��
	int m_wallModelBase; // �ǂ̃R�s�[��
	int m_wallCrackedModelBase; // ��2�̃R�s�[��
	std::vector<int> m_tileModelHandles;
	std::vector<int> m_tileGrateModelHandles;
	std::vector<int> m_wallModelHandles;
	std::vector<int> m_wallCrackedModelHandles;
};

