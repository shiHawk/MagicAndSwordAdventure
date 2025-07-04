#include "WizardSkelton.h"

namespace
{
	constexpr VECTOR kLeftDir = { 0.0,90.0f * DX_PI_F / 180.0f,0.0f };
	constexpr VECTOR kRightDir = { 0.0,270.0f * DX_PI_F / 180.0f,0.0f };
}

WizardSkelton::WizardSkelton():
m_toPlayerDir({0.0f,0.0f,0.0f})
{
}

void WizardSkelton::Init(std::shared_ptr<Player> pPlayer)
{
	m_pPlayer = pPlayer;
	m_modelHandle = MV1LoadModel(L"Data/model/Skeleton_Mage.mv1");
	MV1SetScale(m_modelHandle, VGet(45, 45, 45));
	MV1SetRotationXYZ(m_modelHandle, kLeftDir);
	AttachAnim(m_modelHandle, 41);
}

void WizardSkelton::End()
{
}

void WizardSkelton::Update()
{
}

void WizardSkelton::DoAttack()
{
}

void WizardSkelton::Draw() const
{
}
