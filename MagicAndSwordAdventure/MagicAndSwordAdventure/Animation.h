#pragma once

class Animation
{
public:
	Animation();
	~Animation();
	void Init();
	void AttachAnim(int modelHandle, int animNo);
	void UpdateAnim();
	void ChangeAnim(int modelHandle, int animNo ,bool isLoop, float increment);
	void UpdateBlendAnim(int modelHandle, int animNo);
	void StartBlending();
	bool isNowPlaying() { return m_isNowPlaying; }
	bool GetIsAnimEnd();
	int GetAttachAnimNo() { return m_attachAnimNo; }
	float GetPlayTime() { return m_currentPlayTime; }
private:
	// 総再生時間
	float m_animTotalTime;
	// 再生時間
	float m_playTime;
	float m_currentPlayTime;
	float m_nextPlayTime;
	// 以前のアニメーションのアタッチ番号
	int m_oldAttachNo;
	// 現在のアニメーションのアタッチ番号
	int m_currentAttachNo;
	// 次のアニメーションのアタッチ番号
	int m_nextAttachNo;
	// modelのハンドル
	int m_modelHandle;
	// アニメーションをループさせるか否か
	bool m_isLoop;
	// アニメーションが終了しているか
	bool m_isEnd;
	// 再生時間の増分
	float m_timeIncrement;
	// アニメーションのブレンド率
	float m_blendRate;
	// ブレンドの開始
	bool m_isBlending;
	bool m_isNowPlaying;
	float m_blendTime;
	int m_attachAnimNo;
};

