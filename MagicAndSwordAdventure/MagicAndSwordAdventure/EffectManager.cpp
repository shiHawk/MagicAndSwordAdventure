#include "EffectManager.h"
#include "DxLib.h"
#include<EffekseerForDXLib.h>
namespace
{
	constexpr int kParticleMax = 8000;
}

EffectManager::EffectManager():
	m_effectHandle(-1)
{
}

EffectManager::~EffectManager()
{
}

void EffectManager::Init(std::shared_ptr<Collision> pCollision)
{
	//EffekseerŠÖŒW‰Šú‰»
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	Effekseer_Init(kParticleMax);
	//Effekseer_InitDistortion();
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
	Effekseer_Sync3DSetting();
	m_pCollision = pCollision;
	m_effectHandle = LoadEffekseerEffect("Data/effect/hit_eff.efk");
}


void EffectManager::End()
{
	DeleteEffekseerEffect(m_effectHandle);
}

void EffectManager::Update()
{
	SetPosPlayingEffekseer3DEffect(m_effectHandle,0.0f,0.0f,0.0f);
	UpdateEffekseer3D();
}

void EffectManager::Draw()
{
	Effekseer_Sync3DSetting();

	DrawEffekseer3D();
}

void EffectManager::SetHitEffectPosPlayer()
{
	SetPosPlayingEffekseer3DEffect(m_effectHandle, m_pCollision->GetPlayerHitPos().x, 
								   m_pCollision->GetPlayerHitPos().y, m_pCollision->GetPlayerHitPos().z);
	PlayEffekseer3DEffect(m_effectHandle);
}

void EffectManager::SetHitEffectPosNormalSkelton()
{
	SetPosPlayingEffekseer3DEffect(m_effectHandle, m_pCollision->GetNormalSkeltonHitPos().x,
		m_pCollision->GetNormalSkeltonHitPos().y, m_pCollision->GetNormalSkeltonHitPos().z);
	PlayEffekseer3DEffect(m_effectHandle);
}

void EffectManager::SetHitEffectPosWizardSkelton()
{
	SetPosPlayingEffekseer3DEffect(m_effectHandle, m_pCollision->GetWizardSkeltonHitPos().x,
		m_pCollision->GetWizardSkeltonHitPos().y, m_pCollision->GetWizardSkeltonHitPos().z);
	PlayEffekseer3DEffect(m_effectHandle);
}
