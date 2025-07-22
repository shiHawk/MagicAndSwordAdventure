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
	void SetIsPlayerDead(bool isDead);
private:
	int m_destroyScore;
	int m_timeBonus;
	int m_score;
	int m_startTime;
	int m_elapsedTimeSeconds; // �o�ߎ���(�b)
	bool m_isPlayerDead; // �v���C���[�����񂾂�
};

