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
private:
	int m_destroyScore;
	int m_timeBonus;
	int m_score;
	int m_startTime;
	int m_elapsedTimeSeconds; // Œo‰ßŽžŠÔ(•b)
};

