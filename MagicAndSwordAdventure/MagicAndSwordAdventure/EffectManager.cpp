#include "EffectManager.h"
#include "DxLib.h"
#include<EffekseerForDXLib.h>
namespace
{
	constexpr int kParticleMax = 8000;
	constexpr float kPlayerHitEffectSize = 20.0f;
	constexpr float kEnemyHitEffectSize = 30.0f;
}

EffectManager::EffectManager():
	m_effectHandle(-1),
	m_playEffectHandle(-1),
	m_wasHitPlayer(false),
	m_isHitPlayer(false),
	m_wasHitNormalSkelton(false),
	m_isHitNormalSkelton(false),
	m_wasHitWizardSkelton(false),
	m_isHitWizardSkelton(false)
{
}

EffectManager::~EffectManager()
{
}

void EffectManager::Init(std::shared_ptr<Collision> pCollision)
{
	m_pCollision = pCollision;
	m_effectHandle = LoadEffekseerEffect("Data/effect/hit_eff.efk"); // エフェクトのロード
	m_playEffectHandle = -1;
}


void EffectManager::End()
{
	DeleteEffekseerEffect(m_effectHandle);
}

void EffectManager::Update()
{
	// プレイヤーの被弾エフェクト
	m_isHitPlayer = m_pCollision->GetIsPlayerHit();
	if (m_isHitPlayer && !m_wasHitPlayer)
	{
		m_playEffectHandle = PlayEffekseer3DEffect(m_effectHandle); // エフェクトを再生
		SetScalePlayingEffekseer3DEffect(m_playEffectHandle, kPlayerHitEffectSize, kPlayerHitEffectSize, kPlayerHitEffectSize); // エフェクトのサイズを設定
		SetHitEffectPosPlayer(); // エフェクトの位置を設定
	}
	m_wasHitPlayer = m_isHitPlayer;

	// NormalSkeltonの被弾エフェクト
	const auto& hitResults = m_pCollision->GetNormalSkeltonHit();
	m_isHitNormalSkelton = false;
	for (bool hit : hitResults)
	{
		if (hit)
		{
			m_isHitNormalSkelton = true;
			break; // 一つでもtrueが見つかればループを抜ける
		}
	}
	if (m_isHitNormalSkelton && !m_wasHitNormalSkelton)
	{
		m_playEffectHandle = PlayEffekseer3DEffect(m_effectHandle); // エフェクトを再生
		SetScalePlayingEffekseer3DEffect(m_playEffectHandle, kEnemyHitEffectSize, kEnemyHitEffectSize, kEnemyHitEffectSize); // エフェクトのサイズを設定
		SetHitEffectPosNormalSkelton(); // エフェクトの位置を設定
	}
	m_wasHitNormalSkelton = m_isHitNormalSkelton; // ヒット情報の更新

	// WizardSkeltonの被弾エフェクト
	const auto& hitWizardResults = m_pCollision->GetWizardSkeltonHit();
	m_isHitWizardSkelton = false;
	for (bool hit : hitWizardResults)
	{
		if (hit)
		{
			m_isHitWizardSkelton = true;
			break; // 一つでもtrueが見つかればループを抜ける
		}
	}
	if (m_isHitWizardSkelton && !m_wasHitWizardSkelton)
	{
		m_playEffectHandle = PlayEffekseer3DEffect(m_effectHandle); // エフェクトを再生
		SetScalePlayingEffekseer3DEffect(m_playEffectHandle, kEnemyHitEffectSize, kEnemyHitEffectSize, kEnemyHitEffectSize); // エフェクトのサイズを設定
		SetHitEffectPosWizardSkelton(); // エフェクトの位置を設定
	}
	m_wasHitWizardSkelton = m_isHitWizardSkelton; // ヒット情報の更新
	
	UpdateEffekseer3D(); // エフェクトの更新
}

void EffectManager::Draw()
{
	Effekseer_Sync3DSetting(); // Effekseerの表示設定をDXライブラリの設定に同期させる

	DrawEffekseer3D(); // エフェクトの描画
}

void EffectManager::SetHitEffectPosPlayer()
{
	SetPosPlayingEffekseer3DEffect(m_playEffectHandle, m_pCollision->GetPlayerHitPos().x,
		m_pCollision->GetPlayerHitPos().y, m_pCollision->GetPlayerHitPos().z);
}

void EffectManager::SetHitEffectPosNormalSkelton()
{
	SetPosPlayingEffekseer3DEffect(m_playEffectHandle, m_pCollision->GetNormalSkeltonHitPos().x,
		m_pCollision->GetNormalSkeltonHitPos().y, m_pCollision->GetNormalSkeltonHitPos().z);
}

void EffectManager::SetHitEffectPosWizardSkelton()
{
	SetPosPlayingEffekseer3DEffect(m_playEffectHandle, m_pCollision->GetWizardSkeltonHitPos().x,
		m_pCollision->GetWizardSkeltonHitPos().y, m_pCollision->GetWizardSkeltonHitPos().z);
}
