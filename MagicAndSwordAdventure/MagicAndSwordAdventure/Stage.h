#pragma once
#include "DxLib.h"
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
	int m_stageTileModelHandle[100];
	int m_stageGrateNum; // �^�C��2�̌�
	int m_stageGrateModelHandle[20];
	int m_tileTotal; // 2��ނ̃^�C���̍��v
	int m_stageStart; // �z�u����^�C���̎n�_
	int m_stageEnd; // �z�u����^�C���̏I�_
	float m_tileSize; // �^�C���̃T�C�Y
	VECTOR m_tilePos;
};

