#include "Player.h"
#include "DxLib.h"
#include "Pad.h"

namespace
{
	constexpr float kMoveSpeed = 5.0f;
	constexpr float kDashSpeed = 10.0f;
	constexpr float kJumpPower = 9.0f;
	constexpr float kJumpGravity = -0.4f;
	// à⁄ìÆå¿äE
	constexpr float kBackLimit = 100.0f;
	constexpr float kFrontLimit = -340.0f;

	// ìñÇΩÇËîªíËÇÃîÕàÕ
	constexpr float kColRadius = 50.0f;
	// ç≈ëÂHP
	constexpr int kMaxHp = 50;
	// å∏ë¨
	constexpr float kMoveDecRate = 0.80f;

	constexpr VECTOR kRightDir = { 0.0,270.0f * DX_PI_F / 180.0f,0.0f };
	constexpr VECTOR kLeftDir = { 0.0,90.0f * DX_PI_F / 180.0f,0.0f };

	const char* kIdleAnimName = "CharacterArmature|Idle";
	const char* kRunAnimName = "CharacterArmature|Run";
	bool isStartGravity = false;
}


Player::Player():m_handle(-1),
m_damageFrame(0),
m_hp(kMaxHp),
m_rotMtx(MGetIdent()),
m_angle(0.0f),
m_isJump(false),
m_isDirRight(true),
m_isEvade(false),
m_isPrevButton(false),
m_isNowButton(false),
m_playerHandle(0),
m_jumpCount(0),
m_evadeCount(0),
m_isAttackDirRight(true)
{
	m_pos = { 0, 0, 0 };
	m_vec = VGet(0, 0, 0);
}

Player::~Player()
{
}

void Player::Init(std::shared_ptr<Enemy> pEnemy)
{
	m_pos = VGet(-580, 0, 0);
	m_pEnemy = pEnemy;
	m_playerHandle = MV1LoadModel(L"Data/model/Barbarian.mv1");
	MV1SetRotationXYZ(m_playerHandle, kRightDir);
	m_attachIndex = MV1AttachAnim(m_playerHandle,1,-1);
	m_animTotalTime = MV1GetAttachAnimTotalTime(m_playerHandle,m_attachIndex);
}

void Player::End()
{
	MV1DeleteModel(m_playerHandle);
}

void Player::Update()
{
	UpdateAnime();
	if (m_vec.y > 0)
	{
		isStartGravity = true;
	}
	m_isNowButton = Pad::isPress(PAD_INPUT_2);
	if (isStartGravity)
	{
		m_vec.y += kJumpGravity;
	}
	
	if (Pad::isPress(PAD_INPUT_RIGHT))
	{
		if (!m_isEvade)
		{
			MV1SetRotationXYZ(m_playerHandle, kRightDir);
		}
		m_vec.x = kMoveSpeed;
		if (Pad::isPress(PAD_INPUT_3) && m_pos.y <= 0)
		{
			m_vec.x = kDashSpeed;
		}
		m_isAttackDirRight = true;
		m_isDirRight = true;
	}
	if (Pad::isPress(PAD_INPUT_LEFT))
	{
		if (!m_isEvade)
		{
			MV1SetRotationXYZ(m_playerHandle, kLeftDir);
		}
		m_vec.x = -kMoveSpeed;
		if (Pad::isPress(PAD_INPUT_3) && m_pos.y <= 0)
		{
			m_vec.x = -kDashSpeed;
		}
		m_isAttackDirRight = false;
		m_isDirRight = false;
	}
	if (Pad::isPress(PAD_INPUT_UP))
	{
		m_vec.z = kMoveSpeed;
	}
	if (Pad::isPress(PAD_INPUT_DOWN))
	{
		m_vec.z = -kMoveSpeed;
	}

	if (Pad::isTrigger(PAD_INPUT_1) && !m_isJump)
	{
		m_vec.y = kJumpPower;
		m_jumpCount++;
	}

	if (m_jumpCount > 1)
	{
		m_isJump = true;
	}

	if (Pad::isTrigger(PAD_INPUT_6) )
	{
		if (m_evadeCount < 1)
		{
			DoEvade();
		}
	}

	m_vec.x *= kMoveDecRate;
	m_vec.z *= kMoveDecRate;

	m_pos = VAdd(m_pos, m_vec);
	MV1SetScale(m_playerHandle,VGet(50,50,50));
	MV1SetPosition(m_playerHandle,m_pos);
	if (m_pos.y < 0.0f)
	{
		m_pos.y = 0.0f;
		m_vec.y = 0;
		m_isJump = false;
		m_isEvade = false;
		m_jumpCount = 0;
		m_evadeCount = 0;
	}

	if (m_isNowButton && !m_isPrevButton)
	{
		DoAttack();
	}
	m_isPrevButton = m_isNowButton;

	if (attack.active)
	{
		attack.timer--;
		attack.comboDuration--;
		if (attack.timer <= 0)
		{
			attack.active = false;
		}
	}
	printfDx(L"%f\n", m_screenPos.x);
}

void  Player::Draw() const
{
	MV1DrawModel(m_playerHandle);
#if _DEBUG
	if (attack.active)
	{
		DrawSphere3D(VGet(attack.x,attack.y,attack.z),attack.radius,8,0xff0000,0xffffff,false);
	}
#endif
}

VECTOR Player::GetScreenPos()
{
	m_screenPos = ConvWorldPosToScreenPos(m_pos);
	return m_screenPos;
}

float Player::GetColRadius() const
{
	return kColRadius; 
}

void Player::OnDamage()
{
}

void Player::DoAttack()
{
	attack.active = true;
	attack.timer = 10.0f;
	attack.comboDuration = 20.0f;
	attack.count++;
	if (attack.count > 4)
	{
		attack.count = 1;
	}
	printfDx(L"attack.count:%d", attack.count);
	if (m_isAttackDirRight)
	{
		m_vec.x = +kMoveSpeed * 0.5f;
		attack.x = m_pos.x + 50;
		if (attack.count == 4)
		{
			m_vec.x = +kMoveSpeed;
			attack.x = m_pos.x + 60;
		}
	}
	else 
	{
		m_vec.x = -kMoveSpeed * 0.5f;
		attack.x = m_pos.x - 50;
		if (attack.count == 4)
		{
			m_vec.x = -kMoveSpeed;
			attack.x = m_pos.x - 60;
		}
		
	}

	attack.y = m_pos.y+40;
	attack.z = m_pos.z;
}

void Player::DoEvade()
{
	m_evadeCount++;
	m_isEvade = true;
	if (m_isDirRight)
	{
		MV1SetRotationXYZ(m_playerHandle, kLeftDir);
		m_vec.x = kMoveSpeed * 2.5f;
		m_isAttackDirRight = false;
	}
	else
	{
		MV1SetRotationXYZ(m_playerHandle, kRightDir);
		m_vec.x = -kMoveSpeed * 2.5f;
		m_isAttackDirRight = true;
	}

	m_vec.y = kJumpPower * 0.5f;
}

void Player::AttachAnime(AnimData& data, const char* animName, bool isLoop)
{
	
}

void Player::UpdateAnime()
{
	animData.count += 100.0f;
	if (animData.count > m_animTotalTime)
	{
		animData.count = 0.0f;
	}
	MV1SetAttachAnimTime(m_playerHandle,m_attachIndex,animData.count);
}
