#pragma once
class Animation
{
public:
	Animation();
	~Animation();
	void Init();
	void AttachAnim(int modelHandle, int animNo);
	void Update();
	void ChangeAnim(int modelHandle, int animNo);
private:
	// 総再生時間
	float m_animTotalTime;
	// 再生時間
	float m_playTime;
	// 現在のアニメーションのアタッチ番号
	int m_currentAttachNo;
	// 次のアニメーションのアタッチ番号
	int m_nextAttachNo;
};

