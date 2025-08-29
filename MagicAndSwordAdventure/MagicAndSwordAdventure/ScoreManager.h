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
	int m_destroyScore; // 撃破スコア
	int m_timeBonus; // タイムボーナス
	int m_hpBonus;
	int m_score; // スコアの合計
	int m_startTime;
	int m_elapsedTimeSeconds; // 経過時間(秒)
	bool m_isPlayerDead; // プレイヤーが死んだか
};

