#include "EffectManager.h"
#include "DxLib.h"
#include<EffekseerForDXLib.h>
namespace
{
	constexpr int kParticleMax = 8000;
}

EffectManager::EffectManager():
	m_effectHandle(-1),
	m_playEffectHandle(-1),
	m_wasHitPlayer(false),
	m_isHitPlayer(false)
{
}

EffectManager::~EffectManager()
{
}

void EffectManager::Init(std::shared_ptr<Collision> pCollision)
{
	m_pCollision = pCollision;
	m_effectHandle = LoadEffekseerEffect("Data/effect/hit_eff.efk");
	m_playEffectHandle = -1;
}


void EffectManager::End()
{
	DeleteEffekseerEffect(m_effectHandle);
}

void EffectManager::Update()
{
	m_isHitPlayer = m_pCollision->GetIsPlayerHit();
	if (m_isHitPlayer && !m_wasHitPlayer)
	{
		m_playEffectHandle = PlayEffekseer3DEffect(m_effectHandle);
		SetScalePlayingEffekseer3DEffect(m_playEffectHandle, 10.0f, 10.0f, 10.0f);
		SetHitEffectPosPlayer();
	}
	m_wasHitPlayer = m_isHitPlayer;
	//SetPosPlayingEffekseer3DEffect(m_playEffectHandle, -1500.0f, 0.0f, 0.0f);
	UpdateEffekseer3D();
}

void EffectManager::Draw()
{
	Effekseer_Sync3DSetting();

	DrawEffekseer3D();
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
