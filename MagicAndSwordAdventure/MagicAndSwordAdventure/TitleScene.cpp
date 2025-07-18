#include "TitleScene.h"
#include "DxLib.h"
#include "GameScene.h"
#include "Pad.h"
#include "game.h"

namespace
{
	constexpr int kMaxFadeBright = 255;
	// フェード速度
	constexpr int kFadeSpeed = 8;
}

TitleScene::TitleScene()
{
}

void TitleScene::Init()
{
	m_fadeBright = 0;
	m_fadeSpeed = -kFadeSpeed;
}

void TitleScene::End()
{
}

SceneBase* TitleScene::Update()
{
	UpdateFade();
	if(!m_isNextScene && !IsFadingOut() && Pad::isTrigger(PAD_INPUT_2))
	{
		StartFadeOut();
		m_isNextScene = true;
	}
	if (m_isNextScene && IsFadeComplete())
	{
		return new GameScene();
	}
	return this;
}

void TitleScene::Draw()
{
	DrawFade();
	DrawSphere3D(VGet(500,500,0), 40, 16, 0xff00ff, 0xffffff, false);
}
