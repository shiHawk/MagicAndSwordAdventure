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
	m_effectHandle = LoadEffekseerEffect("Data/effect/hit_eff.efk"); // �G�t�F�N�g�̃��[�h
	m_playEffectHandle = -1;
}


void EffectManager::End()
{
	DeleteEffekseerEffect(m_effectHandle);
}

void EffectManager::Update()
{
	// �v���C���[�̔�e�G�t�F�N�g
	m_isHitPlayer = m_pCollision->GetIsPlayerHit();
	if (m_isHitPlayer && !m_wasHitPlayer)
	{
		m_playEffectHandle = PlayEffekseer3DEffect(m_effectHandle); // �G�t�F�N�g���Đ�
		SetScalePlayingEffekseer3DEffect(m_playEffectHandle, kPlayerHitEffectSize, kPlayerHitEffectSize, kPlayerHitEffectSize); // �G�t�F�N�g�̃T�C�Y��ݒ�
		SetHitEffectPosPlayer(); // �G�t�F�N�g�̈ʒu��ݒ�
	}
	m_wasHitPlayer = m_isHitPlayer;

	// NormalSkelton�̔�e�G�t�F�N�g
	const auto& hitResults = m_pCollision->GetNormalSkeltonHit();
	m_isHitNormalSkelton = false;
	for (bool hit : hitResults)
	{
		if (hit)
		{
			m_isHitNormalSkelton = true;
			break; // ��ł�true��������΃��[�v�𔲂���
		}
	}
	if (m_isHitNormalSkelton && !m_wasHitNormalSkelton)
	{
		m_playEffectHandle = PlayEffekseer3DEffect(m_effectHandle); // �G�t�F�N�g���Đ�
		SetScalePlayingEffekseer3DEffect(m_playEffectHandle, kEnemyHitEffectSize, kEnemyHitEffectSize, kEnemyHitEffectSize); // �G�t�F�N�g�̃T�C�Y��ݒ�
		SetHitEffectPosNormalSkelton(); // �G�t�F�N�g�̈ʒu��ݒ�
	}
	m_wasHitNormalSkelton = m_isHitNormalSkelton; // �q�b�g���̍X�V

	// WizardSkelton�̔�e�G�t�F�N�g
	const auto& hitWizardResults = m_pCollision->GetWizardSkeltonHit();
	m_isHitWizardSkelton = false;
	for (bool hit : hitWizardResults)
	{
		if (hit)
		{
			m_isHitWizardSkelton = true;
			break; // ��ł�true��������΃��[�v�𔲂���
		}
	}
	if (m_isHitWizardSkelton && !m_wasHitWizardSkelton)
	{
		m_playEffectHandle = PlayEffekseer3DEffect(m_effectHandle); // �G�t�F�N�g���Đ�
		SetScalePlayingEffekseer3DEffect(m_playEffectHandle, kEnemyHitEffectSize, kEnemyHitEffectSize, kEnemyHitEffectSize); // �G�t�F�N�g�̃T�C�Y��ݒ�
		SetHitEffectPosWizardSkelton(); // �G�t�F�N�g�̈ʒu��ݒ�
	}
	m_wasHitWizardSkelton = m_isHitWizardSkelton; // �q�b�g���̍X�V
	
	UpdateEffekseer3D(); // �G�t�F�N�g�̍X�V
}

void EffectManager::Draw()
{
	Effekseer_Sync3DSetting(); // Effekseer�̕\���ݒ��DX���C�u�����̐ݒ�ɓ���������

	DrawEffekseer3D(); // �G�t�F�N�g�̕`��
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
