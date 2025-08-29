#pragma once
class ScoreManager
{
public:
	ScoreManager();
	~ScoreManager(){}
	void Init();
	void End();
	void Update();
	void AddScore(int destroyScore);
	int GetDestroyScore();
	int GetTime();
	int GetTimeBonus();
	int GetScore();
	void HpBonus(int hp);
	int GetHpBonus();
	void SetIsPlayerDead(bool isDead);
private:
	int m_destroyScore; // ���j�X�R�A
	int m_timeBonus; // �^�C���{�[�i�X
	int m_hpBonus;
	int m_score; // �X�R�A�̍��v
	int m_startTime;
	int m_elapsedTimeSeconds; // �o�ߎ���(�b)
	bool m_isPlayerDead; // �v���C���[�����񂾂�
};

